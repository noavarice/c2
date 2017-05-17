#ifndef FIGURES_H
#define FIGURES_H

#include <QVector>

struct Vertex {
    Vertex(double x = 0, double y = 0, double z = 0);

    Vertex operator+(const Vertex& p) const;
    Vertex operator-(const Vertex& p) const;
    Vertex operator*(double val) const;
    double operator*(const Vertex& p) const;
    Vertex operator^(const Vertex& p) const;

    Vertex normalize() const;
    double length() const;

    double x, y, z;
};

struct Face {
    Face(Vertex a = {}, Vertex b = {}, Vertex c = {});
    QVector<Vertex> getVector() const;
    Vertex a, b, c;
};

struct Point {
    Point(int x = 0, int y = 0);
    Point operator+(Point p) const;
    Point operator-(Point p) const;
    Point operator*(double val) const;
    int x, y;
};

#endif // FIGURES_H
