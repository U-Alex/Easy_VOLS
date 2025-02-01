#include "mapview.h"
#include "ui_mapview.h"

#include <QScrollBar>
//#include <QSlider>
//#include <QMatrix>

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

void MapView::toggleAntialiasing()
{
//    this->setRenderHint(QPainter::Antialiasing, ui->antialiasButton->isChecked());
}
