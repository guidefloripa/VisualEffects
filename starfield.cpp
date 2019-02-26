#include "starfield.h"

#include <QColor>

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

    QVector<QRgb> palette;
    Star** stars;
    int n;
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
        d->stars[i]->x = (d->stars[i]->x + d->stars[i]->speed) % w;
    }
}

const QVector<QRgb>& StarField::palette() const
{
    return d->palette;
}
