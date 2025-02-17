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
    //offset
    int but_V_offset =                   20;
    QVector<int> but_ext_coup_H_offset = {0,1230};

    QVector<int> lab_ext_coup_H_offset = {40,1020};

    //size
    QSize but_ext_coup_size =            {40,20};

    QSize lab_ext_coup_size =            {210,18};


    //link offset


private:
    QSettings *setting;

private slots:
    void str_to_map(QStringList, QMap<QString,QString> *);


};

#endif // CONFIG_H
