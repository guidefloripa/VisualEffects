#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "fxwidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("Visual Effects Qt");

    effectsBox = new QComboBox(this);
    foreach(QString fx, ui->fxwidget->fxList())
        effectsBox->addItem(fx);

    playPauseButton = new QPushButton(this);

    decreaseRefreshButton = new QPushButton(this);
    decreaseRefreshButton->setText("-");
    increaseRefreshButton = new QPushButton(this);
    increaseRefreshButton->setText("+");

    updateIntervalLabel = new QLabel(this);
    updateIntervalLabel->setMinimumWidth(20);
    updateIntervalLabel->setAlignment(Qt::AlignCenter);
    //updateIntervalLabel->setStyleSheet("QLabel{color: white;}");

    ui->mainToolBar->addWidget(effectsBox);

    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addWidget(playPauseButton);

    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addWidget(decreaseRefreshButton);
    ui->mainToolBar->addWidget(updateIntervalLabel);
    ui->mainToolBar->addWidget(increaseRefreshButton);

    connect(effectsBox, SIGNAL(currentIndexChanged(int)), ui->fxwidget, SLOT(onEffectSelected(int)));
    connect(playPauseButton, &QPushButton::pressed, ui->fxwidget, &FxWidget::onPlayPausePressed);

    connect(decreaseRefreshButton, &QPushButton::released, ui->fxwidget, &FxWidget::onDecreaseIntervalPressed);
    connect(increaseRefreshButton, &QPushButton::released, ui->fxwidget, &FxWidget::onIncreaseIntervalPressed);

    connect(ui->fxKindBox, SIGNAL(currentIndexChanged(int)), ui->fxwidget, SLOT(onFxKindSelected(int)));

    connect(ui->fxwidget, &FxWidget::statusUpdated, this, &MainWindow::onStatusUpdated);
    connect(ui->fxwidget, &FxWidget::paramsUpdated, this, &MainWindow::onParamsUpdated);

    onStatusUpdated();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onStatusUpdated()
{
    playPauseButton->setText(ui->fxwidget->isRunning() ? "Pause" : "Play");
    playPauseButton->setEnabled((ui->fxwidget->updateInterval() > 0) ? true : false);

    updateIntervalLabel->setText(QString("%1").arg(ui->fxwidget->updateInterval()));

    onParamsUpdated();
}

void MainWindow::onParamsUpdated()
{
    QPair<int, QVector<QString>> paramsString = ui->fxwidget->fxKindList();

    ui->fxKindBox->clear();
    if (!paramsString.second.isEmpty()) {
        foreach(QString param, paramsString.second)
            ui->fxKindBox->addItem(param);

        ui->fxKindBox->setCurrentIndex(paramsString.first);
        ui->fxKindBox->setEnabled(true);
    }
    else {
        ui->fxKindBox->setEnabled(false);
    }
}
