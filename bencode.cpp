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

const static QChar intChar = QChar('i');
const static QChar listChar = QChar('l');
const static QChar dictChar = QChar('d');
const static QChar strDelimChar = QChar(':');
const static QChar endChar = QChar('e');
const static QChar negChar = QChar('-');

Bencode::Bencode(QObject *parent) :
    QObject(parent)
{
}

QList<QVariant> Bencode::fromBEncodedString(const QString &bEncodedString) {
    bEncoded.clear();
    bEncoded = bEncodedString;
    pos = 0;

    QList<QVariant> listBuf;
    while(pos<bEncoded.length()) {
        listBuf.append(parseObject());
    }
    return listBuf;
}

QString Bencode::toBEncodedString(const QList<QVariant> &variantList) {
    QString strBuf;

    if(variantList.empty())
        return strBuf;

    for(int i = 0; i<variantList.size(); ++i) {
        strBuf.append(encodeObject(variantList.at(i)));
    }
    return strBuf;
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

QString Bencode::encodeObject(const QVariant &variant) {
    switch(variant.type()) {
    case QMetaType::Int: {
        return QString("i%1e").arg(variant.toInt());
    }
    case QMetaType::QString: {
        return QString("%1:%2").arg(variant.toString().length()).arg(variant.toString());
    }
    case QMetaType::QVariantList: {  // List
        QString strBuf;
        for(int i = 0; i<variant.toList().size(); ++i) {
            strBuf.append(encodeObject(variant.toList().at(i)));
        }
        return strBuf;
    }
    case QMetaType::QVariantHash: {  // Dictionary
        QString strBuf(dictChar);
        QHashIterator<QString,QVariant> pair(variant.toHash());
        while(pair.hasNext()) {
            pair.next();
            strBuf.append(QString("%1:%2").arg(pair.key().length()).arg(pair.key()));
            strBuf.append(encodeObject(pair.value()));
        }
        return strBuf.append(endChar);
    }
    default:
        return QString::QString();
    }
}
