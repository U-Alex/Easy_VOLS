#include "mapview.h"
#include "ui_mapview.h"

#include <QScrollBar>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QGraphicsItem>
#include <QDateTime>

MapView::MapView(QWidget *parent) :
    QGraphicsView(parent),
    ui(new Ui::MapView)
{
    ui->setupUi(this);

    connect(ui->zoomSlider, SIGNAL(valueChanged(int)), this, SLOT(setupMatrix()));
    ui->zoomSlider->setDisabled(true);
    setupMatrix();

}

MapView::~MapView()
{
    delete ui;
}

void MapView::wheelEvent(QWheelEvent *e)
{
    if (e->modifiers() & Qt::ControlModifier) {
        if (e->angleDelta().y() > 0) verticalScrollBar()->setValue(verticalScrollBar()->value() - 120);
        else                         verticalScrollBar()->setValue(verticalScrollBar()->value() + 120);
        e->accept();
    }
    else if (e->modifiers() & Qt::ShiftModifier) {
        if (e->angleDelta().y() > 0) horizontalScrollBar()->setValue(horizontalScrollBar()->value() - 120);
        else                         horizontalScrollBar()->setValue(horizontalScrollBar()->value() + 120);
        e->accept();
    }
    else {
        if (e->angleDelta().y() > 0) ui->zoomSlider->setValue(ui->zoomSlider->value() + 10);
        if (e->angleDelta().y() < 0) ui->zoomSlider->setValue(ui->zoomSlider->value() - 10);
        e->accept();
    }
}

void MapView::setupMatrix()
{
    qreal map_scale = qPow(qreal(2), (ui->zoomSlider->value() - 250) / qreal(50));
    QTransform matrix;
    matrix.scale(map_scale, map_scale);
    this->setTransform(matrix);
}

void MapView::slotCoupOnCenter(/*uint c_id, */QPoint pos)
{
    ui->zoomSlider->setValue(250);
    this->scene()->items(pos).first()->setSelected(1);

    int to_y = pos.y() - (this->viewport()->rect().height() / 2);
    int to_x = pos.x() - (this->viewport()->rect().width() / 2);
    QPropertyAnimation *anim_v = new QPropertyAnimation(this->verticalScrollBar(), "value", this);
    //anim_v->setStartValue(fr_y);
    anim_v->setDuration(300);
    anim_v->setEndValue(to_y);
    QPropertyAnimation *anim_h = new QPropertyAnimation(this->horizontalScrollBar(), "value", this);
    //anim_h->setStartValue(fr_x);
    anim_h->setDuration(300);
    anim_h->setEndValue(to_x);
    QParallelAnimationGroup *group = new QParallelAnimationGroup;
    group->addAnimation(anim_v);
    group->addAnimation(anim_h);
    group->start(QAbstractAnimation::DeleteWhenStopped);
}

void MapView::on_pb_antialiasing_toggled(bool checked)
{
    this->setRenderHint(QPainter::Antialiasing, checked);
}

void MapView::on_pb_select_mode_toggled(bool checked)
{
    this->setDragMode(!checked
                        ? QGraphicsView::RubberBandDrag
                        : QGraphicsView::ScrollHandDrag);
    this->setTransformationAnchor(!checked
                        ? QGraphicsView::AnchorUnderMouse
                        : QGraphicsView::AnchorViewCenter);
    this->setInteractive(!checked);
}

void MapView::on_pb_print_clicked()
{
    QPrinter printer;
    QPrintDialog dialog(&printer, this);
    if (dialog.exec() == QDialog::Accepted) {
        QPainter painter(&printer);
        this->render(&painter);
    }
}

void MapView::on_pb_map_export_clicked()
{
    QImage image(this->scene()->sceneRect().size().toSize(), QImage::Format_ARGB6666_Premultiplied);
    image.fill(Qt::transparent);
    QPainter painter(&image);
    if (ui->pb_antialiasing->isChecked())
        painter.setRenderHint(QPainter::Antialiasing);
    this->scene()->render(&painter);
    QString f_name = QString("export/map_%1.png").arg(QDateTime::currentDateTime().toString());
    image.save(f_name);
}

