#ifndef COUPPAINTEXT_H
#define COUPPAINTEXT_H

#include <QFrame>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "config.h"

namespace Ui { class CoupPaintExt; }

class CoupPaintExt : public QFrame
{
    Q_OBJECT
public:
    explicit CoupPaintExt(Config *ref_conf, uint c_id, short _fr_pos, QVarLengthArray<bool>& fg, QWidget *parent = nullptr);
    ~CoupPaintExt();

public slots:
    void slotCoupPaintExt(uint, short, QJsonDocument);

signals:
    void sigShowHopExt(uint c_p_id);

private:
    Ui::CoupPaintExt *ui;
    Config           *conf = nullptr;
    uint              coup_id = 0;
    short             fr_pos;
    QVarLengthArray<bool>& flood_guard;

private slots:
    void but_hop_clicked();
    void but_parr_clicked();

};

#endif // COUPPAINTEXT_H
