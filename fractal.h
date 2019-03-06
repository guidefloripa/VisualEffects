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
    int defaultRefreshRate();
    const QVector<QRgb>& palette() const;

    virtual bool paint(QPainter *painter) const;

    virtual QPair<int, QVector<QString>> fxKindList() const;
    virtual void setFxKind(int kind);

    virtual bool keyPressed(FxKey key);

private:
    class Priv;
    Priv *d;
};

#endif // FRACTAL_H
