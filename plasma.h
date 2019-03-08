#ifndef PLASMA_H
#define PLASMA_H

#include "effect.h"

class Plasma : public Effect
{
public:
    Plasma(int w, int h);
    ~Plasma();

    void create();
    void destroy();
    void update();
    int defaultRefreshRate();
    const QVector<QRgb>& palette() const;

    virtual QPair<int, QVector<QString>> fxKindList() const;
    virtual void setFxKind(int kind);

private:
    class Priv;
    Priv *d;
};

#endif // PLASMA_H
