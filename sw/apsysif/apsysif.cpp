/*
 * apsysif.cpp
 *
 *  Created on: 13 Dec 2020
 *      Author: vitya
 */


#include "stdint.h"
#include "apsysif.h"

uint32_t apsys_content_checksum(void * adataptr, uint32_t adatalen)
{
	int32_t remaining = adatalen;
	uint32_t * dp = (uint32_t *)adataptr;
	uint32_t csum = 0;

	while (remaining > 3)
	{
		csum += *dp++;
		remaining -= 4;
	}

	if (1 == remaining)
	{
		csum += (*dp & 0x000000FF);
	}
	else if (2 == remaining)
	{
		csum += (*dp & 0x0000FFFF);
	}
	else if (3 == remaining)
	{
		csum += (*dp & 0x00FFFFFF);
	}

	return(0 - csum);
}

uint32_t apsys_header_checksum(TAppHeader * aheader)
{
	uint32_t remaining;
	uint32_t * dp;
	uint32_t csum;

	remaining = sizeof(TAppHeader);
	dp = (uint32_t *)aheader;
	csum = 0;

	while (remaining > 0)
	{
		csum += *dp++;
		remaining -= 4;
	}

	csum -= aheader->header_checksum; // remove the old checksum

	return (0 - csum);
}
