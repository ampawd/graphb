#ifndef GRAPH_H
#define GRAPH_H

#define EPS 1e-6

#include <QVector>
#include <QMessageBox>
#include <QLabel>
#include <QMainWindow>
#include <QDebug>

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsLineItem>

class Graph
{
    public:
        Graph(QGraphicsScene* scene=nullptr, QMainWindow* mainWin = nullptr);

        void loadGraph(const QVector<QPointF>&);
        void loadCoorinateSystem();
        void setScene(QGraphicsScene*);
        void setMainWin(QMainWindow*);
        void setYRanges(std::pair<qreal, qreal>&);
        void initTransformations();

        qreal mapTo(qreal value, qreal a, qreal b, qreal c, qreal d);   //  maps 'value' from [a, b] to [c, d]

    private:
        QGraphicsScene* scene;
        QWidget* mainWin;
        unsigned coordStep;
        std::pair<qreal, qreal> yRanges;
        QTransform m1, m2, bottomLeftToTopLeftTransform;
};

#endif // GRAPH_H
