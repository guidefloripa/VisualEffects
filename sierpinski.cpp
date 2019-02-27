#include "sierpinski.h"

#include <math.h>
#include <QColor>

// code: https://lodev.org/cgtutor/sierpinski.html

typedef enum {
    K_TRIANGLE,
    K_AND,
    K_RAND,
    K_RECT_RECURSION,
    K_CARPET,
    K_CARPET_TERNARY
} SierpinskiKind;

class Sierpinski::Priv
{
public:
    explicit Priv (int _w, int _h) : w(_w), h(_h), curMaxRecursions(0), kind(K_TRIANGLE) {
        palette.push_back(QColor(232, 232, 232).rgb());
        palette.push_back(QColor(48, 48, 48).rgb());
    }

    void _subTriangle(QPainter *p, int n, double x1, double y1, double x2, double y2, double x3, double y3)
    {
        // Draw the 3 sides as black lines
        p->drawLine(int(x1), int(y1), int(x2), int(y2));
        p->drawLine(int(x1), int(y1), int(x3), int(y3));
        p->drawLine(int(x2), int(y2), int(x3), int(y3));

        // Calls itself 3 times with new corners, but only if the current number of recursions is smaller than the maximum depth
        if (n < curMaxRecursions) {
            // Smaller triangle 1
            _subTriangle(
                        p, // This represents the painter
                        n+1, // Number of recursions for the next call increased with 1
                        (x1 + x2) / 2 + (x2 - x3) / 2, // x coordinate of first corner
                        (y1 + y2) / 2 + (y2 - y3) / 2, // y coordinate of first corner
                        (x1 + x2) / 2 + (x1 - x3) / 2, // x coordinate of second corner
                        (y1 + y2) / 2 + (y1 - y3) / 2, // y coordinate of second corner
                        (x1 + x2) / 2, // x coordinate of third corner
                        (y1 + y2) / 2  // y coordinate of third corner
                        );
            // Smaller triangle 2
            _subTriangle(
                        p, // This represents the painter
                        n+1, // Number of recursions for the next call increased with 1
                        (x3 + x2) / 2 + (x2 - x1) / 2, // x coordinate of first corner
                        (y3 + y2) / 2 + (y2 - y1) / 2, // y coordinate of first corner
                        (x3 + x2) / 2 + (x3 - x1) / 2, // x coordinate of second corner
                        (y3 + y2) / 2 + (y3 - y1) / 2, // y coordinate of second corner
                        (x3 + x2) / 2, // x coordinate of third corner
                        (y3 + y2) / 2  // y coordinate of third corner
                        );
            // Smaller triangle 3
            _subTriangle(
                        p, // This represents the painter
                        n+1, // Number of recursions for the next call increased with 1
                        (x1 + x3) / 2 + (x3 - x2) / 2, // x coordinate of first corner
                        (y1 + y3) / 2 + (y3 - y2) / 2, // y coordinate of first corner
                        (x1 + x3) / 2 + (x1 - x2) / 2, // x coordinate of second corner
                        (y1 + y3) / 2 + (y1 - y2) / 2, // y coordinate of second corner
                        (x1 + x3) / 2, // x coordinate of third corner
                        (y1 + y3) / 2  // y coordinate of third corner
                        );
        }
    }

    void drawSierpinski(QPainter *p, double x1, double y1, double x2, double y2, double x3, double y3)
    {
        // Draw the 3 sides of the triangle as black lines
        p->drawLine(int(x1), int(y1), int(x2), int(y2));
        p->drawLine(int(x1), int(y1), int(x3), int(y3));
        p->drawLine(int(x2), int(y2), int(x3), int(y3));

        // Call the recursive function that'll draw all the rest. The 3 corners of it are always the centers of sides, so they're averages
        _subTriangle(
                    p, // This represents the painter
                    1, // This represents the first recursion
                    (x1 + x2) / 2, // x coordinate of first corner
                    (y1 + y2) / 2, // y coordinate of first corner
                    (x1 + x3) / 2, // x coordinate of second corner
                    (y1 + y3) / 2, // y coordinate of second corner
                    (x2 + x3) / 2, // x coordinate of third corner
                    (y2 + y3) / 2  // y coordinate of third corner
                    );
    }

    void drawAnd(QPainter *p)
    {
        for (int i=0; i<w; i++) {
            for (int j=0; j<h; j++) {
                if (i & j)
                    p->drawPoint(i, j);
            }
        }
    }

    void drawRand(QPainter *p)
    {
        double ax = 10;
        double ay = h - 10;
        double bx = w - 10;
        double by = h - 10;
        double cx = w / 2;
        double cy = 10;

        double px = ax;
        double py = ay;
        int numSteps = curMaxRecursions * 5000;

        for(int n = 0; n < numSteps; n++) {
            p->drawPoint(int(px), int(py));

            switch(rand() % 3){
            case 0:
                px = (px + ax) / 2.0;
                py = (py + ay) / 2.0;
                break;
            case 1:
                px = (px + bx) / 2.0;
                py = (py + by) / 2.0;
                break;
            case 2:
                px = (px + cx) / 2.0;
                py = (py + cy) / 2.0;
                break;
            }
        }
    }

    void drawRectRecursion(QPainter *p, int n, int x1, int y1, int x2, int y2)
    {
        //draw white rectangle in the upper right part, thereby making a black L
        QRectF r;
        r.adjust((x1 + x2)/2, y1, x2, (y1 + y2)/2);
        p->fillRect(r, palette[0]);

        //call itself 3 times again, now for the 3 new rectangles in the L shape
        if (n < curMaxRecursions) {
            n++;
            drawRectRecursion(p, n, x1, y1, (x1 + x2)/2, (y1 + y2)/2);
            drawRectRecursion(p, n, x1, (y1 + y2)/2, (x1 + x2)/2, y2);
            drawRectRecursion(p, n, (x1 + x2)/2, (y1 + y2)/2, x2, y2);
        }
    }

    void drawCarpet(QPainter *p, int n, double x1, double y1, double w1, double h1)
    {
        // draw black rectangle with 1/3th the size in the center of the given coordinates
        QRect r;
        r.adjust(int(x1 + w1/3.0), int(y1 + h1/3.0), int(x1 + 2*w1/3.0), int(y1 + 2*h1/3.0));
        p->fillRect(r, palette[1]);

        // call itself 8 times again, now for the 8 new rectangles around the one that was just drawn
        if (n < curMaxRecursions) {
            double wrec = w1/3.0;
            double hrec = h1/3.0;

            if (wrec < 0.5 || hrec < 0.5)
                return;

            n++;
            drawCarpet(p, n, x1, y1, wrec, hrec);
            drawCarpet(p, n, x1 + w1/3.0, y1, wrec, hrec);
            drawCarpet(p, n, x1 + 2*w1/3.0, y1, wrec, hrec);
            drawCarpet(p, n, x1, y1 + h1/3.0, wrec, hrec);
            drawCarpet(p, n, x1 + 2*w1/3.0, y1 + h1/3.0, wrec, hrec);
            drawCarpet(p, n, x1, y1 + 2*h1/3.0, wrec, hrec);
            drawCarpet(p, n, x1 + w1/3.0, y1 + 2*h1/3.0, wrec, hrec);
            drawCarpet(p, n, x1 + 2*w1/3.0, y1 + 2*h1/3.0, wrec, hrec);
        }
    }

    bool _drawCarpetPoint(int i, int j) {
        int power = 1;
        while (power <= i && power <= j) {
            if (((i/power) % 3 == 1) && ((j/power) % 3 == 1))
                return false;
            power *= 3;
        }
        return true;
    }

    void drawCarpetTernary(QPainter *p) {
        for (int i=0; i<w; i++) {
            for (int j=0; j<h; j++) {
                if (_drawCarpetPoint(i, j))
                    p->drawPoint(i, j);
            }
        }
    }

    QVector<QRgb> palette;
    int w, h;
    int curMaxRecursions;
    SierpinskiKind kind;
};

Sierpinski::Sierpinski(int w, int h) : Effect (w, h)
{
    d = new Priv(w, h);
}

Sierpinski::~Sierpinski()
{
    delete d;
}

void Sierpinski::create()
{
}

void Sierpinski::destroy()
{
}

void Sierpinski::update()
{
    d->curMaxRecursions = (d->curMaxRecursions + 1) % 9;
}

const QVector<QRgb>& Sierpinski::palette() const
{
    return d->palette;
}

bool Sierpinski::paint(QPainter *painter) const
{
    if (!painter)
        return false;

    painter->setClipping(false);
    painter->fillRect(0, 0, w, h, d->palette[0]);
    painter->setPen(d->palette[1]);

    if (d->curMaxRecursions > 0) {
        switch (d->kind) {
        case K_TRIANGLE:
            d->drawSierpinski(painter, 10, h-10, w-10, h-10, w/2, 10);
            break;

        case K_AND:
            // invert colors
            painter->fillRect(0, 0, w, h, d->palette[1]);
            painter->setPen(d->palette[0]);

            d->drawAnd(painter);
            break;

        case K_RAND:
            d->drawRand(painter);
            break;

        case K_RECT_RECURSION:
            // invert colors
            painter->fillRect(0, 0, w, h, d->palette[1]);
            painter->setPen(d->palette[0]);

            d->drawRectRecursion(painter, 1, 0, 0, w-1, h-1);
            break;

        case K_CARPET: {
            // adjust w, h to next power of 3 (pixel division problem)
            int new_w, new_h;

            int current = 3;
            while (true) {
                if (w <= current) {
                    new_w = current;
                    break;
                }
                current *= 3;
            }

            current = 3;
            while (true) {
                if (h <= current) {
                    new_h = current;
                    break;
                }
                current *= 3;
            }

            d->drawCarpet(painter, 1, 0, 0, new_w-1, new_h-1);
            break;
        }

        case K_CARPET_TERNARY:
            // invert colors
            painter->fillRect(0, 0, w, h, d->palette[1]);
            painter->setPen(d->palette[0]);

            d->drawCarpetTernary(painter);
            break;
        }
    }

    return true;
}
