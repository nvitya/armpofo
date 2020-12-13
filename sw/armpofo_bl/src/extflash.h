/*
 * extflash.h
 *
 *  Created on: 11 Dec 2020
 *      Author: vitya
 */

#ifndef SRC_EXTFLASH_H_
#define SRC_EXTFLASH_H_

#include "hwqspi.h"
#include "qspiflash.h"

class TExtFlash : public TQspiFlash
{
private:
	typedef TQspiFlash super;

public:
	virtual ~TExtFlash() { }

	void Init();
};

extern TExtFlash g_extflash;

#endif /* SRC_EXTFLASH_H_ */
