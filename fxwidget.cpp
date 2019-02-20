#include "fxwidget.h"

#include <QTimer>
#include <QImage>
#include <QPainter>

#include "doomfire.h"
#include "starfield.h"

typedef QPair<QString,Effect*> FxElement;

class FxWidget::Priv
{
public:
    Priv(int _w, int _h) : currentFx(nullptr), w(_w), h(_h), updateInverval(30), isRunning(true), isCreated(true) {

        effects.push_back(FxElement("Doom Fire", new DoomFire(w, h)));
        effects.push_back(FxElement("Star Field", new StarField(w, h)));

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

FxWidget::FxWidget(int w, int h, QWidget *parent) : QWidget(parent)
{
    d = new Priv(w, h);
    connect(&d->timer, &QTimer::timeout, this, &FxWidget::onTimerUpdate);
    d->timer.start(d->updateInverval);
}

FxWidget::~FxWidget()
{
    delete d;
}

QList<QString> FxWidget::fxList()
{
    QList<QString> list;

    foreach (FxElement fx, d->effects)
        list.append(fx.first);

    return list;
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

    QImage img(d->currentFx->getAlignedVector(), d->w, d->h, QImage::Format_Indexed8);
    img.setColorTable(d->currentFx->palette());

    QPainter p(this);
    p.drawImage(0, 0, img);
}

void FxWidget::onTimerUpdate()
{
    d->currentFx->update();
    update();
}

void FxWidget::onEffectSelected(int fx)
{
    d->selectFx(fx);
    update();
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
    if (d->updateInverval < 100) {
        d->updateInverval += 5;
        d->timer.setInterval(d->updateInverval);
        emit statusUpdated();
    }
}

void FxWidget::onDecreaseIntervalPressed()
{
    if (d->updateInverval > 5) {
        d->updateInverval -= 5;
        d->timer.setInterval(d->updateInverval);
        emit statusUpdated();
    }
}
