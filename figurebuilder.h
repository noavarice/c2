#ifndef FIGUREBUILDER_H
#define FIGUREBUILDER_H

#include "figures.h"
#include <QVector>

QVector<Face> buildFigure(double r, double phi, int step);
QVector<Face> loadModel(std::string filename);

#endif // FIGUREBUILDER_H
