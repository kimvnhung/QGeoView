/***************************************************************************
 * QGeoView is a Qt / C ++ widget for visualizing geographic data.
 * Copyright (C) 2018-2024 Andrey Yaroshenko.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, see https://www.gnu.org/licenses.
 ****************************************************************************/

#include "mainwindow.h"

#include <QTimer>

#include <helpers.h>
#include <rectangle.h>

#include <QGeoView/QGVLayerOSM.h>

#include <QGeoView/Raster/QGVLine.h>
#include <QGeoView/Raster/QGVPoint.h>
#include <QGeoView/Raster/QGVText.h>

MainWindow::MainWindow()
{
    setWindowTitle("QGeoView Samples - 10000 elements");

    mMap = new QGVMap(this);
    setCentralWidget(mMap);

    Helpers::setupCachedNetworkAccessManager(this);

    // Background layer
    auto osmLayer = new QGVLayerOSM();
    mMap->addItem(osmLayer);

    // 10000 layer
    mMap->addItem(create10000Layer());

    // Show target area
    QTimer::singleShot(100, this, [this]() {
        auto target = target10000Area();
        mMap->cameraTo(QGVCameraActions(mMap).scaleTo(target));
    });
}

MainWindow::~MainWindow()
{
}

QGV::GeoRect MainWindow::target10000Area() const
{
    return mMap->getProjection()->boundaryGeoRect();
}

QGVLayer* MainWindow::create10000Layer() const
{
    /*
     * Layers will be owned by map.
     */
    auto target = target10000Area();
    auto layer = new QGVLayer();
    layer->setName("10000 elements");
    layer->setDescription("Demo for 10000 elements");

    /*
     * Items will be owned by layer.
     */
    const int size = 20000;
    for (int i = 0; i < 100; i++) {
        // if(i%10==0){
            auto item = new Rectangle(Helpers::randRect(mMap, target, size), Qt::red);
            layer->addItem(item);
        // }else if(i%5==1){
        //     auto item = new QGVText();
        //     item->setGeometry(Helpers::randPos(target));
        //     item->setText("hello: "+QString::number(i));
        //     layer->addItem(item);
        // }else if(i%5==2){
        //     auto item = new QGVPoint();
        //     item->setGeometry(Helpers::randPos(target));
        //     layer->addItem(item);
        // }else {
        //     auto item = new QGVLine();
        //     item->setGeometry(Helpers::randPos(target), Helpers::randPos(target));
        //     layer->addItem(item);
        // }
    }

    return layer;
}
