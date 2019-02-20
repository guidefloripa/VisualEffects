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

const uchar* Effect::getAlignedVector()
{
    return bufferValue;
}

int Effect::width()
{
    return w;
}

int Effect::height()
{
    return h;
}

int Effect::getVectorIdx(int i, int j) {
    return i + aligned_w * j;
}

uchar Effect::getValue(int i, int j) {
    if (i >= 0 && i < w && j >= 0 && j < h)
        return bufferValue[getVectorIdx(i, j)];

    return static_cast<uchar>(-1);
}

void Effect::setValue(int i, int j, uchar value) {
    if (i >= 0 && i < w && j >= 0 && j < h)
        bufferValue[getVectorIdx(i, j)] = value;
}

void Effect::reset()
{
    size_t len = static_cast<size_t>(vector_len)*sizeof(uchar);
    memset(bufferValue, 0, len);
}
