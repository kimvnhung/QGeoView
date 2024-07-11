#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTimer>
#include <helpers.h>

#include <QGeoView/QGVLayerOSM.h>
#include <QGeoView/QGVWidgetText.h>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mMap = new QGVMap(this);
    connect(mMap,&QGVMap::projectionChanged,this, &MainWindow::onProjectChanged);
    connect(mMap,&QGVMap::areaChanged,this, &MainWindow::onAreaChanged);
    ui->centralwidget->layout()->addWidget(mMap);
    Helpers::setupCachedNetworkAccessManager(this);

    // Background layer
    mOsmLayer = new QGVLayerOSM(/*"http://192.168.239.69:1158/tile/${z}/${x}/${y}.png"*/);

    mMap->addItem(mOsmLayer);
    initInfomationWidget();

    mInfoLayer = new QGVLayer();
    mInfoLayer->setName("Infomations");
    mInfoLayer->setDescription("Demo for infomations");



    // Show init area
    QTimer::singleShot(100, this, [this]() {
        mMap->cameraTo(QGVCameraActions(mMap).scaleTo(QGV::GeoRect(13, 90, 19, 120)));
    });
}

void MainWindow::initInfomationWidget()
{
    if(mMap){
        auto topLeftLb = new QGVWidgetText();
        auto topRightLb = new QGVWidgetText();
        auto bottomLeftLb = new QGVWidgetText();
        auto bottomRightLb = new QGVWidgetText();

        topLeftLb->setAnchor(QPoint(0,0),{Qt::LeftEdge,Qt::TopEdge});
        topRightLb->setAnchor(QPoint(0,0),{Qt::TopEdge,Qt::RightEdge});
        bottomLeftLb->setAnchor(QPoint(0,0),{Qt::BottomEdge,Qt::LeftEdge});
        bottomRightLb->setAnchor(QPoint(0,0),{Qt::BottomEdge,Qt::RightEdge});

        topLeftLb->setText("Hellop");
        topRightLb->setText("Hellop");
        bottomLeftLb->setText("Hellop");
        bottomRightLb->setText("Hellop");

        mMap->addWidget(topLeftLb);
        mMap->addWidget(topRightLb);
        mMap->addWidget(bottomLeftLb);
        mMap->addWidget(bottomRightLb);
    }
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

void MainWindow::onProjectChanged()
{
    qDebug() << "Projection changed";
}

void MainWindow::onAreaChanged()
{
    qDebug() << "Area changed";
    QRectF visibleRect = mMap->getCamera().projRect();
    QGV::GeoRect visibleGeoRect = mMap->getProjection()->projToGeo(visibleRect);


}
