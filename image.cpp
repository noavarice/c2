#include "image.h"

#include <cmath>

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

static void drawFace(QImage& img, Vertex t0, Vertex t1, Vertex t2, QColor color, int* zbuffer)
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

    double height = t2.y - t0.y;
    double d1 = t1.y - t0.y;
    double d2 = t2.y - t1.y;
    for (int i = 0; i < height; i++) {
        bool second_half = i > d1 || t1.y == t0.y;
        double segment_height = second_half ? d2 : d1;
        double alpha = static_cast<double>(i) / height;
        double num = second_half ? d1 : 0;
        double beta = static_cast<double>(i - num) / segment_height;
        Vertex a = t0 + (t2 - t0) * alpha;
        Vertex b = second_half ? t1 + (t2 - t1) * beta
                               : t0 + (t1 - t0) * beta;
        if (a.x > b.x) {
            std::swap(a, b);
        }

        for (int j = a.x; j <= b.x; j++) {
            bool eq = abs((int)(b.x - a.x)) < 1;
            float phi = eq ? 1.0 : static_cast<float>(j - a.x) / (b.x - a.x);
            Vertex p = a + (b - a) * phi;
            p.x = j;
            p.y = static_cast<int>(t0.y) + i;
            int idx = p.x + p.y * img.width();
            if (zbuffer[idx] < p.z) {
                zbuffer[idx] = p.z;
                img.setPixelColor(p.x, p.y, color);
            }
        }
    }
}

void Image::paint()
{
    int* zbuffer = new int[img.width() * img.height()];
    memset(zbuffer, std::numeric_limits<int>::min(), img.width() * img.height() * sizeof(int));

    int w = img.width() / 2;
    int h = img.height() / 2;
    Vertex light_dir = {0, 0, -1};

    for (const Face& f : faces) {
        QVector<Vertex> face = f.getVector();
        Vertex screen[3];
        Vertex world[3];
        for (int j = 0; j < 3; j++) {
            const Vertex& v = face[j];
            world[j]  = v * 100;
            screen[j] = Vertex((v.x + 1.0) * w, (v.y + 1.0) * h, (v.z + 1.0) * 255/2);
        }

        Vertex n = (world[2] - world[0]) ^ (world[1] - world[0]);
        n = n.normalize();
        float intensity = n * light_dir;
        if (intensity > 0) {
            QRgb rgb = qRgb(intensity*255, intensity*255, intensity*255);
            drawFace(img, screen[0], screen[1], screen[2], QColor(rgb), zbuffer);
        }
    }

    { // dump z-buffer (debugging purposes only)
        QImage zimage(img.width(), img.height(), QImage::Format_RGB32);
        for (int i = 0; i < img.width(); i++) {
            for (int j = 0; j < img.height(); j++) {
                int val = zbuffer[i+j*img.width()];
                QColor c(qRgb(val, val, val));
                zimage.setPixelColor(i, j, c);
            }
        }

        zimage = zimage.mirrored(false, true);
        zimage.save("zbuffer.jpg");
    }

    img = img.mirrored(false, true);
    img.save("out.jpg");
    delete[] zbuffer;
}

const QImage &Image::getQImage() const
{
    return img;
}
