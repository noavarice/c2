#include "image.h"

#include <cmath>

Image::Image(int w, int h)
    : img(w, h, QImage::Format_RGB32)
    , xRot{0}
    , yRot{0}
    , zRot{0}
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

static void round(Vertex& v) {
    v.x = floor(v.x);
    v.y = floor(v.y);
    v.z = floor(v.z);
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

    round(t0);
    round(t1);
    round(t2);
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

Vertex rebase(const Vertex& v, const QVector<Vertex>& base)
{
    double x = base[0].x * v.x + base[1].x * v.y + base[2].x * v.z;
    double y = base[0].y * v.x + base[1].y * v.y + base[2].y * v.z;
    double z = base[0].z * v.x + base[1].z * v.y + base[2].z * v.z;
    return {x, y, z};
}

typedef QVector<QVector<double>> Matrix;
Matrix mul(const Matrix& a, const Matrix& b)
{
    Matrix res = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                res[i][j] += a[i][k] * b[k][j];
            }
        }
    }

    return res;
}

QVector<Vertex> Image::getBase()
{
    double pi2 = M_PI / 2;
    Matrix base_x {
        {1, 0, 0},
        {0, cos(xRot), cos(xRot + pi2)},
        {0, sin(xRot), sin(xRot + pi2)}
    };

    Matrix base_y {
        {sin(yRot + pi2), 0, sin(yRot)},
        {0, 1, 0},
        {cos(yRot + pi2), 0, cos(yRot)}
    };

    Matrix base_z {
        {cos(zRot), cos(zRot + pi2), 0},
        {sin(zRot), sin(zRot + pi2), 0},
        {0, 0, 1},
    };

    Matrix t = mul(base_x, base_y);
    Matrix base = mul(t, base_z);
    return {
        {base[0][0], base[0][1], base[0][2]},
        {base[1][0], base[1][1], base[1][2]},
        {base[2][0], base[2][1], base[2][2]}
    };
}

void Image::paint()
{
    img.fill(QColor(Qt::black));
    int* zbuffer = new int[img.width() * img.height()];
    memset(zbuffer, std::numeric_limits<int>::min(), img.width() * img.height() * sizeof(int));

    int w = img.width() / 4;
    int h = img.height() / 4;
    Vertex light_dir {0, 0, -1};
    QVector<Vertex> base = getBase();

    for (const Face& f : faces) {
        QVector<Vertex> face = f.getVector();
        Vertex screen[3];
        Vertex world[3];
        for (int j = 0; j < 3; j++) {
            Vertex v = rebase(face[j], base);
            world[j]  = v * 100;
            screen[j] = Vertex((v.x + 2.0) * w, (v.y + 2.0) * h, (v.z + 1.0) * 255/2);
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

void Image::setXRot(double a)
{
    xRot = a;
}

void Image::setYRot(double a)
{
    yRot = a;
}

void Image::setZRot(double a)
{
    zRot = a;
}
