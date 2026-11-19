#pragma once
/* returns length written (0 on error) */
int ax_encrypt(const char *key, const char *text, char *out, int outmax);
int ax_decrypt(const char *key, const char *b64, char *out, int outmax);
