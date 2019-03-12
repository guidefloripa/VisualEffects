#ifndef FIRE_H
#define FIRE_H

#include "effect.h"

class Fire : public Effect
{
public:
    Fire(int w, int h);
    ~Fire();

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

#endif // FIRE_H
