#ifndef MAPVIEW_H
#define MAPVIEW_H

//#include <QtGui>
#include <QWheelEvent>
#include <QGraphicsView>

//QT_FORWARD_DECLARE_CLASS(QSlider)

namespace Ui { class MapView; }

class MapView : public QGraphicsView
{
    Q_OBJECT

public:
    explicit MapView(QWidget *parent);
    ~MapView();


//signals:
//    void zoomlEvent(int);

protected:
    void wheelEvent(QWheelEvent *) override;

private slots:
    void setupMatrix();
    void toggleAntialiasing();


private:
    Ui::MapView *ui;

};

#endif // MAPVIEW_H
