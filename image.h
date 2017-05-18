#ifndef IMAGE_H
#define IMAGE_H

#include <QImage>
#include <vector>
#include "figures.h"

class Image
{
public:
    Image(int w, int h);
    ~Image();
    void setFaces(const std::vector<Face>& faces);
    void setVertices(const std::vector<Vertex>& vertices);
    void paint();
    const QImage& getQImage() const;

    void setXRot(double a);
    void setYRot(double a);
    void setZRot(double a);

    void setLight(const Vertex& p);
    const Vertex& getLight() const;

private:
    QVector<Vertex> getBase();

private:
    QImage img;
    std::vector<Vertex> vertices;
    std::vector<Face> faces;
    double xRot, yRot, zRot;
    Vertex light {0, 0, -1};
    int* zbuffer;
};

#endif // IMAGE_H
