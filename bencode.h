#ifndef BENCODE_H
#define BENCODE_H

#include <QObject>

class Bencode : public QObject
{
    Q_OBJECT
public:
    explicit Bencode(QObject *parent = 0);
    QList<QVariant> fromBEncodedString(const QString &bEncodedString);

signals:

public slots:

private:
    QVariant parseObject();
    int parseInteger();
    QString parseString();
    QList<QVariant> parseList();
    QHash<QString,QVariant> parseDictionary();
    QChar currentChar();

    int pos;
    QString bEncoded;
};

#endif // BENCODE_H
