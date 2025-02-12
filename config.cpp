#include "config.h"


Config::Config()
{

    setting = new QSettings("vols_config", QSettings::IniFormat);

    QString _sep_ = "◀▶";

    map_f_name = setting->value("background_image").toString();

    str_to_map(setting->value("map/color_lo_co").toString().split(_sep_), &color_lo_co);
    str_to_map(setting->value("map/color_coup").toString().split(_sep_), &color_coup);

    lo_status_color = setting->value("map/lo_status_color").toString().split(_sep_);
    lo_status_list = setting->value("map/lo_status_list").toString().split(_sep_);

    str_to_map(setting->value("map/cab_color").toString().split(_sep_), &cab_color);
    str_to_map(setting->value("map/cab_width").toString().split(_sep_), &cab_width);

    setting->deleteLater();
}

void Config::str_to_map(QStringList src, QMap<QString,QString> *dst)
{
    QStringList str;
    foreach (QString str_str, src) {
        str = str_str.split("=");
        *dst->insert(str.at(0), str.at(1));
//        qDebug() << dst->keys() << dst->values();
    }
}
