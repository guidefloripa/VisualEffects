#include "fractal.h"

#include "sierpinski.h"

#include <math.h>
#include <QColor>

// code: https://lodev.org/cgtutor/juliamandelbrot.html

typedef enum {
    K_JULIA_SET = 0,
    K_MANDELBROT = 1
} FractalKind;

struct FractalParams {
    double zoom;
    double moveX;
    double moveY;
    int maxIterations;
};

class Fractal::Priv
{
public:
    explicit Priv (int _w, int _h) : w(_w), h(_h), kind(K_JULIA_SET) {
        initParams();
    }

    void initParams() {
        params.zoom = 1.0;
        params.moveX = 0.0;
        params.moveY = 0.0;
        params.maxIterations = 300;
    }

    void drawJuliaSet(QPainter *p) {
        double cRe, cIm;           //real and imaginary part of the constant c, determinate shape of the Julia Set
        double newRe, newIm, oldRe, oldIm;   //real and imaginary parts of new and old
        QColor color; //the RGB color value for the pixel
        int maxIterations = 300; //after how much iterations the function should stop

        //pick some values for the constant c, this determines the shape of the Julia Set
        cRe = -0.7;
        cIm = 0.27015;

        //loop through every pixel
        for (int y = 0; y < h; y++) {
            for (int x = 0; x < w; x++) {
                //calculate the initial real and imaginary part of z, based on the pixel location and zoom and position values
                newRe = 1.5 * (x - w / 2) / (0.5 * params.zoom * w) + params.moveX;
                newIm = (y - h / 2) / (0.5 * params.zoom * h) + params.moveY;
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
        QColor color; //the RGB color value for the pixel
        int maxIterations = 300;//after how much iterations the function should stop

        //loop through every pixel
        for (int y = 0; y < h; y++) {
            for (int x = 0; x < w; x++) {
                //calculate the initial real and imaginary part of z, based on the pixel location and zoom and position values
                pr = 1.5 * (x - w / 2) / (0.5 * params.zoom * w) + params.moveX;
                pi = (y - h / 2) / (0.5 * params.zoom * h) + params.moveY;
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
    FractalKind kind;
    struct FractalParams params;
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
    d->initParams();
}

void Fractal::destroy()
{
}

void Fractal::update()
{
}

int Fractal::defaultRefreshRate()
{
    return 0;
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

QPair<int, QVector<QString>> Fractal::fxKindList() const
{
    QVector<QString> v;

    v.append("Julia Set");
    v.append("Mandelbrot");

    return QPair<int, QVector<QString>>(d->kind, v);
}

void Fractal::setFxKind(int kind)
{
    switch (kind) {
        case K_JULIA_SET:
        case K_MANDELBROT:
            d->kind = FractalKind(kind);
            break;
    }
}

//#include <QDebug>
bool Fractal::keyPressed(FxKey key)
{
    switch (key) {
    case KEY_MINUS:
        d->params.zoom /= qMax(0.005, pow(1.015, d->params.zoom));
        //qDebug() << "New zoom: " << d->params.zoom;
        break;

    case KEY_PLUS:
        d->params.zoom *= qMax(0.005, pow(1.015, d->params.zoom));
        //qDebug() << "New zoom: " << d->params.zoom;
        break;

    case KEY_LEFT:
        d->params.moveX -= qMax(0.001, 0.0003 * d->params.moveX / d->params.zoom);
        //qDebug() << "New X: " << d->params.moveX;
        break;

    case KEY_RIGHT:
        d->params.moveX += qMax(0.001, 0.0003 * d->params.moveX / d->params.zoom);
        //qDebug() << "New X: " << d->params.moveX;
        break;

    case KEY_DOWN:
        d->params.moveY += qMax(0.001, 0.0003 * d->params.moveY / d->params.zoom);
        //qDebug() << "New Y: " << d->params.moveY;
        break;

    case KEY_UP:
        d->params.moveY -= qMax(0.001, 0.0003 * d->params.moveY / d->params.zoom);
        //qDebug() << "New Y: " << d->params.moveY;
        break;
    }

    return true;
}
