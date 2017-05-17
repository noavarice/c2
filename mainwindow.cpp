#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QLabel>
#include <fstream>
#include "image.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
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

    Image img(800, 800);
    img.setFaces(faces);
    img.setVertices(vertices);
    img.paint();
    pixmap = QPixmap::fromImage(img.getQImage());
    ui->image->setPixmap(pixmap);
}

MainWindow::~MainWindow()
{
    delete ui;
}
