#include "graph.h"

Graph::Graph(QGraphicsScene* scene, QMainWindow *mainWin):
    scene(scene),
    mainWin(mainWin),
    coordStep(50)
{
    //  default ranges
    yRanges.first = 0.0;
    yRanges.second = 0.0;
    initTransformations();
}

qreal Graph::mapTo(qreal x, qreal a, qreal b, qreal c, qreal d)
{
    qreal denom = b - a;

    if (fabs(denom) > EPS)
    {
        return (x - a)*(d - c)/denom + c;
    }

    return (c + d)*0.5f;
}

void Graph::loadGraph(const QVector<QPointF>& points)
{
    qreal w = scene->width();
    qreal h = scene->height();

    if (w != 0 && h != 0)
    {
        unsigned size = points.size();
        unsigned step = size / w;
        step = step == 0 ? 1 : (step > w ? step / 2 : step);

        // minX = 0;
        qreal maxX = points[size - 1].x();
        qreal minY = yRanges.first;
        qreal maxY = yRanges.second;

        //  mapping ranges
        qreal toX1 = 0;  qreal toX2 = w;
        qreal toY1 = 50; qreal toY2 = h - 200;

        qreal x1 = mapTo(points[0].x(), 0, maxX, toX1, toX2);
        qreal x2 = mapTo(points[step].x(), 0, maxX, toX1, toX2);
        qreal y1 = mapTo(points[0].y(), minY, maxY, toY1, toY2);
        qreal y2 = mapTo(points[step].y(), minY, maxY, toY1, toY2);
        qreal rad = 1.5;

        for (unsigned i = step; i < size; i += step)
        {
            scene->addEllipse(x1-rad, h-y1, 2.0*rad, 2.0*rad,
                        QPen(Qt::black), QBrush(Qt::SolidPattern));

            scene->addLine(QLineF
            (
                x1, h-y1, x2, h-y2
            ),
                QPen(Qt::blue));

            x1 = x2;
            x2 = mapTo(points[i].x(), 0, maxX, toX1, toX2);
            y1 = y2;
            y2 = mapTo(points[i].y(), minY, maxY, toY1, toY2);
        }
    }
}

void Graph::loadCoorinateSystem()
{
    unsigned w = scene->width();
    unsigned h = scene->height();
    qreal minY = mapTo(yRanges.first, yRanges.first, yRanges.second, 50, h-200);
    qreal maxY = mapTo(yRanges.second, yRanges.first, yRanges.second, 50, h-200);

    QFont f1;
    f1.setBold(false);
    f1.setPointSize(8);

    for (unsigned i = coordStep; i < w; i += coordStep)
    {
        scene->addLine(QLineF(i, 0, i, h-coordStep) * bottomLeftToTopLeftTransform, QPen(QColor(200, 220, 220)));  //  vertical line
        qreal xMark = i / coordStep;
        scene->addText(QString::number(xMark) + ".0", f1)->setPos(QPointF(2*i, h-coordStep*0.5));
        scene->addLine(QLineF(2*i, 0, 2*i, 10)*bottomLeftToTopLeftTransform, QPen(Qt::black) );
    }

    for (unsigned i = coordStep; i < h - coordStep; i += coordStep)
    {
        scene->addLine(QLineF(0, i, w, i) * bottomLeftToTopLeftTransform, QPen(QColor(200, 220, 220)));  //  horizontal line
    }


    //  y ranges marks
    if (yRanges.first == 0.0 || yRanges.second == 0.0)
    {
        return;
    }

    QFont f2;
    f2.setBold(false);
    f2.setPointSize(11);

    qreal midMappedY = (yRanges.first + yRanges.second)*0.5f;
    qreal midY = (minY + maxY)*0.5f;

    scene->addText(QString::number(yRanges.second), f2)->setPos(QPointF(0, maxY)*bottomLeftToTopLeftTransform);
    scene->addLine(QLineF(0, maxY, 10, maxY)*bottomLeftToTopLeftTransform, QPen(Qt::black) );

    scene->addText(QString::number(midMappedY), f2)->setPos(QPointF(0, midY)*bottomLeftToTopLeftTransform);
    scene->addLine(QLineF(0, midY, 10, midY)*bottomLeftToTopLeftTransform, QPen(Qt::black) );

    scene->addText(QString::number(yRanges.first), f2)->setPos(QPointF(0, minY)*bottomLeftToTopLeftTransform);
    scene->addLine(QLineF(0, minY, 10, minY)*bottomLeftToTopLeftTransform, QPen(Qt::black) );
}

void Graph::setScene(QGraphicsScene* scene)
{
    this->scene = scene;
}

void Graph::setMainWin(QMainWindow *win)
{
    mainWin = win;
}

void Graph::setYRanges(std::pair<qreal, qreal> & newYRanges)
{
    yRanges = newYRanges;
}

void Graph::initTransformations()
{
    if (scene == nullptr)
        return;

    qreal h = scene->height();
    m1.translate(0, h);
    m2.scale(1, -1);
    bottomLeftToTopLeftTransform = m2 * m1;
}
