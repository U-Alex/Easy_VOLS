#ifndef MAPVIEW_H
#define MAPVIEW_H

#include <QWheelEvent>
#include <QGraphicsView>

namespace Ui { class MapView; }

class MapView : public QGraphicsView
{
    Q_OBJECT

public:
    explicit MapView(QWidget *parent);
    ~MapView();

public slots:
    void slotCoupOnCenter(/*uint, */QPoint);

protected:
    void wheelEvent(QWheelEvent *) override;

private:
    Ui::MapView *ui;

private slots:
    void setupMatrix();
    void on_pb_antialiasing_toggled(bool checked);
    void on_pb_select_mode_toggled(bool checked);
    void on_pb_print_clicked();
    void on_pb_map_export_clicked();
};

#endif // MAPVIEW_H
