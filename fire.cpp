#include "fire.h"

#include <QColor>

#define NCOLORS 256

typedef enum {
    K_TYPE1 = 0,
    K_TYPE2 = 1
} FireKind;

class Fire::Priv
{
public:
    Priv () : kind(K_TYPE1) {
        for (int i=0; i<NCOLORS; i++) {
            QColor color;
            color.setHsl(i/3, 255, qMin(255, i*2));
            palette.push_back(color.rgb());
        }
    }

    QVector<QRgb> palette;
    FireKind kind;
};

Fire::Fire(int w, int h) : Effect (w, h)
{
    d = new Priv();

    reset();
}

Fire::~Fire()
{
    delete d;
}

void Fire::create()
{
    for (int i=0; i<w; i++)
        setValue(i, h-1, static_cast<uchar>(abs(32768 + rand()) % 256));
}

void Fire::destroy()
{
    for (int i=0; i<w; i++)
        setValue(i, h-1, 0);
}

void Fire::update()
{
    uchar v1, v2, v3, v4;
    int new_value;

    create();
    for (int j=0; j<(h-1); j++) {
        for (int i=0; i<w; i++) {
            switch (d->kind) {
            case K_TYPE1:
                v2 = getValue(i, j+1);
                v1 = i>0 ? getValue(i-1, j+1) : v2;
                v3 = (i+1)<w ? getValue(i+1, j+1) : v2;
                v4 = (j+2)<h ? getValue(i, j+2) : v2;
                new_value = (v1+v2+v3+v4)*32/129;
                break;

            case K_TYPE2:
                v1 = i>0 ? getValue(i-1, j+1) : getValue(i, j+1);
                v2 = (j+2)<h ? getValue(i, j+2) : getValue(i, j+1);
                v3 = (i+1)<w ? getValue(i+1, j+1) : getValue(i, j+1);
                if ((j+3) < h)
                    v4 = getValue(i, j+3);
                else if ((j+2) < h)
                    v4 = getValue(i, j+2);
                else
                    v4 = getValue(i, j+1);
                new_value = (v1+v2+v3+v4)*64/257;
                break;
            }

            setValue(i, j, static_cast<uchar>(new_value));
        }
    }
}

int Fire::defaultRefreshRate()
{
    return 30;
}

const QVector<QRgb>& Fire::palette() const
{
    return d->palette;
}

QPair<int, QVector<QString>> Fire::fxKindList() const
{
    QVector<QString> v;

    v.append("Type 1");
    v.append("Type 2");

    return QPair<int, QVector<QString>>(d->kind, v);
}

void Fire::setFxKind(int kind)
{
    switch (kind) {
        case K_TYPE1:
        case K_TYPE2:
            reset();
            d->kind = FireKind(kind);
            break;
    }
}
