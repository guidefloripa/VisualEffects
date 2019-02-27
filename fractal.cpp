#include "fractal.h"

#include "sierpinski.h"

#include <math.h>
#include <QColor>

// code: https://lodev.org/cgtutor/juliamandelbrot.html

typedef enum {
    K_JULIA_SET,
    K_MANDELBROT
} FractalKind;

class Fractal::Priv
{
public:
    explicit Priv (int _w, int _h) : w(_w), h(_h), kind(K_JULIA_SET) {
    }

    void drawJuliaSet(QPainter *p) {
        double cRe, cIm;           //real and imaginary part of the constant c, determinate shape of the Julia Set
        double newRe, newIm, oldRe, oldIm;   //real and imaginary parts of new and old
        double zoom = 1.0, moveX = 0, moveY = 0; //you can change these to zoom and change position
        QColor color; //the RGB color value for the pixel
        int maxIterations = 300; //after how much iterations the function should stop

        //pick some values for the constant c, this determines the shape of the Julia Set
        cRe = -0.7;
        cIm = 0.27015;

        //loop through every pixel
        for (int y = 0; y < h; y++) {
            for (int x = 0; x < w; x++) {
                //calculate the initial real and imaginary part of z, based on the pixel location and zoom and position values
                newRe = 1.5 * (x - w / 2) / (0.5 * zoom * w) + moveX;
                newIm = (y - h / 2) / (0.5 * zoom * h) + moveY;
                //i will represent the number of iterations
                int i;
                //start the iteration process
                for (i = 0; i < maxIterations; i++) {
                    //remember value of previous iteration
                    oldRe = newRe;
                    oldIm = newIm;
                    //the actual iteration, the real and imaginary part are calculated
                    newRe = oldRe * oldRe - oldIm * oldIm + cRe;
                    newIm = 2 * oldRe * oldIm + cIm;
                    //if the point is outside the circle with radius 2: stop
                    if ((newRe * newRe + newIm * newIm) > 4) break;
                }
                //use color model conversion to get rainbow palette, make brightness black if maxIterations reached
                color.setHsv(i % 256, 255, 255 * (i < maxIterations));
                //draw the pixel
                p->setPen(color);
                p->drawPoint(x, y);
            }
        }
    }

    void drawMandelbrot(QPainter *p) {
        //each iteration, it calculates: newz = oldz*oldz + p, where p is the current pixel, and oldz stars at the origin
        double pr, pi;           //real and imaginary part of the pixel p
        double newRe, newIm, oldRe, oldIm;   //real and imaginary parts of new and old z
        double zoom = 1, moveX = -0.5, moveY = 0; //you can change these to zoom and change position
        QColor color; //the RGB color value for the pixel
        int maxIterations = 300;//after how much iterations the function should stop

        //loop through every pixel
        for (int y = 0; y < h; y++) {
            for (int x = 0; x < w; x++) {
                //calculate the initial real and imaginary part of z, based on the pixel location and zoom and position values
                pr = 1.5 * (x - w / 2) / (0.5 * zoom * w) + moveX;
                pi = (y - h / 2) / (0.5 * zoom * h) + moveY;
                newRe = newIm = oldRe = oldIm = 0; //these should start at 0,0
                //"i" will represent the number of iterations
                int i;
                //start the iteration process
                for (i = 0; i < maxIterations; i++) {
                    //remember value of previous iteration
                    oldRe = newRe;
                    oldIm = newIm;
                    //the actual iteration, the real and imaginary part are calculated
                    newRe = oldRe * oldRe - oldIm * oldIm + pr;
                    newIm = 2 * oldRe * oldIm + pi;
                    //if the point is outside the circle with radius 2: stop
                    if((newRe * newRe + newIm * newIm) > 4) break;
                }
                //use color model conversion to get rainbow palette, make brightness black if maxIterations reached
                color.setHsv(i % 256, 255, 255 * (i < maxIterations));
                //draw the pixel
                p->setPen(color);
                p->drawPoint(x, y);
            }
        }
    }


    QVector<QRgb> palette;
    int w, h;
    int curMaxRecursions;
    FractalKind kind;
};

Fractal::Fractal(int w, int h) : Effect (w, h)
{
    d = new Priv(w, h);
}

Fractal::~Fractal()
{
    delete d;
}

void Fractal::create()
{
}

void Fractal::destroy()
{
}

void Fractal::update()
{
    d->curMaxRecursions = (d->curMaxRecursions + 1) % 9;
}

const QVector<QRgb>& Fractal::palette() const
{
    return d->palette;
}

bool Fractal::paint(QPainter *painter) const
{
    if (!painter)
        return false;

    switch (d->kind) {
    case K_JULIA_SET:
        d->drawJuliaSet(painter);
        break;

    case K_MANDELBROT:
        d->drawMandelbrot(painter);
        break;
    }

    return true;
}
