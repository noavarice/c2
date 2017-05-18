#include "figures.h"

#include <cmath>

Vertex::Vertex(double x, double y, double z)
    : x{x}
    , y{y}
    , z{z}
{
}

Vertex Vertex::operator+(const Vertex& p) const
{
    return {p.x + x, p.y + y, p.z + z};
}

Vertex Vertex::operator-(const Vertex& p) const
{
    return {x - p.x, y - p.y, z - p.z};
}

Vertex Vertex::operator*(double val) const
{
    return {x * val, y * val, z * val};
}

double Vertex::operator*(const Vertex& p) const
{
    return p.x * x + p.y * y + p.z * z;
}

Vertex Vertex::operator^(const Vertex& p) const
{
    double xn = y * p.z - z * p.y;
    double yn = z * p.x - x * p.z;
    double zn = x * p.y - y * p.x;
    return { xn, yn, zn };
}

double Vertex::length() const
{
    return sqrt(x*x + y*y + z*z);
}

Vertex Vertex::normalize() const
{
    return *this * (1 / length());
}

Face::Face(Vertex a, Vertex b, Vertex c)
    : a{a}
    , b{b}
    , c{c}
{

}

QVector<Vertex> Face::getVector() const
{
    return { a, b, c };
}

Point::Point(int x, int y)
    : x{x}
    , y{y}
{
}

Point Point::operator+(Point p) const
{
    return { x + p.x, y + p.y };
}

Point Point::operator-(Point p) const
{
    return { x - p.x, y - p.y };
}

Point Point::operator*(double val) const
{
    return { static_cast<int>(x * val),
             static_cast<int>(y * val) };
}
