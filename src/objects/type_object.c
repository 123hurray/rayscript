#include <type_object.h>

type_object base_type_object = {
    INIT_HEADER(&base_type_object),
    "type",
    default_hash,
    default_equals,
    DEFAULT_BIN_OPS,
    default_str,
    NULL, 
    default_destruct,
};


