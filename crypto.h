#ifndef CRYPTO_H
#define CRYPTO_H

#include <memory>

#include <openssl/evp.h>

#include <QByteArray>

using EVP_CIPHER_CTX_free_ptr = std::unique_ptr<EVP_CIPHER_CTX, decltype(&::EVP_CIPHER_CTX_free)>;

class Crypto
{
public:
    Crypto(const QByteArray& key, const QByteArray& iv);

    void encrypt(const QByteArray& plaintext, QByteArray& ciphertext);
    void decrypt(const QByteArray& ciphertext, QByteArray& plaintext);

private:
    EVP_CIPHER_CTX_free_ptr ctx;
    const QByteArray key;
    const QByteArray iv;
};

#endif // CRYPTO_H
