#include "figurebuilder.h"

#include <cmath>
#include <fstream>

void addFace(QVector<Face>& faces, Face f)
{
    faces.push_back(f);
    faces.push_back({f.a, f.c, f.b});
}

QVector<Face> buildFigure(double r, double phi, int step)
{
    QVector<QVector<Vertex>> points(180 / step);
    for (int i = 0; i < 180 / step; i++) {
        QVector<Vertex>& line = points[i];
        for (int j = 0; j < phi / step; j++) {
            double a = i * step * M_PI / 180;
            double b = j * step * M_PI / 180;
            Vertex v {
                r * sin(a) * cos(b),
                r * sin(a) * sin(b),
                r * cos(a)
            };
            line.push_back(v);
        }
    }

    QVector<Face> faces;
    for (int i = 0; i < points.size() - 1; i++) {
        for (int j = 0; j < points[0].size() - 1; j++) {
            addFace(faces, {
                points[i][j],
                points[i][j + 1],
                points[i + 1][j]
            });
            addFace(faces, {
                points[i + 1][j + 1],
                points[i][j + 1],
                points[i + 1][j]
            });
        }
    }

    return faces;
}

QVector<Face> loadModel(std::string filename)
{
    std::ifstream in(filename);
    std::string line;
    QVector<Vertex> vertices;
    QVector<Face> faces;
    while (in >> line) {
        if (line == "v") {
            Vertex v;
            in >> v.x >> v.y >> v.z;
            vertices.push_back(v);
        } else if (line == "f") {
            int a, b, c;
            in >> a >> line >> b >> line >> c >> line;
            faces.push_back({vertices[a - 1], vertices[b - 1], vertices[c - 1]});
        } else if (line == "vt" || line == "vn" || line == "g" || line == "s") {
            // skip
            std::getline(in, line);
        } else {
            std::getline(in, line);
        }
    }

    return faces;
}
