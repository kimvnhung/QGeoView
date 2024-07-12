#include "mainwindow.h"
#include "routeline.h"
#include "target.h"
#include "ui_mainwindow.h"

#include <QTimer>
#include <helpers.h>

#include <QGeoView/QGVLayerOSM.h>
#include <QGeoView/QGVWidgetText.h>

#define INFO_TEMPLATE "(%1,%2) - (%3,%4)"

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
    mOsmLayer = new QGVLayerOSM("http://localhost:8080/tile/${z}/${x}/${y}.png");

    mMap->addItem(mOsmLayer);
    initInfomationWidget();

    mInfoLayer = new QGVLayer();
    mInfoLayer->setName("Infomations");
    mInfoLayer->setDescription("Demo for infomations");
    mMap->addItem(mInfoLayer);



    // Show init area
    QTimer::singleShot(100, this, [this]() {
        mMap->cameraTo(QGVCameraActions(mMap).scaleTo(QGV::GeoRect(13, 90, 19, 120)));
    });

    mTimer = new QTimer(this);
    mTimer->setInterval(3000);
    connect(mTimer, &QTimer::timeout, this, &MainWindow::onUpdatePos);
    mTimer->setSingleShot(true);
}

void MainWindow::initInfomationWidget()
{
    if(mMap){
        mTopLeftLb = new QGVWidgetText();
        mTopRightLb = new QGVWidgetText();
        mBottomLeftLb = new QGVWidgetText();
        mBottomRightLb = new QGVWidgetText();

        mTopLeftLb->setAnchor(QPoint(0,0),{Qt::LeftEdge,Qt::TopEdge});
        mTopRightLb->setAnchor(QPoint(0,0),{Qt::TopEdge,Qt::RightEdge});
        mBottomLeftLb->setAnchor(QPoint(0,0),{Qt::BottomEdge,Qt::LeftEdge});
        mBottomRightLb->setAnchor(QPoint(0,0),{Qt::BottomEdge,Qt::RightEdge});

        mMap->addWidget(mTopLeftLb);
        mMap->addWidget(mTopRightLb);
        mMap->addWidget(mBottomLeftLb);
        mMap->addWidget(mBottomRightLb);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_startBtn_clicked()
{
    if(mTimer){
        if(mTimer->isActive()){
            mTimer->stop();
            ui->startBtn->setText("Start");
        }else {
            mTimer->start();
            ui->startBtn->setText("Stop");
        }
    }
}

void MainWindow::on_changeDirectionBtn_clicked()
{
    // Change direction
}

void MainWindow::on_switchHightLightBtn_clicked()
{
    // Switch highlight
}

void MainWindow::onProjectChanged()
{
    qDebug() << "Projection changed";
}

void MainWindow::onUpdatePos()
{

    // Update position
    // Start GPS
    QGV::GeoRect curArea = mMap->getProjection()->projToGeo(mMap->getCamera().projRect());
    // Random pos in curArea
    QGV::GeoPos pos = Helpers::randPos(curArea);
    auto item = new Target(pos);
    mInfoLayer->addItem(item);
    QGV::GeoPos pos2 = Helpers::randPos(curArea);
    auto item2 = new RouteLine(pos,pos2,true,RouteLine::Type::CURRENT);
    mInfoLayer->addItem(item2);
    qDebug()<<"random start : "<<pos;
    qDebug()<<"random end : "<<pos2;
}

void MainWindow::onAreaChanged()
{
    QRectF visibleRect = mMap->getCamera().projRect();
    QGV::GeoRect visibleGeoRect = mMap->getProjection()->projToGeo(visibleRect);

    mTopLeftLb->setText(QString(INFO_TEMPLATE)
                                .arg(QString::number(visibleGeoRect.topLeft().latitude(),'f',2))
                                .arg(QString::number(visibleGeoRect.topLeft().longitude(),'f',2))
                                .arg(QString::number(visibleRect.topLeft().x(),'f',2))
                                .arg(QString::number(visibleRect.topLeft().y(),'f',2))
                        );
    mTopRightLb->setText(QString(INFO_TEMPLATE)
                                .arg(QString::number(visibleGeoRect.topRight().latitude(),'f',2))
                                .arg(QString::number(visibleGeoRect.topRight().longitude(),'f',2))
                                .arg(QString::number(visibleRect.topRight().x(),'f',2))
                                .arg(QString::number(visibleRect.topRight().y(),'f',2))
                        );
    mBottomLeftLb->setText(QString(INFO_TEMPLATE)
                                .arg(QString::number(visibleGeoRect.bottomLeft().latitude(),'f',2))
                                .arg(QString::number(visibleGeoRect.bottomLeft().longitude(),'f',2))
                                .arg(QString::number(visibleRect.bottomLeft().x(),'f',2))
                                .arg(QString::number(visibleRect.bottomLeft().y(),'f',2))
                        );
    mBottomRightLb->setText(QString(INFO_TEMPLATE)
                                .arg(QString::number(visibleGeoRect.bottomRight().latitude(),'f',2))
                                .arg(QString::number(visibleGeoRect.bottomRight().longitude(),'f',2))
                                .arg(QString::number(visibleRect.bottomRight().x(),'f',2))
                                .arg(QString::number(visibleRect.bottomRight().y(),'f',2))
                        );
}

