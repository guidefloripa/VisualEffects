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
    const QVector<QRgb>& palette() const;

private:
    class Priv;
    Priv *d;
};

#endif // STARFIELD_H
