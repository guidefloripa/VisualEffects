#include "fxwidget.h"

#include <QTimer>
#include <QPainter>

#include "doomfire.h"
#include "starfield.h"
#include "tunnel.h"
#include "recursiontree.h"
#include "sierpinski.h"
#include "fractal.h"

typedef QPair<QString,Effect*> FxElement;

class FxWidget::Priv
{
public:
    Priv(int _w, int _h) : currentFx(nullptr), w(_w), h(_h), updateInverval(30), isRunning(true), isCreated(true) {

        effects.push_back(FxElement("Doom Fire", new DoomFire(w, h)));
        effects.push_back(FxElement("Star Field", new StarField(w, h)));
        effects.push_back(FxElement("Tunnel", new Tunnel(w, h)));
        effects.push_back(FxElement("Recursion Tree", new RecursionTree(w, h)));
        effects.push_back(FxElement("Sierpinski", new Sierpinski(w, h)));
        effects.push_back(FxElement("Fractal", new Fractal(w, h)));

        selectFx(0);
    }

    ~Priv() {
        foreach (FxElement fx, effects)
            delete fx.second;
    }

    void selectFx(int i) {
        currentFx = effects[i].second;
    }

    QVector<FxElement> effects;
    Effect *currentFx;
    QTimer timer;

    int w;
    int h;

    int updateInverval;
    bool isRunning;
    bool isCreated;
};

FxWidget::FxWidget(QWidget *parent) : QWidget(parent)
{
    setMinimumSize(400, 320);
    d = new Priv(width(), height());
    connect(&d->timer, &QTimer::timeout, this, &FxWidget::onTimerUpdate);
    d->timer.start(d->updateInverval);
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
    return d->updateInverval;
}

bool FxWidget::isRunning()
{
    return d->isRunning;
}

bool FxWidget::isCreated()
{
    return d->isCreated;
}

void FxWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter p(this);
    d->currentFx->paint(&p);
}

void FxWidget::onTimerUpdate()
{
    d->currentFx->update();
    update();
}

void FxWidget::onEffectSelected(int fx)
{
    d->selectFx(fx);
    d->updateInverval = d->currentFx->defaultRefreshRate();
    d->timer.setInterval(d->updateInverval);
    update();

    emit statusUpdated();
}

void FxWidget::onPlayPausePressed()
{
    if (d->isRunning)
        d->timer.stop();
    else
        d->timer.start(d->updateInverval);

    d->isRunning = !d->isRunning;
    emit statusUpdated();
}

void FxWidget::onIncreaseIntervalPressed()
{
    if (d->updateInverval < 500) {
        if (d->updateInverval >= 100)
            d->updateInverval += 20;
        else
            d->updateInverval += 5;

        d->timer.setInterval(d->updateInverval);
        emit statusUpdated();
    }
}

void FxWidget::onDecreaseIntervalPressed()
{

    if (d->updateInverval > 5) {
        if (d->updateInverval <= 100)
            d->updateInverval -= 5;
        else
            d->updateInverval -= 20;
        d->timer.setInterval(d->updateInverval);
        emit statusUpdated();
    }
}

void FxWidget::onFxKindSelected(int kind)
{
    d->currentFx->setFxKind(kind);
}
