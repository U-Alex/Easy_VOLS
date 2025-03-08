#ifndef OBJ_H
#define OBJ_H

enum class Idx {
    o_id, parr_id, parr_type, o_name, o_type,/* label,*/
    cabtype, cabcolor, lineidid, linecncn,
    param, VisMode
};

enum class ObjType {
    pix_map,
    o_pw_cont, o_locker, o_coup, o_polyline,
    o_label
};

#endif // OBJ_H
