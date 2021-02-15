# Gamecube ASCII Keyboard Controller Usage Example

This is a very bare-bones project that shows how to read the [Gamecube ASCII Keyboard Controller](https://nintendo.fandom.com/wiki/ASCII_Keyboard_Controller)
using devkitPPC and libogc (from [devkitPro](https://devkitpro.org/), of course)

Currently, libogc contains no support for reading keyboard input from this keyboard controller, and only somewhat 
"wonky" support for detecting it. Though, to be fair to the libogc authors, my feeling of "wonky support" for detecting
the presence of a keyboard controller could very well just be due to libogc's SI API being undocumented.

The **only** working example of any Gamecube code that could read this keyboard is located in the [gc-linux sources](https://github.com/DeltaResero/GC-Wii-Linux-Kernels/blob/GC-Wii-Linux-Kernel-3.15.y-EOL/drivers/input/si/gcn-si.c)
but, since it is a driver implemented in the Linux kernel, it basically re-implements the SI functionality that libogc
provides so it is of _somewhat_ limited direct use, but still useful to read anyway.

The only other pieces of documentation for this keyboard controller can be found on the 
[Yet Another Gamecube Documentation](http://hitmen.c02.at/files/yagcd/yagcd/index.html) website. Specifically, 
[section 9](http://hitmen.c02.at/files/yagcd/yagcd/chap9.html#sec9) and [section 5.8](http://hitmen.c02.at/files/yagcd/yagcd/chap5.html#sec5.8) 
are both useful. However, the explanations can be fairly light for the un-initiated (a.k.a. me).

Anyway!

The code in this repository is the result of bashing my head against this. It has **only** been tested on real 
hardware. More specifically, on my launch-day Nintendo Wii and keyboard controller model ASC-1901PO. I have not done 
any testing on Dolphin whatsoever.
