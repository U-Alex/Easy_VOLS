#ifndef MAPVIEW_H
#define MAPVIEW_H

//#include <QtGui>
#include <QWheelEvent>
#include <QGraphicsView>

//QT_FORWARD_DECLARE_CLASS(QSlider)

namespace Ui {
class MapView;
}

class MapView : public QGraphicsView
{
    Q_OBJECT

public:
    explicit MapView(QWidget *parent);
    ~MapView();


//signals:
//    void zoomlEvent(int);

protected:
    void wheelEvent(QWheelEvent *);

private slots:
    void on_pushButton_clicked();
    void setupMatrix();


private:
    Ui::MapView *ui;

};

#endif // MAPVIEW_H
