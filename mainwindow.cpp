#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QLabel>
#include <QTimer>
#include <cmath>
#include "image.h"
#include "figurebuilder.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , img(800, 800)
{
    ui->setupUi(this);
    double r = 0.5;
    double phi = 20;
    int step = 1;
    QVector<Face> faces = buildFigure(r, phi, step);
    img.setFaces(faces);
    draw();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::draw()
{
    img.paint();
    pixmap = QPixmap::fromImage(img.getQImage());
    ui->image->setPixmap(pixmap);
}

#define ROTATE(COMP) \
void MainWindow::on_s##COMP##Rot_sliderMoved(int position) \
{ \
    position -= 180; \
    img.set##COMP##Rot(position * 2 * M_PI / 360); \
    draw(); \
}

ROTATE(X)
ROTATE(Y)
ROTATE(Z)

#define LIGHT(X, xo) \
void MainWindow::on_sbLight##X##_valueChanged(int val) \
{ \
    Vertex v = img.getLight(); \
    v.xo = val; \
    img.setLight(v); \
    draw(); \
}

LIGHT(X, x)
LIGHT(Y, y)
LIGHT(Z, z)
