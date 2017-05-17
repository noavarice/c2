#include "image.h"

void line(QImage& image, int x0, int y0, int x1, int y1, QColor color) {
    bool steep = false;
    if (std::abs(x0 - x1) < std::abs(y0 - y1)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }

    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    int dx = x1 - x0;
    int dy = y1 - y0;
    int derror2 = std::abs(dy) * 2;
    int error2 = 0;
    int y = y0;
    for (int x = x0; x <= x1; x++) {
        if (steep) {
            image.setPixelColor(y, x, color);
        } else {
            image.setPixelColor(x, y, color);
        }

        error2 += derror2;
        if (error2 > dx) {
            y += (y1 > y0 ? 1 : -1);
            error2 -= dx*2;
        }
    }
}

Image::Image(int w, int h)
    : img(w, h, QImage::Format_RGB32)
{

}

void Image::setFaces(const std::vector<Face> &faces)
{
    this->faces = faces;
}

void Image::setVertices(const std::vector<Vertex> &vertices)
{
    this->vertices = vertices;
}

void drawFace(QImage& img, const Face& f, QColor color)
{
    int w = img.width() / 2;
    int h = img.height() / 2;
    QVector<Vertex> face = f.getVector();
    for (int i=0; i<3; i++) {
        Vertex v0 = face[i];
        Vertex v1 = face[(i + 1) % 3];
        int x0 = (v0.x + 1) * w;
        int y0 = (v0.y + 1) * h;
        int x1 = (v1.x + 1) * w;
        int y1 = (v1.y + 1) * h;
        line(img, x0, y0, x1, y1, color);
    }
}

void Image::paint()
{
    for (const Face& f : faces) {
        drawFace(img, f, QColor(Qt::white));
    }

    img = img.mirrored(false, true);
}

const QImage &Image::getQImage() const
{
    return img;
}
