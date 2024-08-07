#ifndef CONSTANTS_H
#define CONSTANTS_H


typedef enum ErrorCode {
    ERR_NONE = 0x00,
    ERR_NULL_PTR = 0x01,
    ERR_MALLOC_FAILED = 0x02,
    ERR_INVALID_INDEX = 0x03,
    ERR_LIST_EMPTY = 0x04,
    ERR_INVALID_HEAD_PTR = 0x05,
    ERR_INVALID_ARGS = 0x06,
    ERR_UNKNOWN = 0xFF
} ErrorCode;


#endif // CONSTANTS_H