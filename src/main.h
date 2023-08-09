typedef enum {
    ACTION_FREE,
    ACTION_USED,
    ACTION_TOTAL,
    ACTION_PAGESIZE,
    ACTION_INVALID,
    ACTION_HELP,
    ACTION_VERSION
} action_type;

typedef enum {
    UNIT_BYTES,
    UNIT_MB,
    UNIT_MIB,
    UNIT_KB,
    UNIT_KIB
} memory_unit;