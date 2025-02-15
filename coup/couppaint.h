#ifndef COUPPAINT_H
#define COUPPAINT_H

#include <QFrame>

#include "config.h"

namespace Ui { class CoupPaint; }

class CoupPaint : public QFrame
{
    Q_OBJECT
public:
    explicit CoupPaint(Config *ref_conf, QWidget *parent = nullptr);
    ~CoupPaint();

private:
    Ui::CoupPaint *ui;
    Config        *conf = nullptr;


};

#endif // COUPPAINT_H
