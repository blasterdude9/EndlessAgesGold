/* AXEncryptionLib in plain C: Base64( IV[16] | AES-256-CBC | HMAC-SHA256[32] ), key = SHA256(IV+key) x 8192.
   Same format as EAServer/Data/AXencrypt.cs and the client's CAXEncryptionLib. */
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "axcrypt.h"

/* ---------------- SHA-256 ---------------- */
typedef struct { uint32_t s[8]; uint64_t n; uint8_t b[64]; size_t bl; } sha256;
static const uint32_t K[64] = {
0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,
0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,
0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,
0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2};
#define ROR(x,n) (((x)>>(n))|((x)<<(32-(n))))
static void sha_block(sha256 *c, const uint8_t *p) {
    uint32_t w[64], a,b,d,e,f,g,h,cc,t1,t2; int i;
    for (i = 0; i < 16; i++) w[i] = (uint32_t)p[i*4]<<24 | (uint32_t)p[i*4+1]<<16 | (uint32_t)p[i*4+2]<<8 | p[i*4+3];
    for (; i < 64; i++) w[i] = (ROR(w[i-2],17)^ROR(w[i-2],19)^(w[i-2]>>10)) + w[i-7] + (ROR(w[i-15],7)^ROR(w[i-15],18)^(w[i-15]>>3)) + w[i-16];
    a=c->s[0];b=c->s[1];cc=c->s[2];d=c->s[3];e=c->s[4];f=c->s[5];g=c->s[6];h=c->s[7];
    for (i = 0; i < 64; i++) {
        t1 = h + (ROR(e,6)^ROR(e,11)^ROR(e,25)) + ((e&f)^(~e&g)) + K[i] + w[i];
        t2 = (ROR(a,2)^ROR(a,13)^ROR(a,22)) + ((a&b)^(a&cc)^(b&cc));
        h=g; g=f; f=e; e=d+t1; d=cc; cc=b; b=a; a=t1+t2;
    }
    c->s[0]+=a;c->s[1]+=b;c->s[2]+=cc;c->s[3]+=d;c->s[4]+=e;c->s[5]+=f;c->s[6]+=g;c->s[7]+=h;
}
static void sha_init(sha256 *c) { static const uint32_t iv[8]={0x6a09e667,0xbb67ae85,0x3c6ef372,0xa54ff53a,0x510e527f,0x9b05688c,0x1f83d9ab,0x5be0cd19}; memcpy(c->s,iv,32); c->n=0; c->bl=0; }
static void sha_add(sha256 *c, const uint8_t *p, size_t n) {
    c->n += n;
    while (n) { size_t k = 64 - c->bl; if (k > n) k = n; memcpy(c->b + c->bl, p, k); c->bl += k; p += k; n -= k; if (c->bl == 64) { sha_block(c, c->b); c->bl = 0; } }
}
static void sha_end(sha256 *c, uint8_t out[32]) {
    uint64_t bits = c->n * 8; uint8_t pad = 0x80, z = 0, len[8]; int i;
    sha_add(c, &pad, 1); while (c->bl != 56) sha_add(c, &z, 1);
    for (i = 0; i < 8; i++) len[i] = (uint8_t)(bits >> (56 - 8*i));
    sha_add(c, len, 8);
    for (i = 0; i < 8; i++) { out[i*4]=c->s[i]>>24; out[i*4+1]=c->s[i]>>16; out[i*4+2]=c->s[i]>>8; out[i*4+3]=c->s[i]; }
}
static void sha2(const uint8_t *a, size_t an, const uint8_t *b, size_t bn, uint8_t out[32]) { sha256 c; sha_init(&c); sha_add(&c,a,an); sha_add(&c,b,bn); sha_end(&c,out); }

/* ---------------- AES-256 (encrypt + decrypt, one block) ---------------- */
static uint8_t SB[256], ISB[256];
static void aes_tables(void) {
    static int done = 0; uint8_t p = 1, q = 1; if (done) return; done = 1;
    do {
        p = p ^ (uint8_t)(p << 1) ^ (p & 0x80 ? 0x1B : 0);
        q ^= q << 1; q ^= q << 2; q ^= q << 4; if (q & 0x80) q ^= 0x09;
        { uint8_t x = q ^ (uint8_t)((q<<1)|(q>>7)) ^ (uint8_t)((q<<2)|(q>>6)) ^ (uint8_t)((q<<3)|(q>>5)) ^ (uint8_t)((q<<4)|(q>>4)); SB[p] = x ^ 0x63; }
    } while (p != 1);
    SB[0] = 0x63;
    for (int i = 0; i < 256; i++) ISB[SB[i]] = (uint8_t)i;
}
static uint8_t xt(uint8_t x) { return (uint8_t)((x << 1) ^ ((x & 0x80) ? 0x1B : 0)); }
static uint8_t mul(uint8_t a, uint8_t b) { uint8_t r = 0; while (b) { if (b & 1) r ^= a; a = xt(a); b >>= 1; } return r; }
static void aes_key(const uint8_t key[32], uint8_t rk[240]) {
    uint8_t rc = 1; int i; aes_tables(); memcpy(rk, key, 32);
    for (i = 32; i < 240; i += 4) {
        uint8_t t[4]; memcpy(t, rk + i - 4, 4);
        if (i % 32 == 0) { uint8_t u = t[0]; t[0] = SB[t[1]] ^ rc; t[1] = SB[t[2]]; t[2] = SB[t[3]]; t[3] = SB[u]; rc = xt(rc); }
        else if (i % 32 == 16) { t[0]=SB[t[0]]; t[1]=SB[t[1]]; t[2]=SB[t[2]]; t[3]=SB[t[3]]; }
        for (int j = 0; j < 4; j++) rk[i+j] = rk[i-32+j] ^ t[j];
    }
}
static void aes_enc(const uint8_t rk[240], uint8_t s[16]) {
    int r, i; uint8_t t[16];
    for (i = 0; i < 16; i++) s[i] ^= rk[i];
    for (r = 1; r <= 14; r++) {
        for (i = 0; i < 16; i++) t[i] = SB[s[(i + 4*(i%4)) % 16]];      /* SubBytes + ShiftRows */
        if (r != 14) for (i = 0; i < 4; i++) {
            uint8_t *c = t + 4*i, a0=c[0],a1=c[1],a2=c[2],a3=c[3];
            c[0]=xt(a0)^xt(a1)^a1^a2^a3; c[1]=a0^xt(a1)^xt(a2)^a2^a3; c[2]=a0^a1^xt(a2)^xt(a3)^a3; c[3]=xt(a0)^a0^a1^a2^xt(a3);
        }
        for (i = 0; i < 16; i++) s[i] = t[i] ^ rk[16*r + i];
    }
}
static void aes_dec(const uint8_t rk[240], uint8_t s[16]) {
    int r, i; uint8_t t[16];
    for (i = 0; i < 16; i++) s[i] ^= rk[224 + i];
    for (r = 13; r >= 0; r--) {
        for (i = 0; i < 16; i++) t[(i + 4*(i%4)) % 16] = ISB[s[i]];     /* InvShiftRows + InvSubBytes */
        for (i = 0; i < 16; i++) s[i] = t[i] ^ rk[16*r + i];
        if (r) for (i = 0; i < 4; i++) {
            uint8_t *c = s + 4*i, a0=c[0],a1=c[1],a2=c[2],a3=c[3];
            c[0]=mul(a0,14)^mul(a1,11)^mul(a2,13)^mul(a3,9); c[1]=mul(a0,9)^mul(a1,14)^mul(a2,11)^mul(a3,13);
            c[2]=mul(a0,13)^mul(a1,9)^mul(a2,14)^mul(a3,11); c[3]=mul(a0,11)^mul(a1,13)^mul(a2,9)^mul(a3,14);
        }
    }
}

/* ---------------- ATL Base64 (CRLF every 76) ---------------- */
static const char B64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static int b64enc(const uint8_t *d, int n, char *out) {
    int o = 0, col = 0;
    for (int i = 0; i < n; i += 3) {
        uint32_t v = (uint32_t)d[i] << 16 | (i+1<n ? d[i+1]<<8 : 0) | (i+2<n ? d[i+2] : 0);
        char q[4] = { B64[v>>18&63], B64[v>>12&63], i+1<n ? B64[v>>6&63] : '=', i+2<n ? B64[v&63] : '=' };
        for (int k = 0; k < 4; k++) { if (col == 76) { out[o++]='\r'; out[o++]='\n'; col = 0; } out[o++] = q[k]; col++; }
    }
    out[o] = 0; return o;
}
static int b64dec(const char *s, uint8_t *out, int max) {
    int n = 0, bits = 0; uint32_t acc = 0;
    for (; *s; s++) { const char *p = strchr(B64, *s); if (!p || !*s) continue; acc = acc << 6 | (uint32_t)(p - B64); bits += 6; if (bits >= 8) { bits -= 8; if (n < max) out[n++] = (uint8_t)(acc >> bits); } }
    return n;
}

static void derive(const uint8_t iv[16], const char *key, uint8_t digest[32]) {
    memset(digest, 0, 32); memcpy(digest, iv, 16);
    for (int i = 0; i < 8192; i++) sha2(digest, 32, (const uint8_t*)key, strlen(key), digest);
}

int ax_encrypt(const char *key, const char *text, char *out, int outmax) {
    int n = (int)strlen(text), blocks = (n + 15) / 16; uint8_t buf[16] = {0}, iv[16], digest[32], rk[240], raw[16 + 16*8 + 32];
    uint64_t now = (uint64_t)time(NULL), len = (uint64_t)n; uint8_t seed[16];
    if (n < 1 || n > 128 || outmax < 300) return 0;
    for (int i = 0; i < 8; i++) { seed[i] = (uint8_t)(now >> (8*i)); seed[8+i] = (uint8_t)(len >> (8*i)); }
    { uint8_t h[32]; sha2(seed, 16, (const uint8_t*)"test", 4, h); memcpy(iv, h, 16); }
    iv[15] = (uint8_t)((iv[15] & 0xF0) | (n & 0x0F));
    memcpy(raw, iv, 16); derive(iv, key, digest); aes_key(digest, rk);
    for (int b = 0; b < blocks; b++) {
        int k = n - 16*b < 16 ? n - 16*b : 16;
        memcpy(buf, text + 16*b, k);                       /* later bytes keep their old contents, like the original */
        for (int i = 0; i < 16; i++) buf[i] ^= iv[i];
        aes_enc(rk, buf); memcpy(raw + 16 + 16*b, buf, 16); memcpy(iv, buf, 16);
    }
    {   /* HMAC-SHA256(digest, ciphertext) */
        uint8_t ipad[64], opad[64], inner[32]; sha256 c;
        for (int i = 0; i < 64; i++) { ipad[i] = 0x36; opad[i] = 0x5C; }
        for (int i = 0; i < 32; i++) { ipad[i] ^= digest[i]; opad[i] ^= digest[i]; }
        sha_init(&c); sha_add(&c, ipad, 64); sha_add(&c, raw + 16, 16*blocks); sha_end(&c, inner);
        sha2(opad, 64, inner, 32, raw + 16 + 16*blocks);
    }
    return b64enc(raw, 16 + 16*blocks + 32, out);
}

int ax_decrypt(const char *key, const char *b64, char *out, int outmax) {
    uint8_t raw[1024], iv[16], digest[32], rk[240], blk[16]; int n = b64dec(b64, raw, sizeof raw), o = 0;
    out[0] = 0;
    if (n < 48 || (n & 15)) return 0;
    int size = n - 48, last = raw[15] & 15;
    memcpy(iv, raw, 16); derive(iv, key, digest); aes_key(digest, rk);
    for (int off = 0; off < size; off += 16) {
        memcpy(blk, raw + 16 + off, 16); aes_dec(rk, blk);
        for (int i = 0; i < 16; i++) blk[i] ^= iv[i];
        memcpy(iv, raw + 16 + off, 16);
        int k = (last > 0 && off == size - 16) ? last : 16;
        for (int i = 0; i < k && o < outmax - 1; i++) out[o++] = (char)blk[i];
    }
    out[o] = 0;
    return (int)strlen(out);
}
