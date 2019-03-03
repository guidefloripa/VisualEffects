#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>

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

    QComboBox *effectsBox;
    QPushButton* playPauseButton;

    QPushButton* decreaseRefreshButton;
    QPushButton* increaseRefreshButton;

    QLabel *updateIntervalLabel;

public slots:
    void onStatusUpdated();
    void onParamsUpdated();
};

#endif // MAINWINDOW_H
