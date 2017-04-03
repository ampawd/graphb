#ifndef SSDPARSER_H
#define SSDPARSER_H

#include <QVector>
#include <QMessageBox>
#include <QFile>
#include <QIODevice>
#include <QTextStream>
#include <QElapsedTimer>
#include <QDebug>
#include <QObject>
#include <QThread>

class SSDParser : public QObject
{
    Q_OBJECT

public:
    SSDParser(const QString& ssdfileName);

    void load();

    const QVector<QPointF>& getPoints() const;

    QVector<QString>& getHeaders();

    bool parsed() const;

    virtual ~SSDParser();

    std::pair<qreal, qreal>& getYRanges();

private:

    QVector<QPointF> points;
    QVector<QString> headers;
    QFile ssdFile;
    bool parsingStatus;

    std::pair<qreal, qreal> yRanges;
};

#endif // SSDPARSER_H
