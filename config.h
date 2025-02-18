#ifndef CONFIG_H
#define CONFIG_H

#include <QSettings>
#include <QSize>

class Config
{
public:
    explicit Config();

    QString                 map_f_name;
    QMap<QString,QString>   color_coup, color_lo_co;
    QStringList             lo_status_color, lo_status_list;
    QMap<QString,QString>   cab_color, cab_width;






//coupManager
    int frame_LR_width =    260;


//coupPaint
    QMap<QString,QString>    ru_color_list;

    //offset
    int but_V_offset =                     20;
    QVector<int> but_ext_coup_H_offset =  {0,1230};
    QList<int> but_cab_type_H_offset =    {0,1000};
    QList<int> but_cab_up_H_offset =      {270,980};
    QList<int> but_cab_lr_H_offset =      {290,960};
    QList<int> but_fiber_H_offset =       {150,1100};
    QList<int> but_cross_port_H_offset =  {200,1050};

    QList<int> lab_ext_coup_H_offset =    {40,1020};
    QList<int> lab_color_fiber_H_offset = {30,1120};
    QList<int> lab_color_mod_H_offset =   {10,1240};
    QList<int> lab_cross_H_offset =       {220,970};

    //size
    QSize but_ext_coup_size =             {40,20};
    QSize but_cab_type_size =             {270,20};
    QSize but_cab_move_size =             {20,20};
    QSize but_fiber_size =                {20,20};

    QSize lab_ext_coup_size =             {210,18};
    QSize lab_color_fiber_size =          {118,18};
    QSize lab_color_mod_size =            {18,18};
    QSize lab_cross_size =                {80,18};

    //link offset
    int link_v_slot_step =                 6;

    QList<int> link_0_offset =            {160,-160};
    QList<int> link_1_offset =            {300,-300};
    QList<int> link_2_offset =            {170,-170};
    QList<int> link_3_offset =            {50,-50};




private:
    QSettings *setting;

private slots:
    void str_to_map(QStringList, QMap<QString,QString> *);


};

#endif // CONFIG_H
