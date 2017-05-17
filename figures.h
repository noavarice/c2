#ifndef FIGURES_H
#define FIGURES_H

#include <QVector>

struct Vertex {
    Vertex(double x = 0, double y = 0, double z = 0);
    double x, y, z;
};

struct Face {
    Face(Vertex a = {}, Vertex b = {}, Vertex c = {});
    QVector<Vertex> getVector() const;
    Vertex a, b, c;
};

struct Point {
    Point operator+(Point p);
    Point operator-(Point p);
    Point operator*(double val);
    int x, y;
};

#endif // FIGURES_H
