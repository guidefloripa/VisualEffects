#ifndef EFFECT_H
#define EFFECT_H

#include <qglobal.h>
#include <QVector>
#include <QRgb>

class Effect
{
public:
    Effect(int w, int h);
    virtual ~Effect();

    const uchar* getAlignedVector();
    int width();
    int height();

    uchar getValue(int i, int j);
    void setValue(int i, int j, uchar value);

    virtual void create() = 0;
    virtual void destroy() = 0;
    virtual void update() = 0;

    virtual const QVector<QRgb>& palette() = 0;

    void reset();

protected:
    int w;
    int h;

    uchar* bufferValue;

private:
    int aligned_w;
    int vector_len;

    int getVectorIdx(int i, int j);
};


#endif // EFFECT_H
