#include "sha256.h"
#include "atlenc.h"
#ifndef _AES_H
#define _AES_H

#ifndef uint8
#define uint8  unsigned char
#endif

#ifndef uint32
#define uint32 unsigned long int
#endif



typedef struct
{
    uint32 erk[64];     /* encryption round keys */
    uint32 drk[64];     /* decryption round keys */
    int nr;             /* number of rounds */
}
aes_context;

int  aes_set_key( aes_context *ctx, uint8 *key, int nbits );
void aes_encrypt( aes_context *ctx, uint8 input[16], uint8 output[16] );
void aes_decrypt( aes_context *ctx, uint8 input[16], uint8 output[16] );
int Encrypt(char * bufferToEncrypt, char * encryptedBuffer, long * encryptedBufLen, char * key);
int DecyptEnc();
int Decrypt(char * encryptedBuffer, char * decryptedBuffer, int & encryptedLength, char * key);
int Encrypt64(char * bufferToEncrypt,char * encryptedBuffer,char * key);
int Decrypt64(char * encryptedBuffer,char * decryptedBuffer,char * key);

#endif /* aes.h */
