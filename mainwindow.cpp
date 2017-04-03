#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    desktopWidget(qApp->desktop())
{
    ui->setupUi(this);
    QString appIconPath = QApplication::applicationDirPath();
    appIconPath += "/appicon.ico";
    this->setWindowIcon(QIcon(appIconPath));

    this->setGeometry(
        QStyle::alignedRect(
            Qt::LeftToRight,
            Qt::AlignCenter,
            this->size(),
            desktopWidget->availableGeometry()
        )
    );

    startW = ui->graphicsView->width();
    startH = ui->graphicsView->height();

    //ui->graphicsView->setViewport(new QOpenGLWidget());           //  opengl rendering

    ui->graphicsView->setRenderHints(QPainter::Antialiasing);       //  antialiasing
    ui->graphicsView->setScene(&scene);
    scene.setSceneRect(0, 0, startW - 20, startH-10);

    mainGraph.setScene(&scene);
    mainGraph.setMainWin(this);
    mainGraph.initTransformations();

    ui->headersList->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

Graph &MainWindow::getGraph()
{
    return mainGraph;
}

void MainWindow::on_actionOpen_triggered()
{
    QString ssdfileName = QFileDialog::getOpenFileName(this,
          tr("Open ssd or rsd data file"), QApplication::applicationDirPath(), tr("(*.ssd *.rsd)"));


    SSDParser parser(ssdfileName);
    parser.load();

    if (!parser.parsed())
    {
        return;
    }

    QVector<QPointF> points = parser.getPoints();
    QStringList fileNameParts = ssdfileName.split('/');

    if (! points.empty())
    {
        scene.clear();
        if (! fileNameParts.empty())
        {
            ui->fileNameLabel->setText(fileNameParts.at(fileNameParts.size()-1));
        }

        ui->headersList->setVisible(true);
        ui->headersList->clear();
        //QStringList spacedHeaderParts = {"Single", "Filter", "Common", "Ref.osc", "Statistics"};

        QVector<QString> headers = parser.getHeaders();
        for (auto& h: headers)
        {
            QStringList headerParts = h.split(QRegExp("\\s+"), QString::SkipEmptyParts);
            QString header = "";
            for (int i = 1; i < headerParts.size(); ++i)
            {
                header += headerParts[i] + "   ";
            }

            QListWidgetItem* headerItem = new QListWidgetItem(header);
            headerItem->setSizeHint(QSize(300, 19));
            ui->headersList->addItem(headerItem);
        }

        mainGraph.setYRanges(parser.getYRanges());
        mainGraph.loadCoorinateSystem();
        mainGraph.loadGraph(parser.getPoints());

        //qDebug() << "Points are loaded";
    }
}

//  slots
void MainWindow::on_actionExit_triggered()
{
    qApp->exit();
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if ( (e->key() == Qt::Key_O)
         && QApplication::keyboardModifiers() && Qt::ControlModifier)
    {
        this->on_actionOpen_triggered();
    }
}


