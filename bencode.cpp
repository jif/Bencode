/****************************************************************************
 **
 ** Bencode
 **
 ** The the bencode class handles parsing and encoding bencode (from the
 ** BitTorrent specification) strings.
 **
 ****************************************************************************/
#include <QDebug>
#include <QStringRef>

#include "bencode.h"

const QChar intChar = QChar('i');
const QChar listChar = QChar('l');
const QChar dictChar = QChar('d');
const QChar strDelimChar = QChar(':');
const QChar endChar = QChar('e');
const QChar negChar = QChar('-');

Bencode::Bencode(QObject *parent) :
    QObject(parent)
{
}

QList<QVariant> Bencode::fromBEncodedString(const QString &bEncodedString) {
    bEncoded = bEncodedString;
    pos = 0;

    QList<QVariant> listBuf;
    while(pos<bEncoded.length()) {
        qDebug("=== Current string pos=%i char=%s ================", pos, qPrintable(currentChar()));
        listBuf.append(parseObject());
        qDebug() << listBuf;
    }
    return listBuf;
}

QVariant Bencode::parseObject() {
    if(currentChar() == intChar) {
        return parseInteger();
    } else if(currentChar() == listChar) {
        return parseList();
    } else if(currentChar() == dictChar) {
        return parseDictionary();
    } else if(currentChar().isNumber()) {
        return parseString();
    }
    pos++;
    return QVariant::QVariant();
}

int Bencode::parseInteger() {
    // Validate the integer
    if(currentChar() != intChar)
        return 0;

    pos++;
    int len = bEncoded.indexOf(endChar,pos)-pos;
    if(!len>0)
        return 0;

    pos = pos+len+1;
    return bEncoded.mid(pos-len-1,len).toInt();
}

QString Bencode::parseString() {
    int lenLen = bEncoded.indexOf(strDelimChar,pos)-pos;
    if(!lenLen>0)
        return QString::QString();

    int len = bEncoded.mid(pos,lenLen).toInt();
    pos = pos+lenLen+1;

    if(!len>0)
        return QString::QString();

    pos = pos+len;
    return bEncoded.mid(pos-len,len);
}

QList<QVariant> Bencode::parseList() {
    if(currentChar() != listChar)
        return QList<QVariant>::QList();

    pos++;
    QList<QVariant> listBuf;
    while(currentChar() != endChar)
        listBuf.append(parseObject());

    pos++;
    return listBuf;
}

QHash<QString,QVariant> Bencode::parseDictionary() {
    QHash<QString,QVariant> hashBuf;
    if(currentChar() != dictChar)
        return hashBuf;

    pos++;
    while(currentChar() != endChar) {
        QString keyBuf = parseString();
        QVariant valBuf = parseObject();
        if(keyBuf.isEmpty() || valBuf.isNull())
            return hashBuf;

        hashBuf.insert(keyBuf,valBuf);
        keyBuf.clear();
        valBuf.clear();
    }
    pos++;
    return hashBuf;
}

QChar Bencode::currentChar() {
    if(pos >= bEncoded.size())
        return QChar::QChar();
    return bEncoded.at(pos);
}
