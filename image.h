#ifndef IMAGE_H
#define IMAGE_H

#include <QImage>
#include <vector>
#include "figures.h"

class Image
{
public:
    Image(int w, int h);
    void setFaces(const std::vector<Face>& faces);
    void setVertices(const std::vector<Vertex>& vertices);
    void paint();
    const QImage& getQImage() const;

private:
    QImage img;
    std::vector<Vertex> vertices;
    std::vector<Face> faces;
};

#endif // IMAGE_H
