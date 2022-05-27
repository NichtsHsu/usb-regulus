#ifndef HIDRD_CONVERT_H
#define HIDRD_CONVERT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

typedef enum hidrd_error
{
    HIDRD_SUCCESS = 0,
    HIDRD_UNKNOWN_INPUT_FORMAT = 1,
    HIDRD_UNKNOWN_OUTPUT_FORMAT = 2,
    HIDRD_FORMAT_NOT_SUPPORT = 3,
    HIDRD_FMT_LIB_INIT_FAILED = 4,
    HIDRD_OPEN_STEAM_FAILED = 5,
    HIDRD_READ_STEAM_FAILED = 6,
    HIDRD_WRITE_STEAM_FAILED = 7,
    HIDRD_CLOSE_STEAM_FAILED = 8,
} hidrd_error;

hidrd_error hidrd_convert(const char *inputData,
                          size_t inputLen,
                          char **outputData,
                          size_t *outputLen);

#ifdef __cplusplus
}
#endif
#endif // HIDRD_CONVERT_H
