#ifndef RECURSIONTREE_H
#define RECURSIONTREE_H

#include "effect.h"

class RecursionTree : public Effect
{
public:
    RecursionTree(int w, int h);
    ~RecursionTree();

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

#endif // RECURSIONTREE_H
