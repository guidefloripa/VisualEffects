#include "tunnel.h"

#include <math.h>
#include <QColor>

// code: https://lodev.org/cgtutor/tunnel.html

static const int textureLength = 256;

class Tunnel::Priv
{
public:
    Priv(int _w, int _h) : w(_w), h(_h), elapsed(0) {

        palette.push_back(qRgb(0x07,0x07,0x07));
        palette.push_back(qRgb(0x1F,0x07,0x07));
        palette.push_back(qRgb(0x2F,0x0F,0x07));
        palette.push_back(qRgb(0x47,0x0F,0x07));
        palette.push_back(qRgb(0x57,0x17,0x07));
        palette.push_back(qRgb(0x67,0x1F,0x07));
        palette.push_back(qRgb(0x77,0x1F,0x07));
        palette.push_back(qRgb(0x8F,0x27,0x07));
        palette.push_back(qRgb(0x9F,0x2F,0x07));
        palette.push_back(qRgb(0xAF,0x3F,0x07));
        palette.push_back(qRgb(0xBF,0x47,0x07));
        palette.push_back(qRgb(0xC7,0x47,0x07));
        palette.push_back(qRgb(0xDF,0x4F,0x07));
        palette.push_back(qRgb(0xDF,0x57,0x07));
        palette.push_back(qRgb(0xDF,0x57,0x07));
        palette.push_back(qRgb(0xD7,0x5F,0x07));
        palette.push_back(qRgb(0xD7,0x5F,0x07));
        palette.push_back(qRgb(0xD7,0x67,0x0F));
        palette.push_back(qRgb(0xCF,0x6F,0x0F));
        palette.push_back(qRgb(0xCF,0x77,0x0F));
        palette.push_back(qRgb(0xCF,0x7F,0x0F));
        palette.push_back(qRgb(0xCF,0x87,0x17));
        palette.push_back(qRgb(0xC7,0x87,0x17));
        palette.push_back(qRgb(0xC7,0x8F,0x17));
        palette.push_back(qRgb(0xC7,0x97,0x1F));
        palette.push_back(qRgb(0xBF,0x9F,0x1F));
        palette.push_back(qRgb(0xBF,0x9F,0x1F));
        palette.push_back(qRgb(0xBF,0xA7,0x27));
        palette.push_back(qRgb(0xBF,0xA7,0x27));
        palette.push_back(qRgb(0xBF,0xAF,0x2F));
        palette.push_back(qRgb(0xB7,0xAF,0x2F));
        palette.push_back(qRgb(0xB7,0xB7,0x2F));
        palette.push_back(qRgb(0xB7,0xB7,0x37));
        palette.push_back(qRgb(0xCF,0xCF,0x6F));
        palette.push_back(qRgb(0xDF,0xDF,0x9F));
        palette.push_back(qRgb(0xEF,0xEF,0xC7));
        palette.push_back(qRgb(0xFF,0xFF,0xFF));

        for (int i=0; i<textureLength; i++) {
            for (int j=0; j<textureLength; j++) {
                texture[i][j] = (i^j) % 37;
            }
        }

        distance = new int*[w];
        angle = new unsigned int*[w];
        for (int i=0; i<w; i++) {
            distance[i] = new int[h];
            angle[i] = new unsigned int[h];
        }

        init();
    }

    ~Priv() {
        for (int i=0; i<w; i++) {
            delete distance[i];
            delete angle[i];
        }
        delete distance;
        delete angle;
    }

    void init() {
        const double ratio = 16.0;

        for (int i=0; i<w; i++) {
            for (int j=0; j<h; j++) {
                distance[i][j] = int(ratio * textureLength / sqrt((i-w/2.0)*(i-w/2.0) + (j-h/2.0)*(j-h/2.0))) % textureLength;
                angle[i][j] = static_cast<unsigned int>((0.5 * textureLength * atan2(i-w/2.0, j-h/2.0) / 3.1416));
            }
        }
    }

    int w;
    int h;
    int elapsed;

    int** distance;
    unsigned int** angle;
    uchar texture[textureLength][textureLength];

    QVector<QRgb> palette;
};

Tunnel::Tunnel(int w, int h) : Effect (w, h)
{
    d = new Priv(w, h);
}

Tunnel::~Tunnel()
{
    delete d;
}

void Tunnel::create()
{
}

void Tunnel::destroy()
{
}

void Tunnel::update()
{
    int xShift = (textureLength * d->elapsed) / 100;
    int yShift = (textureLength * d->elapsed) / 200;

    for (int j=0; j<h; j++) {
        for (int i=0; i<w; i++) {
            uchar value = d->texture[(d->distance[i][j] + xShift) % textureLength][(d->angle[i][j] + static_cast<unsigned int>(yShift)) % textureLength];
            setValue(i, j, value);
        }
    }

    d->elapsed++;
}

int Tunnel::defaultRefreshRate()
{
    return 30;
}

const QVector<QRgb>& Tunnel::palette() const
{
    return d->palette;
}
