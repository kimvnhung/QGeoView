#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void onStartBtnClicked();
    void onChangeDirectionBtnClicked();
    void onSwitchHighlightBtnClicked();

private:
    Ui::MainWindow* ui;
    QTimer *mTimer = NULL;
    QGVMap* mMap = NULL;
};;

#endif // MAINWINDOW_H
