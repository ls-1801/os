#ifndef _SYSTEM_MMU_H_
#define _SYSTEM_MMU_H_

#include <common.h>

void init_mmu();
const char *get_section(uint32_t ptr);

#endif /* _SYSTEM_MMU_H_ */