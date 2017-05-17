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

static void drawFace(QImage& img, Point t0, Point t1, Point t2, QColor color)
{
    if (t0.y == t1.y && t0.y == t2.y) {
        return;
    }

    if (t0.y > t1.y) {
        std::swap(t0, t1);
    }
    if (t0.y > t2.y) {
        std::swap(t0, t2);
    }
    if (t1.y > t2.y) {
        std::swap(t1, t2);
    }

    int height = t2.y - t0.y;
    int d1 = t1.y - t0.y;
    int d2 = t2.y - t1.y;
    for (int i = 0; i < height; i++) {
        bool second_half = i > d1 || t1.y == t0.y;
        int segment_height = second_half ? d2 : d1;
        float alpha = static_cast<float>(i) / height;
        float num = second_half ? d1 : 0;
        float beta = static_cast<float>(i - num) / segment_height;
        Point a = t0 + (t2 - t0) * alpha;
        Point b = second_half ? t1 + (t2 - t1) * beta
                              : t0 + (t1 - t0) * beta;
        if (a.x > b.x) {
            std::swap(a, b);
        }

        for (int j = a.x; j <= b.x; j++) {
            img.setPixelColor(j, t0.y + i, color); // attention, due to int casts t0.y+i != A.y
        }
    }
}

void Image::paint()
{
    int w = img.width() / 2;
    int h = img.height() / 2;

    for (const Face& f : faces) {
        QVector<Point> points;
        for (const Vertex& v : f.getVector()) {
            int x = (v.x + 1.0) * w;
            int y = (v.y + 1.0) * h;
            points.push_back( {x, y} );
        }

        Qt::GlobalColor color = static_cast<Qt::GlobalColor>(rand() % Qt::yellow + 3);
        drawFace(img, points[0], points[1], points[2], QColor(color));
    }

    img = img.mirrored(false, true);
}

const QImage &Image::getQImage() const
{
    return img;
}
