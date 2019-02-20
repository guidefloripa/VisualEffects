#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("Visual Effects Qt");

    fxwidget = new FxWidget(centralWidget()->width(), centralWidget()->height());
    setCentralWidget(fxwidget);

    effectsBox = new QComboBox(this);
    foreach(QString fx, fxwidget->fxList())
        effectsBox->addItem(fx);

    playPauseButton = new QPushButton(this);

    decreaseRefreshButton = new QPushButton(this);
    decreaseRefreshButton->setText("-");
    increaseRefreshButton = new QPushButton(this);
    increaseRefreshButton->setText("+");

    updateIntervalLabel = new QLabel(this);
    updateIntervalLabel->setMinimumWidth(20);
    updateIntervalLabel->setAlignment(Qt::AlignCenter);

    ui->mainToolBar->addWidget(effectsBox);

    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addWidget(playPauseButton);

    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addWidget(decreaseRefreshButton);
    ui->mainToolBar->addWidget(updateIntervalLabel);
    ui->mainToolBar->addWidget(increaseRefreshButton);

    connect(effectsBox, SIGNAL(currentIndexChanged(int)), fxwidget, SLOT(onEffectSelected(int)));
    connect(playPauseButton, &QPushButton::pressed, fxwidget, &FxWidget::onPlayPausePressed);

    connect(decreaseRefreshButton, &QPushButton::released, fxwidget, &FxWidget::onDecreaseIntervalPressed);
    connect(increaseRefreshButton, &QPushButton::released, fxwidget, &FxWidget::onIncreaseIntervalPressed);

    connect(fxwidget, &FxWidget::statusUpdated, this, &MainWindow::onStatusUpdated);

    onStatusUpdated();
}

MainWindow::~MainWindow()
{
    delete fxwidget;
    delete ui;
}

void MainWindow::onStatusUpdated()
{
    playPauseButton->setText(fxwidget->isRunning() ? "Pause" : "Play");
    updateIntervalLabel->setText(QString("%1").arg(fxwidget->updateInterval()));
}
