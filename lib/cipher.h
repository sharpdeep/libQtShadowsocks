/*
 * cipher.h - the header file of Cipher class
 *
 * Communicate with lower-level encrytion library
 *
 * Seperated from Encryptor enables us to change low-level library easier.
 * If there is a modification associated with encryption/decryption, it's
 * this class that needs changes instead of messing up lots of classes.
 *
 * Copyright (C) 2014-2015 Symeon Huang <hzwhuang@gmail.com>
 *
 * This file is part of the libQtShadowsocks.
 *
 * libQtShadowsocks is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * libQtShadowsocks is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with libQtShadowsocks; see the file LICENSE. If not, see
 * <http://www.gnu.org/licenses/>.
 */

#ifndef CIPHER_H
#define CIPHER_H

#include <array>
#include <QObject>
#include <QMap>
#include <QCryptographicHash>
#include <botan/pipe.h>
#include <botan/version.h>
#include "rc4.h"
#include "chacha.h"

namespace QSS {

#if BOTAN_VERSION_CODE < BOTAN_VERSION_CODE_FOR(1,10,0)
#error "Botan library is too old."
#elif BOTAN_VERSION_CODE < BOTAN_VERSION_CODE_FOR(1,11,0)
typedef Botan::SecureVector<Botan::byte> SecureByteArray;
#define DataOfSecureByteArray(sba) sba.begin()
#else
typedef Botan::secure_vector<Botan::byte> SecureByteArray;
#define DataOfSecureByteArray(sba) sba.data()
#endif

class Cipher : public QObject
{
    Q_OBJECT
public:
    explicit Cipher(const QByteArray &method, const QByteArray &key, const QByteArray &iv, bool encode, QObject *parent = 0);
    Cipher(Cipher &&) = default;
    ~Cipher();

    QByteArray update(const QByteArray &data);

    typedef std::array<int, 2> kiLenArray;

    /*
     * keyIvMap contains required key length and IV length
     * The kiLenArray contains two integers, the first one is key length,
     * while the second one is IV length.
     */
    static const QMap<QByteArray, kiLenArray> keyIvMap;

    static QByteArray randomIv(int length);
    inline static QByteArray md5Hash(const QByteArray &in)
    {
        return QCryptographicHash::hash(in, QCryptographicHash::Md5);
    }

    static bool isSupported(const QByteArray &method);

private:
    Botan::Pipe *pipe;
    RC4 *rc4;
    ChaCha *chacha;

    static QMap<QByteArray, kiLenArray> generateKeyIvMap();
};

}

#endif // CIPHER_H
