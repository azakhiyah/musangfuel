#pragma once
#include <stdint.h>
#include "core/RuntimeStorage.h"

typedef enum {
    PRINT_TRANSACTION,
    PRINT_REPRINT,
    PRINT_SHIFTREPORT,
    PRINT_CURRENTSHIFT
} PrintType;

typedef struct {
    PrintType type;
    TransactionData trx;   // 🔥 langsung bawa data
    ShiftData shift;
} PrintJob;