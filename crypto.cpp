#include "crypto.h"

#include <iostream>
#include <string>
#include <limits>
#include <openssl/err.h>

#include <QDebug>

static const unsigned int KEY_SIZE = 32;
static const unsigned int BLOCK_SIZE = 16;

typedef unsigned char byte;

static void error() {
    qFatal("%s", ERR_error_string(0, nullptr));
}

Crypto::Crypto(const QByteArray &key, const QByteArray &iv)
    : ctx(EVP_CIPHER_CTX_free_ptr(EVP_CIPHER_CTX_new(), ::EVP_CIPHER_CTX_free))
    , key(key)
    , iv(iv)
{
    static bool once = false;
    if (once) {
        once = false;
        EVP_add_cipher(EVP_aes_256_cbc());
    }
}

void Crypto::encrypt(const QByteArray &ptext, QByteArray &ctext)
{
    auto _key = (unsigned char*)key.data();
    auto _iv = (unsigned char*)iv.data();
    if (1 != EVP_EncryptInit_ex(ctx.get(), EVP_aes_256_cbc(), NULL, _key, _iv)) {
        error();
    }

    ctext.resize(ptext.size()+BLOCK_SIZE);
    int out_len1 = (int)ctext.size();

    if (1 != EVP_EncryptUpdate(ctx.get(), (byte*)ctext.begin(), &out_len1, (const byte*)ptext.begin(), (int)ptext.size())) {
        error();
    }

    int out_len2 = (int)ctext.size() - out_len1;
    if (1 != EVP_EncryptFinal_ex(ctx.get(), (byte*)ctext.begin()+out_len1, &out_len2)) {
        error();
    }

    ctext.resize(out_len1 + out_len2);
}

void Crypto::decrypt(const QByteArray &ctext, QByteArray &rtext)
{
    auto _key = (unsigned char*)key.data();
    auto _iv = (unsigned char*)iv.data();
    if (1 != EVP_DecryptInit_ex(ctx.get(), EVP_aes_256_cbc(), NULL, _key, _iv)) {
        error();
    }

    rtext.resize(ctext.size());
    int out_len1 = (int)rtext.size();

    if (1 != EVP_DecryptUpdate(ctx.get(), (byte*)rtext.begin(), &out_len1, (const byte*)ctext.begin(), (int)ctext.size())) {
        error();
    }

    int out_len2 = (int)rtext.size() - out_len1;
    if (1 != EVP_DecryptFinal_ex(ctx.get(), (byte*)rtext.begin()+out_len1, &out_len2)) {
        error();
    }

    rtext.resize(out_len1 + out_len2);
}
