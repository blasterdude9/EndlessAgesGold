using System;
using System.Collections.Generic;
using System.Configuration;
using System.Security.Cryptography;
using System.Text;

/// <summary>
/// Pure C# replacement for the native AXEncryptX.dll (originally AuraX/AXEncryptionLib.cpp + AXEncryptDLL/aes.cpp).
/// Same format as the C++ servers: Base64( IV[16] | AES-256-CBC blocks | HMAC-SHA256[32] ),
/// key = SHA-256 of (IV + key) iterated 8192 times. Strings it produces decrypt in the C++ code and vice versa.
/// The key is read from Web.config: appSettings "AXEncryptKey".
/// </summary>
public class AXencrypt
{
    /// <summary>
    /// The original C++ Encrypt() always copied the FIRST 16 characters into every block, so any
    /// text longer than 16 characters came out scrambled (e.g. "abcdefghijklmnopqrs" -> "abcdefghijklmnopabc").
    /// false = behave exactly like the original (matches existing database data). true = encrypt correctly.
    /// Decryption is correct either way.
    /// </summary>
    public static bool FixLongStringBug = false;

    private static readonly Encoding Ansi = Encoding.Default;
    private const string B64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    private readonly byte[] m_key;

    public AXencrypt()
    {
        string key = ConfigurationManager.AppSettings["AXEncryptKey"];
        if (String.IsNullOrEmpty(key))
            throw new ConfigurationErrorsException("AXEncryptKey is missing from <appSettings> in Web.config.");
        m_key = Ansi.GetBytes(key);
    }

    public AXencrypt(string key)
    {
        m_key = Ansi.GetBytes(key);
    }

    public string Encrypt(string inputString)
    {
        byte[] data = Ansi.GetBytes(inputString ?? "");
        long fileSize = data.Length;

        // IV = SHA256( time[8] + length[8] + "test" ), low 4 bits of byte 15 = length mod 16
        long now = (long)(DateTime.UtcNow - new DateTime(1970, 1, 1, 0, 0, 0, DateTimeKind.Utc)).TotalSeconds;
        byte[] buffer = new byte[16];
        for (int i = 0; i < 8; i++)
        {
            buffer[i] = (byte)(now >> (i * 8));
            buffer[i + 8] = (byte)(fileSize >> (i * 8));
        }
        byte[] iv = new byte[16];
        Array.Copy(Sha(buffer, Encoding.ASCII.GetBytes("test")), iv, 16);
        iv[15] = (byte)((iv[15] & 0xF0) | (int)(fileSize & 0x0F));

        List<byte> output = new List<byte>(iv);
        byte[] digest = DeriveKey(iv);
        byte[] cipherText = new byte[((fileSize + 15) / 16) * 16];

        using (Aes aes = CreateAes(digest))
        using (ICryptoTransform enc = aes.CreateEncryptor())
        {
            int written = 0;
            for (long offset = 0; offset < fileSize; offset += 16)
            {
                int n = (int)Math.Min(16, fileSize - offset);
                int src = FixLongStringBug ? (int)offset : 0;
                Array.Copy(data, src, buffer, 0, n);   // bytes after n keep their previous contents, as in the C code
                for (int i = 0; i < 16; i++)
                    buffer[i] ^= iv[i];
                enc.TransformBlock(buffer, 0, 16, buffer, 0);
                Array.Copy(buffer, 0, cipherText, written, 16);
                written += 16;
                Array.Copy(buffer, iv, 16);
            }
        }

        output.AddRange(cipherText);
        output.AddRange(Hmac(digest, cipherText));
        return Base64EncodeAtl(output.ToArray());
    }

    public string Decrypt(string inputString)
    {
        byte[] raw = Base64DecodeAtl(inputString ?? "");
        if (raw.Length < 48 || (raw.Length & 0x0F) != 0)
            return "";

        int fileSize = raw.Length - 48;
        byte[] iv = new byte[16];
        Array.Copy(raw, 0, iv, 0, 16);
        int lastN = iv[15] & 0x0F;
        byte[] digest = DeriveKey(iv);

        List<byte> plain = new List<byte>();
        byte[] block = new byte[16];
        using (Aes aes = CreateAes(digest))
        using (ICryptoTransform dec = aes.CreateDecryptor())
        {
            for (int offset = 0; offset < fileSize; offset += 16)
            {
                dec.TransformBlock(raw, 16 + offset, 16, block, 0);
                for (int i = 0; i < 16; i++)
                    block[i] ^= iv[i];
                Array.Copy(raw, 16 + offset, iv, 0, 16);
                int n = (lastN > 0 && offset == fileSize - 16) ? lastN : 16;
                for (int i = 0; i < n; i++)
                    plain.Add(block[i]);
            }
        }
        // The original returned the text even when the HMAC check failed; kept the same so behaviour matches.

        int end = plain.IndexOf(0);
        if (end < 0) end = plain.Count;
        return Ansi.GetString(plain.ToArray(), 0, end);
    }

    // ---- helpers ----

    private byte[] DeriveKey(byte[] iv)
    {
        byte[] digest = new byte[32];
        Array.Copy(iv, digest, 16);
        for (int i = 0; i < 8192; i++)
            digest = Sha(digest, m_key);
        return digest;
    }

    private static byte[] Hmac(byte[] digest, byte[] cipherText)
    {
        byte[] ipad = new byte[64], opad = new byte[64];
        for (int i = 0; i < 64; i++) { ipad[i] = 0x36; opad[i] = 0x5C; }
        for (int i = 0; i < 32; i++) { ipad[i] ^= digest[i]; opad[i] ^= digest[i]; }
        return Sha(opad, Sha(ipad, cipherText));
    }

    private static byte[] Sha(byte[] a, byte[] b)
    {
        using (SHA256 sha = SHA256.Create())
        {
            sha.TransformBlock(a, 0, a.Length, null, 0);
            sha.TransformFinalBlock(b, 0, b.Length);
            return sha.Hash;
        }
    }

    private static Aes CreateAes(byte[] key)
    {
        Aes aes = Aes.Create();
        aes.Mode = CipherMode.ECB;
        aes.Padding = PaddingMode.None;
        aes.Key = key;
        return aes;
    }

    // ATL Base64Encode with ATL_BASE64_FLAG_NONE: CRLF after every 76 characters.
    private static string Base64EncodeAtl(byte[] data)
    {
        string s = Convert.ToBase64String(data);
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < s.Length; i += 76)
        {
            if (i > 0) sb.Append("\r\n");
            sb.Append(s, i, Math.Min(76, s.Length - i));
        }
        return sb.ToString();
    }

    // ATL Base64Decode: characters outside the Base64 alphabet (CR, LF, '=', spaces) are skipped.
    private static byte[] Base64DecodeAtl(string s)
    {
        List<byte> result = new List<byte>();
        int acc = 0, bits = 0;
        foreach (char c in s)
        {
            int v = B64.IndexOf(c);
            if (v < 0) continue;
            acc = (acc << 6) | v;
            bits += 6;
            if (bits >= 8)
            {
                bits -= 8;
                result.Add((byte)((acc >> bits) & 0xFF));
            }
        }
        return result.ToArray();
    }
}
