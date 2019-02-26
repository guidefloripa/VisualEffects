#include "recursiontree.h"

#include <math.h>
#include <QColor>

class RecursionTree::Priv
{
public:
    Priv () : angle(0.2 * M_PI), shrink(1.8), maxRecursions(0) {
        palette.push_back(QColor(232, 232, 232).rgb());
        palette.push_back(QColor(128, 96, 64).rgb());
    }

    void recursion(QPainter *p, double posX, double posY, double dirX, double dirY, double size, int n)
    {
        double posX2, posY2;
        posX2 = posX + size * dirX;
        posY2 = posY + size * dirY;

        p->drawLine(int(posX), int(posY), int(posX2), int(posY2));

        if (n >= maxRecursions)
            return;

        double dirX2, dirY2, size2;
        size2 = size / shrink;

        dirX2 = cos(angle) * dirX + sin(angle) * dirY;
        dirY2 = -sin(angle) * dirX + cos(angle) * dirY;
        recursion(p, posX2, posY2, dirX2, dirY2, size2, n+1);

        dirX2 = cos(-angle) * dirX + sin(-angle) * dirY;
        dirY2 = -sin(-angle) * dirX + cos(-angle) * dirY;
        recursion(p, posX2, posY2, dirX2, dirY2, size2, n+1);
    }

    QVector<QRgb> palette;
    double angle;
    double shrink;
    int maxRecursions;
};

RecursionTree::RecursionTree(int w, int h) : Effect (w, h)
{
    d = new Priv();
}

RecursionTree::~RecursionTree()
{
    delete d;
}

void RecursionTree::create()
{
}

void RecursionTree::destroy()
{
}

void RecursionTree::update()
{
    d->maxRecursions = (d->maxRecursions + 1) % 9;
}

const QVector<QRgb>& RecursionTree::palette() const
{
    return d->palette;
}

bool RecursionTree::paint(QPainter *painter) const
{
    if (!painter)
        return false;

    painter->setClipping(false);
    painter->fillRect(0, 0, w, h, d->palette[0]);

    painter->setPen(d->palette[1]);
    if (d->maxRecursions > 0)
        d->recursion(painter, w/2, h-1, 0, -1, h/2.3, 1);

    return true;
}
