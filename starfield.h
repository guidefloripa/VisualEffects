#ifndef STARFIELD_H
#define STARFIELD_H

#include "effect.h"

class StarField : public Effect
{
public:
    StarField(int w, int h);
    ~StarField();

    void create();
    void destroy();
    void update();
    int defaultRefreshRate();
    const QVector<QRgb>& palette() const;

private:
    class Priv;
    Priv *d;
};

#endif // STARFIELD_H
