#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QKeyEvent>
#include <QResizeEvent>
#include <QElapsedTimer>

#include <QGraphicsScene>
#include <QGraphicsItem>

#include <QDebug>
#include "ssdparser.h"
#include "graph.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    Graph& getGraph();

private slots:
    void on_actionOpen_triggered();

    void on_actionExit_triggered();

private:
    Ui::MainWindow *ui;
    Graph mainGraph;
    QGraphicsScene scene;
    QDesktopWidget* desktopWidget;

    unsigned startW, startH;
    void keyPressEvent(QKeyEvent* e);
};



#endif // MAINWINDOW_H
