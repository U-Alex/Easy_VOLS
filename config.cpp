#include "config.h"


Config::Config()
{

    setting = new QSettings("vols_config", QSettings::IniFormat);

    QString _sep_ = "◀▶";

    rest_url = setting->value("rest_url").toString();
    map_f_name = setting->value("background_image").toString();
//    coup_stamp_name = setting->value("coup_stamp_image").toString();
    image_allocation_limit = setting->value("map/image_allocation_limit").toInt();

    str_to_map(setting->value("map/color_lo_co").toString().split(_sep_), &color_lo_co);
    str_to_map(setting->value("map/color_coup").toString().split(_sep_), &color_coup);

    lo_status_color = setting->value("map/lo_status_color").toString().split(_sep_);
    lo_status_list = setting->value("map/lo_status_list").toString().split(_sep_);

    str_to_map(setting->value("map/cab_color").toString().split(_sep_), &cab_color);
    str_to_map(setting->value("map/cab_width").toString().split(_sep_), &cab_width);
    str_to_map(setting->value("coup/ru_color_list").toString().split(_sep_), &ru_color_list);

    color_coup_ext = setting->value("coup/color_coup_ext").toString().split(_sep_);
    color_cross = setting->value("coup/color_cross").toString().split(_sep_);

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
