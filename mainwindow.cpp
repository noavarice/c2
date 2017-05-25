#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QLabel>
#include <QTimer>
#include <QSlider>
#include <QSpinBox>
#include <cmath>
#include "image.h"
#include "figurebuilder.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , img(800, 800)
    , height{1.0}
    , radius{0.3}
    , sidesCount{3}
{
    ui->setupUi(this);
    build();
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

void MainWindow::build()
{
    img.setFaces(buildFigure(height, sidesCount, radius));
    draw();
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

void MainWindow::on_doubleSpinBoxHeight_valueChanged(double newHeight)
{
    height = newHeight;
    build();
}

void MainWindow::on_doubleSpinBoxRadius_valueChanged(double newRadius)
{
    radius = newRadius;
    build();
}

void MainWindow::on_spinBoxSidesCount_valueChanged(int newSidesCount)
{
    sidesCount = newSidesCount;
    build();
}
