#include "starfield.h"

#include <QColor>

typedef enum {
    K_L = 0,
    K_R = 1,
    K_DOWN = 2,
    K_UP = 3
} StarFieldKind;

class Star
{
public:
    Star(int w_max, int h_max) {
        reset(w_max, h_max);
    }

    void reset(int w_max, int h_max) {
        x = rand() % w_max;
        y = rand() % h_max;
        speed = (rand() % 3) + 1;
    }

    int x;
    int y;
    int speed;
};

class StarField::Priv
{
public:
    Priv (int w, int h, int num_stars): n(num_stars) {
        initKind(K_R);

        palette.push_back(QColor(5, 5, 5).rgb());
        palette.push_back(QColor(100, 100, 100).rgb());
        palette.push_back(QColor(170, 170, 170).rgb());
        palette.push_back(QColor(250, 250, 250).rgb());

        stars = new Star*[n];
        for (int i=0; i<n; i++)
            stars[i] = new Star(w, h);
    }

    ~Priv() {
        for (int i=0; i<n; i++)
            delete stars[i];
        delete stars;
    }

    void initKind(StarFieldKind k) {
        kind = k;
        switch (k) {
        case K_L:
            direction_x = -1;
            direction_y = 0;
            break;

        case K_R:
            direction_x = 1;
            direction_y = 0;
            break;

        case K_DOWN:
            direction_x = 0;
            direction_y = -1;
            break;

        case K_UP:
            direction_x = 0;
            direction_y = 1;
            break;
        }
    }

    QVector<QRgb> palette;
    Star** stars;
    StarFieldKind kind;
    int n;
    int direction_x;
    int direction_y;
};

StarField::StarField(int w, int h) : Effect (w, h)
{
    d = new Priv(w, h, 600);
}

StarField::~StarField()
{
    delete d;
}

void StarField::create()
{
}

void StarField::destroy()
{
}

void StarField::update()
{
    Effect::reset();
    for (int i=0; i<d->n; i++) {
        setValue(d->stars[i]->x, d->stars[i]->y, static_cast<uchar>(d->stars[i]->speed));
        if (d->direction_x != 0)
            d->stars[i]->x = (w + d->stars[i]->x + d->direction_x*d->stars[i]->speed) % w;
        if (d->direction_y != 0)
            d->stars[i]->y = (h + d->stars[i]->y + d->direction_y*d->stars[i]->speed) % h;
    }
}

int StarField::defaultRefreshRate()
{
    return 30;
}

const QVector<QRgb>& StarField::palette() const
{
    return d->palette;
}

QPair<int, QVector<QString>> StarField::fxKindList() const
{
    QVector<QString> v;

    v.append("Direction: Left");
    v.append("Direction: Right");
    v.append("Direction: Down");
    v.append("Direction: Up");

    return QPair<int, QVector<QString>>(d->kind, v);
}

void StarField::setFxKind(int kind)
{
    switch (kind) {
        case K_L:
        case K_R:
        case K_DOWN:
        case K_UP:
            d->initKind(StarFieldKind(kind));
            break;
    }
}
