/*
*   utils.c
*/

#include "fs.h"
#include "utils.h"
#include "draw.h"
#include "screen.h"
#include "cache.h"
#include "i2c.h"
#include "payload.h"


u32 waitInput(void)
{
    u32 key,
        oldKey = HID_PAD;

    while(true)
    {
        key = HID_PAD;

        if(!key)
        {
            oldKey = 0;
            continue;
        }

        if(key == oldKey) continue;

        //Make sure the key is pressed
        u32 i;
        for(i = 0; i < 0x13000 && key == HID_PAD; i++);
        if(i == 0x13000) break;
    }

    return key;
}

void mcuReboot(void)
{
    clearScreens();

    //Ensure that all memory transfers have completed and that the data cache has been flushed
    flushEntireDCache();

    i2cWriteRegister(I2C_DEV_MCU, 0x20, 1 << 2);
    while(true);
}

void inputSequence(void)
{
    posY = drawString("If you would like to continue, press:", 10, posY, COLOR_WHITE);
    posY = drawString("Up, Down, Left, Right, B, A, START, SELECT", 10, posY, COLOR_WHITE);

    u32 unlockSequence[] = { BUTTON_UP, BUTTON_DOWN, BUTTON_LEFT, BUTTON_RIGHT, BUTTON_B, BUTTON_A, BUTTON_START, BUTTON_SELECT },
        sequenceSize = sizeof(unlockSequence) / sizeof(u32);

    for(u32 correctPresses = 0; correctPresses < sequenceSize; correctPresses++)
    {
        if(waitInput() != unlockSequence[correctPresses])
            shutdown(1, "Button sequence not entered correctly");
    }
}

void shutdown(u32 mode, const char *message)
{
    u32 pressed = 0;
	if(mode != 0)
    {
        posY = drawString(message, 10, posY + SPACING_Y, mode == 1 ? COLOR_RED : COLOR_GREEN);
        
    }
	
	drawString("Press any button to reboot arm9.bin", 10, posY, COLOR_WHITE);
    drawString("Press button START to Poweroff", 10, posY+10, COLOR_WHITE);
	pressed = waitInput();
	if(pressed == BUTTON_START)i2cWriteRegister(I2C_DEV_MCU, 0x20, 1 << 0);
    
	clearScreens();
	mountFs(true);
    Load_Payload();
}