#ifndef MAPMANAGEREDIT_H
#define MAPMANAGEREDIT_H

#include <QFrame>
#include <QGraphicsTextItem>

//#include "map/mapscene.h"
#include "map/obj/objpolyline.h"

namespace Ui {
class MapManagerEdit;
}

class MapManagerEdit : public QFrame
{
    Q_OBJECT
public:
    explicit MapManagerEdit(QWidget *parent = nullptr);
    ~MapManagerEdit();

public slots:
//    void slot_blank(QPoint);
//    void slot_label(QGraphicsTextItem *);
    void cableClick(ObjPolyline *);

private:
    Ui::MapManagerEdit *ui;

    ObjPolyline        *line = nullptr;

    QList<Qt::PenStyle> styles;

private slots:
    void on_pb_lab_apply_clicked();
    void on_pb_line_apply_clicked();

};

#endif // MAPMANAGEREDIT_H
