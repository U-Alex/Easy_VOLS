#ifndef CONFIG_H
#define CONFIG_H

#include <QSettings>

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








private:
    QSettings *setting;

private slots:
    void str_to_map(QStringList, QMap<QString,QString> *);


};

#endif // CONFIG_H
