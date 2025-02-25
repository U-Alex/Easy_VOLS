#include "coupmanager.h"
#include "ui_coupmanager.h"

#include <QPainter>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>

#include "b_logic/logger.h"

CoupManager::CoupManager(Config *ref_conf, UserSession *us, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CoupManager),
    conf(ref_conf),
    userSession(us)
{
    ui->setupUi(this);
    ui->spinBox_w_line->setValue(conf->link_v_slot_step);
//    time_last_update = QTime::currentTime();
}

CoupManager::~CoupManager()
{
    delete ui;
}

void CoupManager::slotNextCoup(uint c_id, QPoint pos)
{
    nextCoup(c_id);
    emit sigToMapCoup(pos);
}

void CoupManager::slotNextCoupExt(QStringList c_l, QStringList c_r)
{
    flood_guard[2] = true;
    cab_L = c_l; cab_R = c_r;
    fr_P_repaint(2);
}

void CoupManager::nextCoup(uint c_id)
{
//qDebug()<<time_last_update;
//QTime curr_time = QTime::currentTime();
//qDebug()<<curr_time;
//qDebug()<< time_last_update.msecsTo(curr_time);
//if (time_last_update.msecsTo(curr_time) < 2000)
//    qDebug()<<"flood";
//time_last_update = curr_time;

    if (std::all_of(flood_guard.begin(), flood_guard.end(), [](bool x){ return x; })) {
        flood_guard[2] = false;
        userSession->getDataPaint(c_id);
    }
    else {
        Logger::sendLog({"текущий запрос отменен, ожидание окончания предыдущей операции"}, "orange");
        return;
    }
    coup_id = c_id;
    if (coupPaint != nullptr) coupPaint->deleteLater();
    coupPaint = new CoupPaint(conf, coup_id, flood_guard, this);
    ui->gridLayout->addWidget(coupPaint, 0, 1);
    QObject::connect(coupPaint, &CoupPaint::sigNextCoup, this, &CoupManager::slotNextCoup);
    QObject::connect(coupPaint, &CoupPaint::sigNextCoupExt, this, &CoupManager::slotNextCoupExt);
    QObject::connect(userSession, &UserSession::sigCoupPaint, coupPaint, &CoupPaint::slotCoupPaint);
//qDebug()<<flood_guard;
}

void CoupManager::fr_P_repaint(short fr_pos)
{
    if ((fr_pos == 0 || fr_pos == 2) && ui->pb_L->isChecked()) {
        if (coupPaintExtL != nullptr) coupPaintExtL->deleteLater();
        coupPaintExtL = new CoupPaintExt(conf, coup_id, 0, flood_guard, this);
        ui->gridLayout->addWidget(coupPaintExtL, 0, 0);
//        QVector<int> cab_list = coupPaint->getCabList(0);
        QObject::connect(userSession, &UserSession::sigCoupPaintExt, coupPaintExtL, &CoupPaintExt::slotCoupPaintExt);
        QObject::connect(coupPaintExtL, &CoupPaintExt::sigShowHopExt, this, &CoupManager::slotShowHopExt);
        if (!cab_L.isEmpty()) {
            flood_guard[0] = false;
            userSession->getDataPaintExt(coup_id, cab_L, 0);
        } else
            flood_guard[0] = true;
    }
    if ((fr_pos == 1 || fr_pos == 2) && ui->pb_R->isChecked()) {
        if (coupPaintExtR != nullptr) coupPaintExtR->deleteLater();
        coupPaintExtR = new CoupPaintExt(conf, coup_id, 1, flood_guard, this);
        ui->gridLayout->addWidget(coupPaintExtR, 0, 2);
//        QVector<int> cab_list = coupPaint->getCabList(1);
        QObject::connect(userSession, &UserSession::sigCoupPaintExt, coupPaintExtR, &CoupPaintExt::slotCoupPaintExt);
        QObject::connect(coupPaintExtR, &CoupPaintExt::sigShowHopExt, this, &CoupManager::slotShowHopExt);
        if (!cab_R.isEmpty()) {
            flood_guard[1] = false;
            userSession->getDataPaintExt(coup_id, cab_R, 1);
        } else
            flood_guard[1] = true;
    }
}

void CoupManager::on_pb_L_toggled(bool checked)
{
    QPushButton *bp = static_cast<QPushButton*>(sender());
    if (coupPaint == nullptr) {
        bp->setChecked(0);
        return;
    }
    int frame_LR_width = conf->frame_LR_width;
    if (checked) {
        ui->frame_head_L->setFixedWidth(frame_LR_width + 20);
        ui->frame_head->move(ui->frame_head->pos() + QPoint(frame_LR_width, 0));
        ui->frame_head_R->move(ui->frame_head_R->pos() + QPoint(frame_LR_width, 0));
        ui->scrollArea->setFixedWidth(ui->scrollArea->width() + frame_LR_width);
        this->setFixedWidth(this->width() + frame_LR_width);
        fr_P_repaint(0);
        bp->setText(">>");
        this->move(this->x()-frame_LR_width, this->y());
    } else {
        ui->frame_head_L->setFixedWidth(20);
        ui->frame_head->move(ui->frame_head->pos() - QPoint(frame_LR_width, 0));
        ui->frame_head_R->move(ui->frame_head_R->pos() - QPoint(frame_LR_width, 0));
        ui->scrollArea->setFixedWidth(ui->scrollArea->width() - frame_LR_width);
        this->setFixedWidth(this->width() - frame_LR_width);
        coupPaintExtL->deleteLater();
        coupPaintExtL = nullptr;
        flood_guard[0] = true;
        bp->setText("<<");
        this->move(this->x()+frame_LR_width, this->y());
    }
}

void CoupManager::on_pb_R_toggled(bool checked)
{
    QPushButton *bp = static_cast<QPushButton*>(sender());
    if (coupPaint == nullptr) {
        bp->setChecked(0);
        return;
    }
    int frame_LR_width = conf->frame_LR_width;
    if (checked) {
        ui->frame_head_R->setFixedWidth(frame_LR_width + 36);
        ui->scrollArea->setFixedWidth(ui->scrollArea->width() + frame_LR_width);
        this->setFixedWidth(this->width() + frame_LR_width);
        fr_P_repaint(1);
        bp->setText("<<");
    } else {
        ui->frame_head_R->setFixedWidth(36);
        ui->scrollArea->setFixedWidth(ui->scrollArea->width() - frame_LR_width);
        this->setFixedWidth(this->width() - frame_LR_width);
        coupPaintExtR->deleteLater();
        coupPaintExtR = nullptr;
        flood_guard[1] = true;
        bp->setText(">>");
    }
}

void CoupManager::slotShowHopExt(uint c_p_id)
{
//    qDebug()<<"CoupManager::slotShowHopExt"<< c_p_id;
    userSession->getShowHop(c_p_id);
}

void CoupManager::on_spinBox_w_line_valueChanged(int arg1)
{
    conf->link_v_slot_step = arg1;
}

void CoupManager::on_pb_update_clicked()
{
    if (coup_id) nextCoup(coup_id);
}

void CoupManager::on_pb_export_clicked()
{
    if (coupPaint == nullptr) return;
//    QPalette palette1, palette2 = this->palette();
//    palette1.setColor(QPalette::Window, Qt::white);
//    palette1.setColor(QPalette::Button, Qt::white);
//    this->setPalette(palette1);

    QImage image(QSize(ui->scrollAreaWidgetContents->width(), ui->scrollAreaWidgetContents->height()), QImage::Format_ARGB6666_Premultiplied);
    image.fill(Qt::transparent);
    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing);
    ui->scrollAreaWidgetContents->render(&painter);
    QString f_name = QString("export/coup_%1").arg(coup_id);
    image.save(f_name + ".png");

//    this->setPalette(palette2);
}

void CoupManager::on_pb_print_clicked()
{
    if (coupPaint == nullptr) return;
    QPalette palette1, palette2 = this->palette();
    palette1.setColor(QPalette::Window, Qt::white);
    palette1.setColor(QPalette::Button, Qt::white);
//    coupPaint->setPalette(palette1);
    this->setPalette(palette1);

    QPrinter *printer = new QPrinter(QPrinter::HighResolution);
    printer->setPageSize (QPageSize::A4);
    printer->setOutputFormat(QPrinter::PdfFormat);
    printer->setResolution(1200);

    QPrintDialog printdialog(printer, this);
    if (printdialog.exec() == QDialog::Accepted) {
        QPainter painter;
        painter.begin(printer);
        auto xscale = printer->pageRect(QPrinter::Point).width() / double(ui->scrollAreaWidgetContents->width()) * 16.3;
        auto yscale = printer->pageRect(QPrinter::Point).height() / double(ui->scrollAreaWidgetContents->height()) * 16.3;
        double scale = qMin(xscale, yscale);
        painter.scale(scale, scale);
        ui->scrollAreaWidgetContents->render(&painter);
    }
//    coupPaint->setPalette(palette2);
    this->setPalette(palette2);
    delete printer;
}

