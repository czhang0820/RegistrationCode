#include "ccxor.h"

ccXor::ccXor()
{

}

ccXor::~ccXor()
{

}

QString ccXor::XorEncryptDecrypt(const QString &str, const char &key)
{
    QString result;
    QByteArray bs = this->qstringToByte(str);
    for(int i=0; i<bs.size();i++) {
        bs[i] = bs[i] ^ key;
     }
    result = byteToQString(bs);
    return result;
}

QString ccXor::byteToQString(const QByteArray &byte)
{
    QString result;
    if(byte.size() > 0) {
        QTextCodec *codec = QTextCodec::codecForName("utf-8");
        result = codec->toUnicode(byte);
     }
    return result;
}

QByteArray ccXor::qstringToByte(const QString &strInfo)
{
    QByteArray result;
    if(strInfo.length() > 0) {
        QTextCodec *codec = QTextCodec::codecForName("utf-8");
        result = codec->fromUnicode(strInfo);
     }
    return result;
}
