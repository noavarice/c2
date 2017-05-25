#ifndef FIGUREBUILDER_H
#define FIGUREBUILDER_H

#include "figures.h"
#include <QVector>

QVector<Face> buildFigure(double height, int sidesCount, double radius);
QVector<Face> loadModel(std::string filename);

#endif // FIGUREBUILDER_H
