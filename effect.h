#ifndef EFFECT_H
#define EFFECT_H

#include <qglobal.h>
#include <QVector>
#include <QRgb>
#include <QPainter>

class Effect
{
public:
    Effect(int w, int h);
    virtual ~Effect();

    int width();
    int height();

    void reset();

    uchar getValue(int i, int j) const;
    void setValue(int i, int j, uchar value);

    virtual void create() = 0;
    virtual void destroy() = 0;
    virtual void update() = 0;
    virtual int defaultRefreshRate() = 0;
    virtual const QVector<QRgb>& palette() const = 0;

    virtual bool paint(QPainter *painter) const;

    virtual QPair<int, QVector<QString>> fxKindList() const;
    virtual void setFxKind(int kind);

protected:
    int w;
    int h;

    uchar* bufferValue;

private:
    int aligned_w;
    int vector_len;

    int getVectorIdx(int i, int j) const;
};


#endif // EFFECT_H
