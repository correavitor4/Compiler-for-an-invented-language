#ifndef HASH_API_H
#define HASH_API_H

typedef struct {
    unsigned long (*djb2)(const char *str);
} HASH_API;

extern const HASH_API HASH;

#endif // HASH_API_H