#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onStartBtnClicked()
{
    // Start GPS
}

void MainWindow::onChangeDirectionBtnClicked()
{
    // Change direction
}

void MainWindow::onSwitchHighlightBtnClicked()
{
    // Switch highlight
}

