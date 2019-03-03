#ifndef FXWIDGET_H
#define FXWIDGET_H

#include <QWidget>
#include <QString>
#include <QVector>

class FxWidget : public QWidget
{
    Q_OBJECT
public:
    FxWidget(QWidget *parent = nullptr);
    ~FxWidget();

    QVector<QString> fxList();
    QPair<int, QVector<QString>> fxKindList();
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
    void paramsUpdated();

public slots:
    void onTimerUpdate();

    void onEffectSelected(int fx);
    void onPlayPausePressed();

    void onIncreaseIntervalPressed();
    void onDecreaseIntervalPressed();

    void onFxKindSelected(int kind);
};

#endif // FXWIDGET_H
