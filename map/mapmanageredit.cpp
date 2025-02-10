#include "mapmanageredit.h"
#include "ui_mapmanageredit.h"

#include <QColorDialog>
#include <QBitmap>
#include <QPainter>

#include "map/obj/obj.h"

MapManagerEdit::MapManagerEdit(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::MapManagerEdit)
{
    ui->setupUi(this);

    ui->pb_lab_apply->setVisible(false);
    ui->pb_line_apply->setVisible(false);

    styles = {Qt::SolidLine, Qt::DashLine, Qt::DotLine, Qt::DashDotLine, Qt::DashDotDotLine};
    QSize sz{140, 2};
    QBitmap pix{sz};
    ui->comboBox_line_type->setIconSize( sz );
    foreach (auto style, styles) {
        pix.clear();
        QPainter painter( &pix );
        painter.setRenderHint( QPainter::Antialiasing );
        painter.setPen( QPen( Qt::black, 2, style, Qt::RoundCap ) );
        painter.drawLine( 2, sz.height() / 2, sz.width() - 2, sz.height() / 2 );
        ui->comboBox_line_type->addItem( pix, QString() );
    }
}

MapManagerEdit::~MapManagerEdit()
{
    delete ui;
}



void MapManagerEdit::on_pb_lab_apply_clicked()
{

}



void MapManagerEdit::cableClick(ObjPolyline *item)
{
    if (line != item) {
        line = item;
        ui->pb_line_apply->blockSignals(true);
        QList<QString> param = line->data((int)Idx::param).toString().split(",");
        ui->comboBox_line_type->setCurrentIndex(param.at(1).toInt() - 1);
        ui->spinBox_line_size->setValue(param.at(0).toInt());
        ui->spinBox_line_z_index->setValue(int(item->zValue()));
        ui->pb_line_apply->blockSignals(false);
    }
}

void MapManagerEdit::on_pb_line_apply_clicked()
{
    if (line == nullptr) return;
    int param1 = ui->spinBox_line_size->value();
    int param2 = ui->comboBox_line_type->currentIndex() + 1;
    int param3 = ui->spinBox_line_z_index->value();
    line->setData((int)Idx::param, QString("%1,%2,%3").arg(param1).arg(param2).arg(param3));
    line->setZValue(param3);
    line->setPen(QPen(QColor(line->data((int)Idx::cabcolor).toString()),
                      param1,
                      Qt::PenStyle(param2),
                      Qt::RoundCap,
                      Qt::RoundJoin
                      ));
    //line->update();
}

