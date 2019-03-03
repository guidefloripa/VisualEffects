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
    int defaultRefreshRate();
    const QVector<QRgb>& palette() const;

    virtual bool paint(QPainter *painter) const;

    virtual QPair<int, QVector<QString>> fxKindList() const;
    virtual void setFxKind(int kind);

private:
    class Priv;
    Priv *d;
};

#endif // SIERPINSKI_H
