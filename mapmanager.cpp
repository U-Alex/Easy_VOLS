#include "mapmanager.h"
#include "ui_mapmanager.h"

#include <QImageReader>
//#include <QScrollBar>

MapManager::MapManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MapManager)
{
    ui->setupUi(this);

    ui->map_scrollArea->setVisible(false);
    ui->map_frame_L->setVisible(false);

    scene = new MapScene(this);
    mapView = new MapView(this);
    mapView->setScene(scene);
    ui->map_frame_temp->deleteLater();                   //
    ui->map_Layout->addWidget(mapView, 0, 1);


    QImageReader::setAllocationLimit(2048);
    QPixmap image("images/map1.png");
    pix_map = scene->addPixmap(image);
    pix_map ->setData(6, "pix_map");

    mapView->centerOn(QPointF(9297, 12978));        //

//    connect(ui->zoomSlider, SIGNAL(valueChanged(int)), mapView, SLOT(setupMatrix(int)));
//        connect(mapView, SIGNAL(zoomlEvent(int)), ui->zoomSlider, SLOT(setValue(int)));
}

MapManager::~MapManager()
{
    delete ui;
}
