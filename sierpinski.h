#ifndef SIERPINSKI_H
#define SIERPINSKI_H

#include "effect.h"

class Sierpinski : public Effect
{
public:
    Sierpinski(int w, int h);
    ~Sierpinski();

    void create();
    void destroy();
    void update();
    const QVector<QRgb>& palette() const;

    virtual bool paint(QPainter *painter) const;

private:
    class Priv;
    Priv *d;
};

#endif // SIERPINSKI_H
