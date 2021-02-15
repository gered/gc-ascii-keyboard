#ifndef GCKEYBRD_H_INCLUDED
#define GCKEYBRD_H_INCLUDED

#include <gctypes.h>

/**
 * Attempts to detect the presence of a GC Keyboard Controller connected to any of the controller ports.
 * Returns the SI channel of the found keyboard controller, or -1 if one could not be found.
 */
int GCKB_Detect(void);

/**
 * Initializes the keyboard controller, previously detected on the given SI channel. This must be called before
 * key press information can be read. Returns 1 on success, 0 on failure.
 */
int GCKB_Init(int chan);

/**
 * Reads current key press information from the previously initialized keyboard controller, located on the given
 * SI channel. Returns 1 if key press data has been returned, or 0 on failure.
 *
 * The pressedKeys buffer passed in should be large enough to hold 3 bytes. Each byte will correspond to one key
 * pressed. The keyboard controller can only recognize 3 simultaneous key presses at a time (and, depending on the
 * specific keys, probably only 2 at a time). If too many keys are held down, all of the values returned will be 0x02.
 * A value of 0x00 indicates no key press.
 */
int GCKB_ReadKeys(int chan, u8 *pressedKeys);

#endif
