#include "figures.h"

Vertex::Vertex(double x, double y, double z)
    : x{x}
    , y{y}
    , z{z}
{
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
