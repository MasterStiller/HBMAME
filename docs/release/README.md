What is HBMAME?
===============

HBMAME stands for Home Brew MAME. It runs on Windows 7 SP1 or later.

HBMAME has 2 purposes:
Firstly is to preserve homebrew games written for arcade machines or arcade emulators.
Secondly is to include worthwhile hacks of arcade games.
All the documentation of the hardware can be found in the MAME source.

How to compile
==============

```
make PTR64=1 TARGET=hbmame SYMBOLS=0 NO_SYMBOLS=1 DEPRECATED=0
```

or

```
make PTR64=1 TARGET=hbmame SYMBOLS=0 NO_SYMBOLS=1 DEPRECATED=0 OSD=winui
```

For HBMAMEUI, you must use GCC 10.1 - later versions will compile but crash at start.


Where can I find out more?
==========================

* [Official HBMAME site](http://hbmame.1emulation.com/)
* [Forum](http://www.mameworld.info/ubbthreads/postlist.php?Cat=&Board=misfitmame) (bug reports go here too)


Licensing Information
=====================

Information about the MAME content can be found at https://github.com/mamedev/mame/blob/master/README.md

Information about the MAME license can be found in COPYING

Information about the WINUI portion can be found at https://github.com/Robbbert/mameui/blob/master/docs/winui_license.txt
