#ifndef FXWIDGET_H
#define FXWIDGET_H

#include <QWidget>
#include <QString>
#include <QList>

class FxWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FxWidget(int w, int h, QWidget *parent = nullptr);
    ~FxWidget();

    QList<QString> fxList();
    int updateInterval();

    bool isRunning();
    bool isCreated();

protected:
    void paintEvent(QPaintEvent *event);

private:
    class Priv;
    Priv *d;

signals:
    void statusUpdated();

public slots:
    void onTimerUpdate();

    void onEffectSelected(int fx);

    void onPlayPausePressed();

    void onIncreaseIntervalPressed();
    void onDecreaseIntervalPressed();
};

#endif // FXWIDGET_H
