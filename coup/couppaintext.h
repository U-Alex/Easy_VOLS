#ifndef COUPPAINTEXT_H
#define COUPPAINTEXT_H

#include <QFrame>
#include <QJsonDocument>

#include "config.h"

namespace Ui { class CoupPaintExt; }

class CoupPaintExt : public QFrame
{
    Q_OBJECT
public:
    explicit CoupPaintExt(Config *ref_conf, QWidget *parent = nullptr);
    ~CoupPaintExt();

public slots:
    void slotCoupPaintExt(uint, short, QJsonDocument);

private:
    Ui::CoupPaintExt *ui;
    Config           *conf = nullptr;

};

#endif // COUPPAINTEXT_H
