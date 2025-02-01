#include "mapmanager.h"
#include "ui_mapmanager.h"

#include <QImageReader>
//#include <QUrlQuery>        //
//#include "b_logic/restaccessmanager.h"   //

MapManager::MapManager(QWidget *parent, UserSession *us) :
    QWidget(parent),
    ui(new Ui::MapManager),
    userSession(us)
{
    ui->setupUi(this);

//    ui->map_scrollArea->setVisible(false);
//    ui->map_frame_L->setVisible(false);

    scene = new MapScene(this);
    mapView = new MapView(this);
    mapView->setScene(scene);
//    ui->map_frame_temp->deleteLater();                   //
    ui->map_Layout->addWidget(mapView, 0, 1);


    QImageReader::setAllocationLimit(2048);
//    QPixmap image("images/map1.png");
//    pix_map = scene->addPixmap(image);
//    pix_map ->setData(6, "pix_map");              //

    mapView->centerOn(QPointF(9297, 12978));        //



    QMap<QString, QString> test_params;
    test_params.insert("1", "2");

    ObjCoup *coup = new ObjCoup(test_params);

    coup->setPos(QPoint(9297, 12978));
    scene->addItem(std::move(coup));


//
//    RestAccessManager *ram = new RestAccessManager(this);
//    RestAccessManager::ResponseCallback callback = [/*this*/](QNetworkReply* reply, bool success) {
//        if (success) qDebug() << "success";
//        else         qDebug() << "! success";
//        qDebug() << reply->readAll();
//    };
//    ram->setUrl(QUrl("https://reqres.in/api/users?page=2"));
//    QUrlQuery query;
////    query.addQueryItem("page/", QString::number(1));
//    ram->get("---", query, callback);
//    qDebug() << "ram->get";
////    qDebug() << "sslSupported " << ram->sslSupported();



    userSession->getObj();
    userSession->getObj(1);
    userSession->getObj(23);






}

MapManager::~MapManager()
{
    delete ui;
}
