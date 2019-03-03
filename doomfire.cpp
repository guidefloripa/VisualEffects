#include "doomfire.h"

#include <QColor>

typedef enum {
    K_WIND_L3 = 0,
    K_WIND_L2 = 1,
    K_WIND_L1 = 2,
    K_NO_WIND = 3,
    K_WIND_R1 = 4,
    K_WIND_R2 = 5,
    K_WIND_R3 = 6,
} WindKind;

class DoomFire::Priv
{
public:
    Priv () {
        initKind(K_WIND_R1);

        palette.push_back(QColor(7, 7, 7).rgb());
        palette.push_back(QColor(31, 7, 7).rgb());
        palette.push_back(QColor(47, 15, 7).rgb());
        palette.push_back(QColor(71, 15, 7).rgb());
        palette.push_back(QColor(87, 23, 7).rgb());
        palette.push_back(QColor(103, 31, 7).rgb());
        palette.push_back(QColor(119, 31, 7).rgb());
        palette.push_back(QColor(143, 39, 7).rgb());
        palette.push_back(QColor(159, 47, 7).rgb());
        palette.push_back(QColor(175, 63, 7).rgb());
        palette.push_back(QColor(191, 71, 7).rgb());
        palette.push_back(QColor(199, 71, 7).rgb());
        palette.push_back(QColor(223, 79, 7).rgb());
        palette.push_back(QColor(223, 87, 7).rgb());
        palette.push_back(QColor(223, 87, 7).rgb());
        palette.push_back(QColor(215, 95, 7).rgb());
        palette.push_back(QColor(215, 95, 7).rgb());
        palette.push_back(QColor(215, 103, 15).rgb());
        palette.push_back(QColor(207, 111, 15).rgb());
        palette.push_back(QColor(207, 119, 15).rgb());
        palette.push_back(QColor(207, 127, 15).rgb());
        palette.push_back(QColor(207, 135, 23).rgb());
        palette.push_back(QColor(199, 135, 23).rgb());
        palette.push_back(QColor(199, 143, 23).rgb());
        palette.push_back(QColor(199, 151, 31).rgb());
        palette.push_back(QColor(191, 159, 31).rgb());
        palette.push_back(QColor(191, 159, 31).rgb());
        palette.push_back(QColor(191, 167, 39).rgb());
        palette.push_back(QColor(191, 167, 39).rgb());
        palette.push_back(QColor(191, 175, 47).rgb());
        palette.push_back(QColor(183, 175, 47).rgb());
        palette.push_back(QColor(183, 183, 47).rgb());
        palette.push_back(QColor(183, 183, 55).rgb());
        palette.push_back(QColor(207, 207, 111).rgb());
        palette.push_back(QColor(223, 223, 159).rgb());
        palette.push_back(QColor(239, 239, 199).rgb());
        palette.push_back(QColor(255, 255, 255).rgb());
    }

    void initKind(WindKind k) {
        kind = k;

        switch (k) {
        case K_WIND_L3: windSpeed = -3; break;
        case K_WIND_L2: windSpeed = -2; break;
        case K_WIND_L1: windSpeed = -1; break;
        case K_NO_WIND: windSpeed = 0; break;
        case K_WIND_R1: windSpeed = 1; break;
        case K_WIND_R2: windSpeed = 2; break;
        case K_WIND_R3: windSpeed = 3; break;
        }
    }

    QVector<QRgb> palette;
    WindKind kind;
    int windSpeed;
};

DoomFire::DoomFire(int w, int h) : Effect (w, h)
{
    d = new Priv();

    reset();
    create();
}

DoomFire::~DoomFire()
{
    delete d;
}

void DoomFire::create()
{
    for (int i=0; i<w; i++)
        setValue(i, h-1, 36);
}

void DoomFire::destroy()
{
    for (int i=0; i<w; i++)
        setValue(i, h-1, 0);
}

void DoomFire::update()
{
    for (int j=0; j<(h-1); j++) {
        for (int i=0; i<w; i++) {
            int decay = rand() % 3;
            int new_value = getValue(i, j+1) - (decay > 1 ? 1 : 0);
            if (new_value < 0)
                new_value = 0;

            // change pixel direction
            int new_i = i + (rand()%3 -1);

            // lateral effect (wind)
            if (d->windSpeed > 0 && rand()%2) {
                new_i += d->windSpeed;
            }
            else if (d->windSpeed < 0 && rand()%2) {
                new_i += d->windSpeed;
            }

            new_i = (new_i % w);

            setValue(new_i, j, static_cast<uchar>(new_value));
        }
    }
}

int DoomFire::defaultRefreshRate()
{
    return 30;
}

const QVector<QRgb>& DoomFire::palette() const
{
    return d->palette;
}

QPair<int, QVector<QString>> DoomFire::fxKindList() const
{
    QVector<QString> v;

    v.append("Wind: -3");
    v.append("Wind: -2");
    v.append("Wind: -1");
    v.append("Wind: No Wind");
    v.append("Wind: 1");
    v.append("Wind: 2");
    v.append("Wind: 3");

    return QPair<int, QVector<QString>>(d->kind, v);
}

void DoomFire::setFxKind(int kind)
{
    switch (kind) {
        case K_WIND_L3:
        case K_WIND_L2:
        case K_WIND_L1:
        case K_NO_WIND:
        case K_WIND_R1:
        case K_WIND_R2:
        case K_WIND_R3:
            d->initKind(WindKind(kind));
            break;
    }
}
