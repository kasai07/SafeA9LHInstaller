#include <stdio.h>
#include "payload.h"
#include "cache.h"
#include "fatfs/ff.h"
void Memcpy_Payload(void *dest, const void *src, u32 size)
{
    u8 *destc = (u8 *)dest;
    const u8 *srcc = (const u8 *)src;

    for(u32 i = 0; i < size; i++)
        destc[i] = srcc[i];
}

u32 Read_Payload(void *dest, const char *path)
{
    FIL file;
    u32 size;

    if(f_open(&file, path, FA_READ) == FR_OK)
    {
        unsigned int read;
        size = f_size(&file);
        f_read(&file, dest, size, &read);
        f_close(&file);
    }
    else size = 0;

    return size;
}

void Load_Payload(void)
{
	
	const unsigned char loader[160] = {
    0x02, 0x00, 0x00, 0xEA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xA0, 0xE1, 0x00, 0x00, 0xA0, 0xE1,
    0x00, 0xC0, 0x9F, 0xE5, 0x1C, 0xFF, 0x2F, 0xE1, 0x79, 0xFF, 0xFF, 0x24, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x10, 0xA0, 0xE3, 0x00, 0x00, 0xA0, 0xE3, 0x00, 0x20, 0x81, 0xE1, 0x5E, 0x2F, 0x07, 0xEE,
    0x20, 0x00, 0x80, 0xE2, 0x01, 0x0B, 0x50, 0xE3, 0xFA, 0xFF, 0xFF, 0x1A, 0x01, 0x11, 0x81, 0xE2,
    0x00, 0x00, 0x51, 0xE3, 0xF6, 0xFF, 0xFF, 0x1A, 0x9A, 0x1F, 0x07, 0xEE, 0x15, 0x1F, 0x07, 0xEE,
    0x1E, 0xFF, 0x2F, 0xE1, 0x00, 0x28, 0x0A, 0xD0, 0x05, 0x4B, 0x9C, 0x46, 0x60, 0x44, 0xFF, 0x22,
    0x19, 0x78, 0x12, 0x06, 0x9A, 0x18, 0x01, 0x33, 0x11, 0x70, 0x98, 0x42, 0xF7, 0xD1, 0x70, 0x47,
    0x00, 0x00, 0xF0, 0x24, 0x00, 0x00, 0x00, 0x00, 0x10, 0xB5, 0x04, 0x4B, 0x18, 0x68, 0xFF, 0xF7,
    0xE9, 0xFF, 0x00, 0xF0, 0x09, 0xF8, 0x02, 0x4B, 0x98, 0x47, 0x10, 0xBD, 0x04, 0xFF, 0xFF, 0x24,
    0x00, 0x00, 0xF0, 0x23, 0x00, 0x00, 0x00, 0x00, 0x78, 0x47, 0xC0, 0x46, 0xDF, 0xFF, 0xFF, 0xEA
	};
	
	
	u32 *const loaderAddress = (u32 *)0x24FFFF00;
    Memcpy_Payload(loaderAddress, loader, 160);
	
    loaderAddress[1] = Read_Payload((void *)0x24F00000, "/arm9.bin");
	
    flushDCacheRange(loaderAddress, 160);
    flushICacheRange(loaderAddress, 160);
    
	((void (*)())loaderAddress)();
	
}
