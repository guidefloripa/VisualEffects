#ifndef TUNNEL_H
#define TUNNEL_H

#include "effect.h"

class Tunnel : public Effect
{
public:
    Tunnel(int w, int h);
    ~Tunnel();

    void create();
    void destroy();
    void update();
    const QVector<QRgb>& palette();

private:
    class Priv;
    Priv *d;
};

#endif // TUNNEL_H
