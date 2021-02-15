#include <stdio.h>
#include <stdlib.h>
#include <gccore.h>
#include <debug.h>

#include "gckeybrd.h"

static void *xfb = NULL;
static GXRModeObj *rmode = NULL;

int main(int argc, char **argv) {

    VIDEO_Init();

    rmode = VIDEO_GetPreferredMode(NULL);
    xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));

    console_init(xfb,20,20,rmode->fbWidth,rmode->xfbHeight,rmode->fbWidth*VI_DISPLAY_PIX_SZ);

    VIDEO_Configure(rmode);
    VIDEO_SetNextFramebuffer(xfb);
    VIDEO_SetBlack(FALSE);
    VIDEO_Flush();
    VIDEO_WaitVSync();
    if(rmode->viTVMode&VI_NON_INTERLACE) VIDEO_WaitVSync();

    printf("\n\nHello ...\n");

    //DEBUG_Init(GDBSTUB_DEVICE_USB,1);
    //printf("Waiting for debugger ...\n");
    //_break();
    //printf("debugger connected ...\n");

    PAD_Init();

    int keyboardChan = GCKB_Detect();
    printf("GC Keyboard located at chan %d\n", keyboardChan);

    int keyboardEnabled = GCKB_Init(keyboardChan);
    if (keyboardEnabled)
        printf("GC Keyboard initialized on chan %d\n", keyboardChan);

    printf("Start of main loop.\nPress Start on the controller plugged into slot 1 to exit ...\n");
    while(1) {
        if (keyboardEnabled) {
            u8 keys[3];
            if (GCKB_ReadKeys(keyboardChan, keys)) {
                if (keys[0] | keys[1] | keys[2])
                    printf("keys pressed: 0x%02x, 0x%02x, 0x%02x\n", keys[0], keys[1], keys[2]);
            }
        }

        PAD_ScanPads();
        u32 pressed = PAD_ButtonsDown(0);
        if (pressed)
            printf("PAD 0 buttons pressed: 0x%08x\n", pressed);

        if ( pressed & PAD_BUTTON_START ) {
            printf("exiting ...\n");
            exit(0);
        }

        VIDEO_WaitVSync();
    }

    return 0;
}
