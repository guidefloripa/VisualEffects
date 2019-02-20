#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>

#include "fxwidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    FxWidget *fxwidget;

    QComboBox *effectsBox;
    QPushButton* playPauseButton;

    QPushButton* decreaseRefreshButton;
    QPushButton* increaseRefreshButton;

    QLabel *updateIntervalLabel;

public slots:
    void onStatusUpdated();
};

#endif // MAINWINDOW_H
