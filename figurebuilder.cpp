#include "figurebuilder.h"

#include <cmath>
#include <fstream>

void addFace(QVector<Face>& faces, Face f)
{
    faces.push_back(f);
    faces.push_back({f.a, f.c, f.b});
}

QVector<Face> buildFigure(double height, int sidesCount, double radius)
{
    const Vertex start{0.0, 0.0, 0.0};
    const Vertex top{0.0, 0.0, height};
    double angleStep = M_PI * 2 / sidesCount;
    double currentAngle = 0.0;
    Vertex currentVertex{radius, 0.0, 0.0};
    QVector<Face> faces;
    for (int i = 0; i < sidesCount; ++i) {
        currentAngle += angleStep;
        Vertex nextVertex{
            cos(currentAngle) * radius,
            sin(currentAngle) * radius,
            0.0
        };
        addFace(faces, {start, currentVertex, nextVertex});
        addFace(faces, {top, currentVertex, nextVertex});
        currentVertex = nextVertex;
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
