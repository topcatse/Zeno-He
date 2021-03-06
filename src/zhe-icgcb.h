#ifndef ZHE_ICGCB_H
#define ZHE_ICGCB_H

#include <stdint.h>

typedef uint16_t uripos_t; /* FIXME: the usual */

struct icgcb_hdr {
    uripos_t size;      /* includes header, multiple of UNIT unless allocated; sentinel is exception and has size = 0 */
    uripos_t ref;       /* URIPOS_INVALID if free, else whatever the user puts in */
    unsigned char data[];
};

struct icgcb {
    uripos_t size;
    uripos_t pad;
    uripos_t freespace; /* total free space, that is, buf size - allocated (incl. headers) - 1 header */
    uripos_t firstfree; /* index in e[], first free block, GC starts here */
    uripos_t openspace; /* index in e[], nothing to the end of the buffer, allocations happen here */
    uripos_t sentinel;  /* index in e[] of sentinel at the end of memory */
    union {
        struct icgcb_hdr e; /* really e[] ... C is so overrated ... */
        char buf;       /* really buf[] ... */
    } u;
};

enum icgcb_alloc_result {
    IAR_OK,
    IAR_AGAIN,
    IAR_NOSPACE
};

void zhe_icgcb_init(struct icgcb * const b, uripos_t size);
void zhe_icgcb_free(struct icgcb * const b, void * const ptr);
enum icgcb_alloc_result zhe_icgcb_alloc(void ** const ptr, struct icgcb * const b, uripos_t size, uripos_t ref);
void zhe_icgcb_gc(struct icgcb * const b, void (*move_cb)(uripos_t ref, void *newptr, void *arg), void *arg);
uripos_t zhe_icgcb_getsize(struct icgcb const * const b, const void *ptr);

#endif
