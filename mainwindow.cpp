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
{
    ui->setupUi(this);
    ui->sQuality->setValue(ui->sQuality->maximum() - step + 1);
    ui->sbPhi->setValue(phi);
    ui->sbR->setValue(r);
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
    img.setFaces(buildFigure(r, phi, step));
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

void MainWindow::on_sQuality_sliderMoved(int position)
{
    step = ui->sQuality->maximum() - position + 1;
    build();
}

void MainWindow::on_sbR_valueChanged(double val)
{
    r = val;
    build();
}

void MainWindow::on_sbPhi_valueChanged(int val)
{
    phi = val;
    build();
}
