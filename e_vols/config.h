#ifndef CONFIG_H
#define CONFIG_H

#include <QSettings>
#include <QSize>

class Config                    //TODO  to Singleton (as logger)
{
public:
    explicit Config();

    QString                 rest_url;
    QString                 map_f_name;
    int                     image_allocation_limit;

    QMap<QString,QString>   color_coup, color_lo_co;
    QStringList             lo_status_color, lo_status_list;
    QMap<QString,QString>   cab_color, cab_width;

//coupManager
    int frame_LR_width =    300;
//    QString                 coup_stamp_name;

//coupPaint
    QMap<QString,QString>    ru_color_list;

    //offset
    int but_V_offset =                     20;
    QList<int> but_ext_coup_H_offset =    {0,1230};
    QList<int> but_cab_type_H_offset =    {0,1000};
    QList<int> but_cab_up_H_offset =      {270,980};
    QList<int> but_cab_lr_H_offset =      {290,960};
    QList<int> but_fiber_H_offset =       {150,1100};
    QList<int> but_cross_port_H_offset =  {200,1050};

    QList<int> lab_ext_coup_H_offset =    {40,990};
    QList<int> lab_color_fiber_H_offset = {30,1120};
    QList<int> lab_color_mod_H_offset =   {10,1240};
    QList<int> lab_cross_H_offset =       {220,920};

    //size
    QSize but_ext_coup_size =             {40,20};
    QSize but_cab_type_size =             {270,20};
    QSize but_cab_move_size =             {20,20};
    QSize but_fiber_size =                {20,20};

    QSize lab_ext_coup_size =             {240,18};
    QSize lab_color_fiber_size =          {118,18};
    QSize lab_color_mod_size =            {18,18};
    QSize lab_cross_size =                {130,18};

    //link offset
    int link_v_slot_step =                 6;

    QList<int> link_0_offset =            {210,-210};
    QList<int> link_1_offset =            {210,-210};
    QList<int> link_2_offset =            {230,-230};
    QList<int> link_3_offset =            {50,-50};

//coupPaintExt
    //offset
    QList<QString> color_coup_ext;
    QList<QString> color_cross;

    QList<int> but_hop_H_offset =          {10,260};
    QList<int> but_parrent_H_offset =      {80,0};

    QList<int> lab_up_info_H_offset =      {80,10};
    QList<int> lab_color_cr1_H_offset =    {40,236};
    QList<int> lab_color_cr2_H_offset =    {56,220};

    //size
    QSize but_hop_size =                   {20,20};
    QSize but_parrent_size =               {210,20};

    QSize lab_up_info_size =               {200,18};
    QSize lab_color_cr_size =              {14,18};

private:
    QSettings *setting;

private slots:
    void str_to_map(QStringList, QMap<QString,QString> *);

};

#endif // CONFIG_H
