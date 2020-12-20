// flasher.h

#ifndef SRC_FLASHER_H_
#define SRC_FLASHER_H_

#include "stdint.h"

bool flasher_copy(void * memaddr, uint32_t flashaddr, uint32_t len, void * tempbuf_4k_addr);

#endif /* SRC_FLASHER_H_ */
