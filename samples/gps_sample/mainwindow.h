#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QGeoView/QGVLayer.h>
#include <QGeoView/QGVMap.h>
#include <QGeoView/QGVWidgetText.h>

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
    void on_startBtn_clicked();
    void on_changeDirectionBtn_clicked();
    void on_switchHightLightBtn_clicked();

    void onProjectChanged();
    void onAreaChanged();
    void onUpdatePos();


private:
    Ui::MainWindow* ui;
    QTimer *mTimer = NULL;
    QGVMap* mMap = NULL;

    QGVLayer* mOsmLayer = NULL;
    QGVLayer* mInfoLayer = NULL;

    QGVWidgetText *mTopLeftLb = NULL;
    QGVWidgetText *mTopRightLb = NULL;
    QGVWidgetText *mBottomLeftLb = NULL;
    QGVWidgetText *mBottomRightLb = NULL;

    void initInfomationWidget();

};;

#endif // MAINWINDOW_H
