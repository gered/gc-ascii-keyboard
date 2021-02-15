#include <stdio.h>
#include <stdlib.h>
#include <gccore.h>
#include <debug.h>

static void *xfb = NULL;
static GXRModeObj *rmode = NULL;

#if defined(HW_DOL)
#define SI_REG_BASE 0xCC006400
#elif defined(HW_RVL)
#define SI_REG_BASE 0xCD006400
#else
#error HW model unknown.
#endif

#define SIREG(n)               ((vu32*)(SI_REG_BASE + (n)))

#define SICOUTBUF(n)           (SIREG(0x00 + (n)*12))               /* SI Channel n Output Buffer (Joy-channel n Command) (4 bytes) */
#define SICINBUFH(i)           (SIREG(0x04 + (i)*12))               /* SI Channel n Input Buffer High (Joy-channel n Buttons 1) (4 bytes) */
#define SICINBUFL(i)           (SIREG(0x08 + (i)*12))               /* SI Channel n Input Buffer Low (Joy-channel n Buttons 2) (4 bytes) */
#define SIPOLL                 (SIREG(0x30))                        /* SI Poll Register (4 bytes) */
#define SICOMCSR               (SIREG(0x34))                        /* SI Communication Control Status Register (command) (4 bytes) */
#define SISR                   (SIREG(0x38))                        /* SI Status Register (4 bytes) */
#define SIIOBUF                (SIREG(0x80))                        /* SI I/O buffer (access by word) (128 bytes) */

#define PAD_ENABLEDMASK(chan)  (0x80000000 >> chan)



static void SI_AwaitPendingCommands(void) {
    while(*SICOMCSR & 0x1);
}

static int SI_DetectGCKeyboard(void) {
    SI_AwaitPendingCommands();

    u32 buf[2];
    for (int i = 0; i < 4; ++i) {
        SI_GetResponse(i, buf);
        SI_SetCommand(i, 0x00400300);
        SI_EnablePolling(PAD_ENABLEDMASK(i));
    }
    SI_AwaitPendingCommands();

    int keyboardChan = -1;

    for (int i = 0; i < 4; ++i) {
        u32 type = SI_DecodeType(SI_GetType(i));
        if (type == SI_GC_KEYBOARD)
            keyboardChan = i;
    }

    return keyboardChan;
}

static int SI_InitGCKeyboard(int chan) {
    if (chan == -1)
        return 0;

    u32 buf[2];

    SI_GetResponse(chan, buf);
    SI_SetCommand(chan, 0x00540000);
    SI_EnablePolling(PAD_ENABLEDMASK(chan));
    SI_TransferCommands();
    SI_AwaitPendingCommands();

    return 1;
}

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

    DEBUG_Init(GDBSTUB_DEVICE_USB,1);
    printf("Waiting for debugger ...\n");
    _break();
    printf("debugger connected ...\n");

    PAD_Init();

    int keyboardChan = SI_DetectGCKeyboard();
    printf("Keyboard located at chan %d\n", keyboardChan);

    int keyboardEnabled = SI_InitGCKeyboard(keyboardChan);
    if (keyboardEnabled)
        printf("keyboard initialized on chan %d\n", keyboardChan);

    printf("start of main loop ...\n");
    while(1) {
        if (keyboardEnabled) {
            u32 buf[2];
            if (SI_GetResponse(keyboardChan, buf)) {
                u8 key1 = buf[1] >> 24;
                u8 key2 = (buf[1] >> 16) & 0xff;
                u8 key3 = (buf[1] >> 8) & 0xff;
                if (key1 | key2 | key3)
                    printf("keyboard data - raw: 0x%08x 0x%08x - keys: 0x%02x, 0x%02x, 0x%02x\n",
                           buf[0], buf[1], key1, key2, key3);
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
        if (pressed & PAD_BUTTON_A) {
            _break();
            printf("A pressed\n");
        }

        VIDEO_WaitVSync();
    }

    return 0;
}
