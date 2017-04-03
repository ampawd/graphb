#include "ssdparser.h"

SSDParser::SSDParser(const QString& ssdfileName) :
    ssdFile(ssdfileName),
    parsingStatus(false)
{
    yRanges.first = 1e+10;  //  initial minY
    yRanges.second = 1e-10; //  initial maxY

    if (!ssdFile.open(QIODevice::ReadOnly))
    {
        QMessageBox::information(0, "File opening error", "Can't open this ssd/rsd file");
        parsingStatus = false;
    }
}

SSDParser::~SSDParser()
{
    points.clear();
    headers.clear();
    if (ssdFile.isOpen())
    {
        ssdFile.close();
    }
}

void SSDParser::load()
{
    QElapsedTimer timer;

    yRanges.first = 1e+10;
    yRanges.second = 1e-10;

    timer.start();
    points.clear();
    headers.clear();

    QTextStream inputSream(&ssdFile);
    QString line;

    QStringRef x, y;

    while (!inputSream.atEnd())
    {
        line = inputSream.readLine();
        int size = line.size();
        if (size > 0)
        {
            if (! line.contains("#"))
            {
                const QVector<QStringRef>& lineParts = line.splitRef(' ', QString::SkipEmptyParts);
                //lineParts = line.split(' ', QString::SkipEmptyParts);  //  performance bottleneck here

                if (lineParts.size() != 2)
                {
                    QMessageBox::information(0, "Data parsing error", "Please load ssd/rsd files only");
                    parsingStatus = false;
                    points.clear();
                    headers.clear();
                    ssdFile.close();
                    return;
                }

                x = lineParts[0];
                y = lineParts[1];

                qreal yd = y.toDouble();
                qreal xd = x.toDouble();

                QPointF p;
                p.setX(xd);
                p.setY(yd);

                if (yd < yRanges.first)
                    yRanges.first = yd;      //  minY

                if (yd > yRanges.second)
                    yRanges.second = yd;     //  maxY

                points.push_back(p);

            } else
            {
                headers.push_back(line);
            }
        }
    }

    parsingStatus = true;
    qDebug() << timer.elapsed()/1000.0f;
}

const QVector<QPointF>& SSDParser::getPoints() const
{
    return points;
}

QVector<QString> &SSDParser::getHeaders()
{
    return headers;
}

bool SSDParser::parsed() const
{
    return parsingStatus;
}

std::pair<qreal, qreal> &SSDParser::getYRanges()
{
    return yRanges;
}

