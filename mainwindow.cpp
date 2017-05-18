#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QLabel>
#include <QTimer>
#include <fstream>
#include <cmath>
#include "image.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , img(800, 800)
{
    ui->setupUi(this);
    std::ifstream in("/home/diadlo/Pictures/african_head.obj");
    std::string line;
    Vertex v;
    Face f;
    int a, b, c;
    std::vector<Vertex> vertices;
    std::vector<Face> faces;
    while (in >> line) {
        if (line == "v") {
            in >> v.x >> v.y >> v.z;
            vertices.push_back(v);
        } else if (line == "f") {
            in >> a >> line >> b >> line >> c >> line;
            f = {vertices[a - 1], vertices[b - 1], vertices[c - 1]};
            faces.push_back(f);
        } else if (line == "vt" || line == "vn" || line == "g" || line == "s") {
            std::getline(in, line);
        } else {
            std::getline(in, line);
        }
    }

    img.setFaces(faces);
    img.setVertices(vertices);
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
