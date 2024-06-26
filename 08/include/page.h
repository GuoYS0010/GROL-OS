#ifndef PAGE_H
#define PAGE_H
#include "types.h"
#include "platform.h"
#include <stddef.h>
#include "printf.h"

void *page_alloc(int npages);
void page_free(void *p);
void page_test();
void page_init(void);

#endif