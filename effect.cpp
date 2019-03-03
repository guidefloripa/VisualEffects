#include "effect.h"

#define MEM_ALIGNMENT 4

Effect::Effect(int _w, int _h) : w(_w), h(_h)
{
    if (w % MEM_ALIGNMENT)
        aligned_w = w + MEM_ALIGNMENT - (w % MEM_ALIGNMENT);
    else
        aligned_w = w;

    vector_len = aligned_w * h;
    bufferValue = new uchar[vector_len];

    reset();
}

Effect::~Effect()
{
    delete bufferValue;
}

int Effect::width()
{
    return w;
}

int Effect::height()
{
    return h;
}

void Effect::reset()
{
    size_t len = static_cast<size_t>(vector_len)*sizeof(uchar);
    memset(bufferValue, 0, len);
}

int Effect::getVectorIdx(int i, int j) const
{
    return i + aligned_w * j;
}

uchar Effect::getValue(int i, int j) const
{
    if (i >= 0 && i < w && j >= 0 && j < h)
        return bufferValue[getVectorIdx(i, j)];

    return static_cast<uchar>(-1);
}

void Effect::setValue(int i, int j, uchar value)
{
    if (i >= 0 && i < w && j >= 0 && j < h)
        bufferValue[getVectorIdx(i, j)] = value;
}

bool Effect::paint(QPainter *painter) const
{
    if (!painter)
        return false;

    QImage img(bufferValue, w, h, QImage::Format_Indexed8);
    img.setColorTable(palette());

    painter->drawImage(0, 0, img);

    return true;
}

QPair<int, QVector<QString>> Effect::fxKindList() const
{
    return QPair<int, QVector<QString>> ();
}

void Effect::setFxKind(int kind)
{
    Q_UNUSED(kind);
}
