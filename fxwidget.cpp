#include "fxwidget.h"

#include <QTimer>
#include <QPainter>
#include <QKeyEvent>

#include "doomfire.h"
#include "starfield.h"
#include "tunnel.h"
#include "recursiontree.h"
#include "sierpinski.h"
#include "fractal.h"
#include "plasma.h"
#include "fire.h"

typedef QPair<QString,Effect*> FxElement;

class FxWidget::Priv
{
public:
    Priv(int _w, int _h) : currentFx(nullptr), w(_w), h(_h), isRunning(true), isCreated(true) {

        effects.push_back(FxElement("Doom Fire", new DoomFire(w, h)));
        effects.push_back(FxElement("Star Field", new StarField(w, h)));
        effects.push_back(FxElement("Tunnel", new Tunnel(w, h)));
        effects.push_back(FxElement("Recursion Tree", new RecursionTree(w, h)));
        effects.push_back(FxElement("Sierpinski", new Sierpinski(w, h)));
        effects.push_back(FxElement("Fractal", new Fractal(w, h)));
        effects.push_back(FxElement("Plasma", new Plasma(w, h)));
        effects.push_back(FxElement("Fire", new Fire(w, h)));

        selectFx(0);
    }

    ~Priv() {
        foreach (FxElement fx, effects)
            delete fx.second;
    }

    void selectFx(int i) {
        currentFx = effects[i].second;
        updateInterval = currentFx->defaultRefreshRate();
        timer.setInterval(updateInterval);
    }

    QVector<FxElement> effects;
    Effect *currentFx;
    QTimer timer;

    int w;
    int h;

    int updateInterval;
    bool isRunning;
    bool isCreated;
};

FxWidget::FxWidget(QWidget *parent) : QWidget(parent)
{
    setMinimumSize(400, 320);
    setFocusPolicy(Qt::StrongFocus);

    d = new Priv(width(), height());
    connect(&d->timer, &QTimer::timeout, this, &FxWidget::onTimerUpdate);
    if (d->updateInterval > 0)
        d->timer.start();
}

FxWidget::~FxWidget()
{
    delete d;
}

QVector<QString> FxWidget::fxList()
{
    QVector<QString> v;

    foreach (FxElement fx, d->effects)
        v.append(fx.first);

    return v;
}

QPair<int, QVector<QString>> FxWidget::fxKindList()
{
    return d->currentFx->fxKindList();
}

int FxWidget::updateInterval()
{
    return d->updateInterval;
}

bool FxWidget::isRunning()
{
    return d->isRunning;
}

bool FxWidget::isCreated()
{
    return d->isCreated;
}

//#include <QDebug>
void FxWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    //qDebug() << "Paint event" << d->updateInterval;

    QPainter p(this);
    d->currentFx->paint(&p);
}

void FxWidget::keyPressEvent(QKeyEvent *event)
{
    bool needUpdate = false;

    switch (event->key()) {
    case Qt::Key_Plus: needUpdate = d->currentFx->keyPressed(KEY_PLUS); break;
    case Qt::Key_Minus: needUpdate = d->currentFx->keyPressed(KEY_MINUS); break;
    case Qt::Key_Left: needUpdate = d->currentFx->keyPressed(KEY_LEFT); break;
    case Qt::Key_Right: needUpdate = d->currentFx->keyPressed(KEY_RIGHT); break;
    case Qt::Key_Up: needUpdate = d->currentFx->keyPressed(KEY_UP); break;
    case Qt::Key_Down: needUpdate = d->currentFx->keyPressed(KEY_DOWN); break;
    }

    QWidget::keyPressEvent(event);
    if (needUpdate)
        update();
}

void FxWidget::onTimerUpdate()
{
    d->currentFx->update();
    update();
}

void FxWidget::onEffectSelected(int fx)
{
    d->selectFx(fx);

    d->timer.stop();
    if (d->isRunning && d->updateInterval > 0)
        d->timer.start();

    update();
    emit statusUpdated();
    emit paramsUpdated();
}

void FxWidget::onPlayPausePressed()
{
    if (d->isRunning)
        d->timer.stop();
    else if (d->updateInterval > 0)
        d->timer.start();

    d->isRunning = !d->isRunning;
    emit statusUpdated();
}

void FxWidget::onIncreaseIntervalPressed()
{
    if (d->updateInterval < 500) {
        if (d->updateInterval >= 100)
            d->updateInterval += 20;
        else
            d->updateInterval += 5;

        d->timer.setInterval(d->updateInterval);
        emit statusUpdated();
    }
}

void FxWidget::onDecreaseIntervalPressed()
{
    if (d->updateInterval > 5) {
        if (d->updateInterval <= 100)
            d->updateInterval -= 5;
        else
            d->updateInterval -= 20;
        d->timer.setInterval(d->updateInterval);
        emit statusUpdated();
    }
}

void FxWidget::onFxKindSelected(int kind)
{
    d->currentFx->setFxKind(kind);
}
