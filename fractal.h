#ifndef FRACTAL_H
#define FRACTAL_H

#include "effect.h"

class Fractal : public Effect
{
public:
    Fractal(int w, int h);
    ~Fractal();

    void create();
    void destroy();
    void update();
    const QVector<QRgb>& palette() const;

    virtual bool paint(QPainter *painter) const;

private:
    class Priv;
    Priv *d;
};

#endif // FRACTAL_H
