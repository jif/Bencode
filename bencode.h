#ifndef BENCODE_H
#define BENCODE_H

#include <QObject>

class Bencode : public QObject
{
    Q_OBJECT
public:
    explicit Bencode(QObject *parent = 0);
    bool fromBEncodedString(const QString &bEncodedString);

signals:

public slots:

private:
    void parseObject();
    void parseInteger();
    void parseString();
    void parseList();
    void parseDictionary();
    void parseKeyValue();

    int pos;
    QString bEncoded;

};

#endif // BENCODE_H
