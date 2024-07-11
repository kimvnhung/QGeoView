#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QGeoView/QGVLayer.h>
#include <QGeoView/QGVMap.h>

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


    void onProjectChanged();
    void onAreaChanged();
private:
    Ui::MainWindow* ui;
    QTimer *mTimer = NULL;
    QGVMap* mMap = NULL;

    QGVLayer* mOsmLayer = NULL;
    QGVLayer* mInfoLayer = NULL;

    void initInfomationWidget();

};;

#endif // MAINWINDOW_H
