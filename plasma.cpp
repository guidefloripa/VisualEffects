#include "plasma.h"

#include <math.h>
#include <QColor>

#define PALETTE_COLORS 256

typedef enum {
    K_VERTICAL = 0,
    K_CIRCULAR = 1,
    K_PLASMA_MONO = 2,
    K_PLASMA_COLOR = 3
} PlasmaKind;

class Plasma::Priv
{
public:
    Priv () : kind(K_VERTICAL) {
        for (int i=0; i<PALETTE_COLORS; i++) {
            QColor color;
            int c = i*2;
            if (c >= PALETTE_COLORS)
                c = (2*PALETTE_COLORS) - (i*2) - 2;

            color.setHsv(c, 255, 255);
            paletteColor.push_back(color.rgb());
            paletteMonoSin.push_back(QColor(c, c, c).rgb());
        }
    }

    QVector<QRgb> paletteColor;
    QVector<QRgb> paletteMonoSin;
    PlasmaKind kind;
};

Plasma::Plasma(int w, int h) : Effect (w, h)
{
    d = new Priv();

    reset();
    create();
}

Plasma::~Plasma()
{
    delete d;
}

void Plasma::create()
{
    switch (d->kind) {
    case K_VERTICAL:
        for (int j=0; j<h; j++) {
            for (int i=0; i<w; i++) {
                uchar color = static_cast<uchar>(128.0 + (128.0 * sin(i / 8.0)));
                setValue(i, j, color);
            }
        }
        break;

    case K_CIRCULAR:
        for (int j=0; j<h; j++) {
            for (int i=0; i<w; i++) {
                uchar color = static_cast<uchar>(128.0 + (128.0 * sin(sqrt((i - w / 2.0) * (i - w / 2.0) + (j - h / 2.0) * (j - h / 2.0)) / 8.0)));
                setValue(i, j, color);
            }
        }
        break;

    case K_PLASMA_MONO:
    case K_PLASMA_COLOR:
        for (int j=0; j<h; j++) {
            for (int i=0; i<w; i++) {
                uchar color = static_cast<uchar>((128.0 + (128.0 * sin(i / 16.0)) + 128.0 + (128.0 * sin(j / 16.0)))/2);
                setValue(i, j, color);
            }
        }
        break;
    }
}

void Plasma::destroy()
{
}

void Plasma::update()
{
    switch (d->kind) {
    case K_VERTICAL:
    case K_CIRCULAR:
    case K_PLASMA_MONO:
    case K_PLASMA_COLOR:
        for (int j=0; j<h; j++) {
            for (int i=0; i<w; i++) {
                uchar color = getValue(i, j);
                color = static_cast<uchar>((color + 1) % PALETTE_COLORS);
                setValue(i, j, color);
            }
        }
        break;
    }
}

int Plasma::defaultRefreshRate()
{
    return 30;
}

const QVector<QRgb>& Plasma::palette() const
{
    switch (d->kind) {
    case K_VERTICAL:
    case K_CIRCULAR:
    case K_PLASMA_MONO:
        return d->paletteMonoSin;

    case K_PLASMA_COLOR:
        return d->paletteColor;
    }
}

QPair<int, QVector<QString>> Plasma::fxKindList() const
{
    QVector<QString> v;

    v.append("Vertical");
    v.append("Circular");
    v.append("Plasma Mono");
    v.append("Plasma Color");

    return QPair<int, QVector<QString>>(d->kind, v);
}

void Plasma::setFxKind(int kind)
{
    switch (kind) {
    case K_VERTICAL:
    case K_CIRCULAR:
    case K_PLASMA_MONO:
    case K_PLASMA_COLOR:
        d->kind = PlasmaKind(kind);
        create();
    }
}
