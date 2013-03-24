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

bool Bencode::fromBEncodedString(const QString &bEncodedString) {
    bEncoded = bEncodedString;
    pos = 0;
    parseObject();
    return false;
}

void Bencode::parseObject() {
    if(bEncoded.at(pos) == intChar) {
        parseInteger();
    } else if(bEncoded.at(pos) == listChar) {
        parseList();
    } else if(bEncoded.at(pos) == dictChar) {
        parseDictionary();
    } else if(bEncoded.at(pos).isNumber()) {
        parseString();
    } else {
        return;
    }
    if(pos < bEncoded.size())
        parseObject();
}

void Bencode::parseInteger() {
    qDebug() << "Parsing an Integer";

    if(bEncoded.at(pos) != intChar) {
        qDebug() << "No leading i for integer";
        return;
    }
    pos++;
    QString buf;
    if(bEncoded.at(pos).isNumber() || bEncoded.at(pos) == negChar) {
        buf.append(bEncoded.at(pos));
        pos++;
    }
    while(bEncoded.at(pos).isNumber()) {
        buf.append(bEncoded.at(pos));
        pos++;
    }
    if(!bEncoded.at(pos).unicode() == 'e') {
        qDebug() << "No training e for integer";
        return;
    }
    pos++;
    qDebug("Integer: %i", buf.toInt());
}

void Bencode::parseString() {
    qDebug() << "Parsing a string";
    QString len;
    while(bEncoded.at(pos).isNumber()) {
        len.append(bEncoded.at(pos));
        pos++;
    }
    if(bEncoded.at(pos) != strDelimChar) {
        qDebug() << "missing the : for string";
        return;
    }
    pos++;
    QStringRef buf(&bEncoded,pos,len.toInt());
    pos=pos+len.toInt();
    qDebug("String: %s", qPrintable(buf.toLocal8Bit()));
}

void Bencode::parseList() {

}

void Bencode::parseDictionary() {

}

void Bencode::parseKeyValue() {

}
