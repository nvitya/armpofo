// apsysif.h

#ifndef APSYSIF_APSYSIF_H_
#define APSYSIF_APSYSIF_H_

#define APSYS_SYS_LOAD_ADDR   0x24060000
#define APSYS_SYS_FLASH_ADDR     0x00000

struct TAppHeader
{
	char         name[16]; // name filled up with zeroes

	unsigned     imgtype;  // 1 = system, 2 = application
	unsigned     load_address;
	unsigned     content_length;
	unsigned     entry_point;

	unsigned     func_service;  // service function address
	unsigned     _reserved_8;
	unsigned     _reserved_9;
	unsigned     _reserved_10;

	unsigned     _reserved_11;
	unsigned     _reserved_12;
	unsigned     content_checksum;
	unsigned     header_checksum;
};

uint32_t apsys_content_checksum(void * adataptr, uint32_t adatalen);
uint32_t apsys_header_checksum(TAppHeader * aheader);

#endif /* APSYSIF_APSYSIF_H_ */
