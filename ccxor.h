#ifndef CCXOR_H
#define CCXOR_H

#include <QString>
#include <QTextCodec>

class ccXor
{

public:
    ccXor();
    ~ccXor();

public:
    QString XorEncryptDecrypt(const QString &, const char &);

private:
    QString byteToQString(const QByteArray &byte);
    QByteArray qstringToByte(const QString &strInfo);

};

#endif // CCXOR_H
