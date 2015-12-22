// license:BSD-3-Clause
// copyright-holders:Mike Coates
/*************************************************************************

  Rainbow Islands C-Chip Protection

  2002-Feb rewritten by Robert Gallagher

    - A replacement romset was coded, then inserted into a Rainbow Islands Extra PCB.
      This replacement would break into the gameplay, and begin dumping all possible data from
      the games C-chip to the display. The data was collected, recorded (photos) then added
      to this driver
    - C-chip data are all handled internally with arrays (no longer using JB1-F89 from Jumping)
    - More accurate emulation of C-chip (allowing writes, only returning data when requested...)
    - Aspects of the C-chip are more accurately 'simulated' by dynamically updating data, rather
      than relying on hard-coded data from the Jumping bootleg
    - GOAL IN Y-coordinates are randomized, from a collected set of possibles, similar to a real machine
    - None of the data present has been 'made up', everything has been verified with real hardware
    - Many thanks to Tormod Tjaberg for dumping the Rainbow Islands Extra c-chip data
      making RIE fully playable


  BANK 0  000 - 002  set by the 68000 during initialization
          003 - 006  joystick and other input bits
          008        coin counters and lock-out
          00D        round number for world data request
          01D        internal use
  BANK 1  000        internal use
          001 - 002  round height (LSB first)
          004 - 0ff  unknown data
          100        request round data
          101 - 140  walking on the rainbow offsets
          141        round number for round data request
          142 - 145  round address (MSB first)
          148        boss or secret room flag
          149        request goal-in data
          14A - 155  goal-in data
  BANK 2  000 - 01f  rainbow dissolve graphics
          020 - 03f  rainbow smashing graphics
          040 - 061  unused data
  BANK 4  002 - XXX  monster animation sequences /
                     starts with directory of 2-byte offsets
  BANK 5  000        request world data
          002 - XXX  monster property records /
                     starts with directory of 2-byte offsets
  BANK 6  002 - 141  unknown, has $20 bytes per world
  BANK 7  002 - XXX  monster to Bubble Bobble sprite mapping


*************************************************************************/

#include "emu.h"
#include "includes/rbisland.h"


/*************************************
 *
 * C-Chip bank data
 *
 *************************************/

struct CBANK
{
	const UINT8* data;
	size_t size;
};

static const UINT8 CROM_BANK1[] =
{
	0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
	0x01, 0x01, 0x01, 0x02, 0x0A, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
	0x01, 0x01, 0x01, 0x02, 0x0A, 0x0C, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
	0x01, 0x01, 0x01, 0x02, 0x0A, 0x0B, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
	0x01, 0x01, 0x01, 0x02, 0x07, 0x08, 0x09, 0x01, 0x01, 0x01, 0x01, 0x01,
	0x01, 0x01, 0x01, 0x02, 0x07, 0x08, 0x08, 0x01, 0x01, 0x01, 0x01, 0x01,
	0x01, 0x01, 0x01, 0x02, 0x07, 0x08, 0x08, 0x09, 0x01, 0x01, 0x01, 0x01,
	0x01, 0x01, 0x01, 0x02, 0x07, 0x08, 0x08, 0x08, 0x01, 0x01, 0x01, 0x01,
	0x01, 0x01, 0x01, 0x02, 0x03, 0x04, 0x04, 0x04, 0x05, 0x01, 0x01, 0x01,
	0x01, 0x01, 0x01, 0x02, 0x03, 0x04, 0x04, 0x04, 0x04, 0x01, 0x01, 0x01,
	0x01, 0x01, 0x01, 0x02, 0x03, 0x04, 0x04, 0x04, 0x04, 0x05, 0x01, 0x01,
	0x01, 0x01, 0x01, 0x02, 0x03, 0x04, 0x04, 0x04, 0x04, 0x04, 0x01, 0x01,
	0x01, 0x01, 0x01, 0x02, 0x03, 0x04, 0x04, 0x04, 0x04, 0x04, 0x05, 0x01,
	0x01, 0x01, 0x01, 0x02, 0x03, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x01,
	0x01, 0x01, 0x01, 0x02, 0x03, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x05,
	0x01, 0x01, 0x01, 0x02, 0x03, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
	0x01, 0x01, 0x01, 0x02, 0x03, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
	0x05, 0x01, 0x01, 0x02, 0x03, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
	0x04, 0x01, 0x01, 0x02, 0x03, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
	0x04, 0x05, 0x01, 0x02, 0x03, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
	0x04, 0x04, 0x04, 0x02, 0x03, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
	0x04, 0x04, 0x04, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x06,
	0x08, 0x08, 0x0A, 0x0C, 0x0C, 0x0E, 0x0E, 0x10, 0x12, 0x14, 0x16, 0x18,
	0x1A, 0x1C, 0x1E, 0x20, 0x22, 0x24, 0x26, 0x28, 0x2A, 0x2C, 0x2E, 0x30,
	0x32, 0x58, 0x5A, 0x5C, 0x5E, 0x60, 0x62, 0x64, 0x66, 0x68, 0x6A, 0x6C,
	0x6E, 0x70, 0x72, 0x74, 0x76, 0x78, 0x7A, 0x7C, 0x7E, 0x7E, 0x7E, 0x80,
	0x82, 0x86, 0x86, 0x8A, 0x8A, 0x8A, 0x8A, 0x8A, 0x8A
};

static const UINT8 CROM_BANK2[] =
{
	0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13,
	0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
	0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B,
	0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
	0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 0x40, 0x41, 0x42, 0x43,
	0x44, 0x45, 0x46, 0x47, 0x00, 0x00, 0x00, 0x00, 0x02, 0x02, 0x02, 0x02,
	0x04, 0x04, 0x04, 0x04, 0x06, 0x06, 0x06, 0x06, 0x10, 0x10, 0x12, 0x12,
	0x14, 0x14, 0x16, 0x20, 0x22, 0x16, 0x20, 0x22, 0x16, 0x16, 0x20, 0x20,
	0x22, 0x22
};

static const UINT8 CROM_BANK4_WORLD0[] =
{
	0x28, 0x00, 0x2C, 0x00, 0x30, 0x00, 0x34, 0x00, 0x38, 0x00, 0x3C, 0x00,
	0x40, 0x00, 0x44, 0x00, 0x48, 0x00, 0x54, 0x00, 0x58, 0x00, 0x5C, 0x00,
	0x60, 0x00, 0x68, 0x00, 0x70, 0x00, 0x74, 0x00, 0x78, 0x00, 0x7C, 0x00,
	0x80, 0x00, 0x84, 0x00, 0x32, 0x00, 0x34, 0x00, 0x56, 0x00, 0x58, 0x00,
	0x98, 0x00, 0x96, 0x00, 0x7A, 0x00, 0x7C, 0x00, 0x56, 0x00, 0x58, 0x00,
	0x98, 0x00, 0x96, 0x00, 0x12, 0x01, 0x13, 0x01, 0x1C, 0x01, 0x1D, 0x01,
	0x29, 0x01, 0x28, 0x01, 0x26, 0x01, 0x27, 0x01, 0x2A, 0x01, 0x2B, 0x01,
	0xFA, 0x00, 0xFC, 0x00, 0xFE, 0x00, 0x00, 0x01, 0x02, 0x10, 0x00, 0x10,
	0xD6, 0x00, 0xD8, 0x00, 0xD2, 0x00, 0xD4, 0x00, 0xDE, 0x00, 0xE0, 0x00,
	0xDA, 0x00, 0xDC, 0x00, 0xF4, 0x00, 0xF2, 0x00, 0x36, 0x01, 0x37, 0x01,
	0x38, 0x01, 0x39, 0x01, 0x3B, 0x01, 0x3A, 0x01, 0x34, 0x01, 0x35, 0x01,
	0xC9, 0x02, 0xC8, 0x02
};

static const UINT8 CROM_BANK4_WORLD1[] =
{
	0x24, 0x00, 0x28, 0x00, 0x2C, 0x00, 0x30, 0x00, 0x34, 0x00, 0x38, 0x00,
	0x3C, 0x00, 0x42, 0x00, 0x48, 0x00, 0x4C, 0x00, 0x50, 0x00, 0x54, 0x00,
	0x58, 0x00, 0x5A, 0x00, 0x5C, 0x00, 0x60, 0x00, 0x62, 0x00, 0x64, 0x00,
	0xAA, 0x01, 0xAC, 0x01, 0xCE, 0x01, 0xD0, 0x01, 0x6E, 0x02, 0x70, 0x02,
	0x62, 0x01, 0x64, 0x01, 0x86, 0x01, 0x88, 0x01, 0x6A, 0x02, 0x6C, 0x02,
	0x02, 0x02, 0x04, 0x02, 0x06, 0x02, 0x08, 0x02, 0x0A, 0x02, 0x0C, 0x02,
	0x82, 0x02, 0x84, 0x02, 0x0E, 0x02, 0x10, 0x02, 0x22, 0x02, 0x24, 0x02,
	0x86, 0x02, 0x88, 0x02, 0x26, 0x02, 0x2E, 0x02, 0x8A, 0x02, 0x8C, 0x02,
	0x28, 0x10, 0x28, 0x10, 0x8E, 0x02, 0x90, 0x02
};

static const UINT8 CROM_BANK4_WORLD2[] =
{
	0x2A, 0x00, 0x32, 0x00, 0x3A, 0x00, 0x3E, 0x00, 0x46, 0x00, 0x4E, 0x00,
	0x52, 0x00, 0x56, 0x00, 0x5A, 0x00, 0x5E, 0x00, 0x60, 0x00, 0x62, 0x00,
	0x66, 0x00, 0x68, 0x00, 0x6A, 0x00, 0x6E, 0x00, 0x76, 0x00, 0x7E, 0x00,
	0x82, 0x00, 0x86, 0x00, 0x8A, 0x00, 0x01, 0x03, 0x03, 0x03, 0x05, 0x03,
	0x07, 0x03, 0x01, 0x03, 0x03, 0x03, 0x05, 0x03, 0x07, 0x03, 0x23, 0x03,
	0x21, 0x03, 0x25, 0x03, 0x27, 0x03, 0x29, 0x03, 0x2B, 0x03, 0x25, 0x03,
	0x27, 0x03, 0x29, 0x03, 0x2B, 0x03, 0x47, 0x03, 0x23, 0x03, 0x61, 0x03,
	0x63, 0x03, 0x61, 0x03, 0x63, 0x03, 0x67, 0x03, 0x65, 0x03, 0x49, 0x03,
	0x49, 0x03, 0x67, 0x03, 0x65, 0x03, 0x69, 0x03, 0x69, 0x03, 0xC9, 0x03,
	0xC7, 0x03, 0x85, 0x03, 0x87, 0x03, 0x89, 0x03, 0x8B, 0x03, 0x85, 0x03,
	0x87, 0x03, 0x89, 0x03, 0x8B, 0x03, 0xC9, 0x03, 0xC7, 0x03, 0xAB, 0x03,
	0xAD, 0x03, 0xAF, 0x03, 0xC1, 0x03, 0xC5, 0x03, 0xC3, 0x03
};

static const UINT8 CROM_BANK4_WORLD3[] =
{
	0x30, 0x00, 0x34, 0x00, 0x38, 0x00, 0x3C, 0x00, 0x40, 0x00, 0x44, 0x00,
	0x48, 0x00, 0x4A, 0x00, 0x4C, 0x00, 0x50, 0x00, 0x54, 0x00, 0x58, 0x00,
	0x5C, 0x00, 0x5E, 0x00, 0x60, 0x00, 0x64, 0x00, 0x68, 0x00, 0x6C, 0x00,
	0x70, 0x00, 0x7A, 0x00, 0x84, 0x00, 0x88, 0x00, 0xA2, 0x00, 0xBC, 0x00,
	0x51, 0x04, 0x53, 0x04, 0x51, 0x04, 0x53, 0x04, 0x51, 0x04, 0x51, 0x04,
	0x55, 0x04, 0x57, 0x04, 0x55, 0x04, 0x57, 0x04, 0x55, 0x04, 0x55, 0x04,
	0x59, 0x04, 0x59, 0x04, 0x59, 0x04, 0x9F, 0x04, 0x5D, 0x04, 0x5B, 0x04,
	0x5D, 0x04, 0x5B, 0x04, 0x95, 0x04, 0x93, 0x04, 0x5F, 0x04, 0x5F, 0x04,
	0x99, 0x04, 0x97, 0x04, 0x75, 0x04, 0x77, 0x04, 0x7F, 0x04, 0x91, 0x04,
	0x9D, 0x04, 0x9B, 0x04, 0xB1, 0x04, 0xB3, 0x04, 0xB5, 0x04, 0xB7, 0x04,
	0xB9, 0x04, 0xB1, 0x04, 0xB3, 0x04, 0xB5, 0x04, 0xB7, 0x04, 0xB9, 0x04,
	0xBD, 0x04, 0xBB, 0x04, 0xD6, 0x04, 0xD7, 0x04, 0xD8, 0x04, 0xD9, 0x04,
	0xDA, 0x04, 0xDB, 0x04, 0xDC, 0x04, 0xDD, 0x04, 0xDE, 0x04, 0xDF, 0x04,
	0xE0, 0x04, 0xE1, 0x04, 0xE2, 0x04, 0xD6, 0x04, 0xD7, 0x04, 0xD8, 0x04,
	0xD9, 0x04, 0xDA, 0x04, 0xDB, 0x04, 0xDC, 0x04, 0xDD, 0x04, 0xDE, 0x04,
	0xDF, 0x04, 0xE0, 0x04, 0xE1, 0x04, 0xE2, 0x04, 0x12, 0x05, 0x11, 0x05
};

static const UINT8 CROM_BANK4_WORLD4[] =
{
	0x24, 0x00, 0x24, 0x00, 0x34, 0x00, 0x38, 0x00, 0x38, 0x00, 0x60, 0x00,
	0x64, 0x00, 0x64, 0x00, 0x7C, 0x00, 0x80, 0x00, 0x80, 0x00, 0x90, 0x00,
	0x94, 0x00, 0x94, 0x00, 0x96, 0x00, 0x9A, 0x00, 0x9A, 0x00, 0xB2, 0x00,
	0x56, 0x05, 0x57, 0x05, 0x58, 0x05, 0x59, 0x05, 0x5A, 0x05, 0x5B, 0x05,
	0x5C, 0x05, 0x5D, 0x05, 0x87, 0x05, 0x86, 0x05, 0x5E, 0x05, 0x5F, 0x05,
	0x60, 0x05, 0x61, 0x05, 0x62, 0x05, 0x63, 0x05, 0x5E, 0x05, 0x5F, 0x05,
	0x60, 0x05, 0x61, 0x05, 0x62, 0x05, 0x63, 0x05, 0x64, 0x05, 0x65, 0x05,
	0x66, 0x05, 0x67, 0x05, 0x66, 0x05, 0x65, 0x05, 0x64, 0x05, 0x63, 0x05,
	0x85, 0x05, 0x84, 0x05, 0x68, 0x05, 0x69, 0x05, 0x6A, 0x05, 0x6B, 0x05,
	0x6C, 0x05, 0x6D, 0x05, 0x6E, 0x05, 0x6F, 0x05, 0x70, 0x05, 0x71, 0x05,
	0x72, 0x05, 0x6D, 0x05, 0x89, 0x05, 0x88, 0x05, 0x73, 0x05, 0x74, 0x05,
	0x75, 0x05, 0x76, 0x05, 0x77, 0x05, 0x78, 0x05, 0x79, 0x05, 0x7A, 0x05,
	0x8B, 0x05, 0x8A, 0x05, 0x7B, 0x05, 0x8C, 0x05, 0x8C, 0x05, 0x4A, 0x05,
	0x4B, 0x05, 0x4C, 0x05, 0x4D, 0x05, 0x4E, 0x05, 0x4F, 0x05, 0x50, 0x05,
	0x51, 0x05, 0x52, 0x05, 0x53, 0x05, 0x54, 0x05, 0x55, 0x05, 0x83, 0x05,
	0x82, 0x05
};

static const UINT8 CROM_BANK4_WORLD5[] =
{
	0x24, 0x00, 0x2C, 0x00, 0x34, 0x00, 0x38, 0x00, 0x40, 0x00, 0x48, 0x00,
	0x4C, 0x00, 0x54, 0x00, 0x5C, 0x00, 0x60, 0x00, 0x64, 0x00, 0x68, 0x00,
	0x6C, 0x00, 0x78, 0x00, 0x84, 0x00, 0x88, 0x00, 0x8C, 0x00, 0x90, 0x00,
	0x43, 0x06, 0x45, 0x06, 0x47, 0x06, 0x49, 0x06, 0x43, 0x06, 0x45, 0x06,
	0x47, 0x06, 0x49, 0x06, 0x65, 0x06, 0x63, 0x06, 0x6D, 0x06, 0x6F, 0x06,
	0x71, 0x06, 0x83, 0x06, 0x6D, 0x06, 0x6F, 0x06, 0x71, 0x06, 0x83, 0x06,
	0x6F, 0x06, 0x83, 0x06, 0x85, 0x06, 0x87, 0x06, 0x89, 0x06, 0x8B, 0x06,
	0x85, 0x06, 0x87, 0x06, 0x89, 0x06, 0x8B, 0x06, 0x8F, 0x06, 0x8D, 0x06,
	0xAD, 0x06, 0xAF, 0x06, 0xAD, 0x06, 0xAF, 0x06, 0xC3, 0x06, 0xB1, 0x06,
	0xC5, 0x06, 0xC7, 0x06, 0xC9, 0x06, 0xCB, 0x06, 0xCD, 0x06, 0xCF, 0x06,
	0xC5, 0x06, 0xC7, 0x06, 0xC9, 0x06, 0xCB, 0x06, 0xCD, 0x06, 0xCF, 0x06,
	0xCB, 0x06, 0xC5, 0x06, 0xE3, 0x06, 0xD1, 0x06, 0xE3, 0x06, 0xD1, 0x06,
	0xE5, 0x06, 0xE3, 0x06
};

static const UINT8 CROM_BANK4_WORLD6[] =
{
	0x2A, 0x00, 0x32, 0x00, 0x3A, 0x00, 0x3E, 0x00, 0x46, 0x00, 0x4E, 0x00,
	0x52, 0x00, 0x5A, 0x00, 0x62, 0x00, 0x66, 0x00, 0x6A, 0x00, 0x6E, 0x00,
	0x72, 0x00, 0x76, 0x00, 0x7A, 0x00, 0x7E, 0x00, 0x82, 0x00, 0x86, 0x00,
	0x8A, 0x00, 0x8E, 0x00, 0x92, 0x00, 0x5B, 0x08, 0x5D, 0x08, 0x5F, 0x08,
	0x61, 0x08, 0x5B, 0x08, 0x5D, 0x08, 0x5F, 0x08, 0x61, 0x08, 0x65, 0x08,
	0x63, 0x08, 0x67, 0x08, 0x69, 0x08, 0x7B, 0x08, 0x7D, 0x08, 0x67, 0x08,
	0x69, 0x08, 0x7B, 0x08, 0x7D, 0x08, 0x81, 0x08, 0x7F, 0x08, 0x83, 0x08,
	0x85, 0x08, 0x87, 0x08, 0x89, 0x08, 0x83, 0x08, 0x85, 0x08, 0x87, 0x08,
	0x89, 0x08, 0x9D, 0x08, 0x9B, 0x08, 0x9F, 0x08, 0xA1, 0x08, 0x9F, 0x08,
	0xA1, 0x08, 0xA5, 0x08, 0xA3, 0x08, 0xA7, 0x08, 0xA9, 0x08, 0xA7, 0x08,
	0xA9, 0x08, 0xBD, 0x08, 0xBB, 0x08, 0xBF, 0x08, 0xC1, 0x08, 0xBF, 0x08,
	0xC1, 0x08, 0xC5, 0x08, 0xC3, 0x08, 0xDD, 0x08, 0xDF, 0x08, 0xDD, 0x08,
	0xDF, 0x08, 0xE1, 0x08, 0xC7, 0x08, 0xC7, 0x08
};

static const UINT8 CROM_BANK4_WORLD7[] =
{
	0x2A, 0x00, 0x32, 0x00, 0x3A, 0x00, 0x3E, 0x00, 0x42, 0x00, 0x46, 0x00,
	0x4A, 0x00, 0x4E, 0x00, 0x52, 0x00, 0x56, 0x00, 0x5E, 0x00, 0x66, 0x00,
	0x6A, 0x00, 0x6E, 0x00, 0x72, 0x00, 0x76, 0x00, 0x78, 0x00, 0x7A, 0x00,
	0x7E, 0x00, 0x82, 0x00, 0x86, 0x00, 0xA1, 0x05, 0xA3, 0x05, 0xA5, 0x05,
	0xA7, 0x05, 0xA9, 0x05, 0xAB, 0x05, 0xAD, 0x05, 0xAF, 0x05, 0xC1, 0x05,
	0xC3, 0x05, 0xC5, 0x05, 0xC7, 0x05, 0xC5, 0x05, 0xC7, 0x05, 0xE3, 0x05,
	0xE5, 0x05, 0xE7, 0x05, 0xE9, 0x05, 0xED, 0x05, 0xEF, 0x05, 0x03, 0x06,
	0x05, 0x06, 0x58, 0x10, 0x5A, 0x10, 0x5C, 0x10, 0x5E, 0x10, 0x58, 0x10,
	0x5A, 0x10, 0x5C, 0x10, 0x5E, 0x10, 0x62, 0x10, 0x64, 0x10, 0x25, 0x06,
	0x27, 0x06, 0x25, 0x06, 0x27, 0x06, 0x29, 0x06, 0x2B, 0x06, 0x2F, 0x06,
	0x2F, 0x06, 0x2D, 0x06, 0x2F, 0x06, 0x9C, 0x05, 0x9D, 0x05, 0x9C, 0x05,
	0x9D, 0x05, 0x78, 0x10, 0x9D, 0x05
};

static const UINT8 CROM_BANK4_WORLD8[] =
{
	0x30, 0x00, 0x34, 0x00, 0x38, 0x00, 0x3C, 0x00, 0x3E, 0x00, 0x40, 0x00,
	0x42, 0x00, 0x46, 0x00, 0x4A, 0x00, 0x4E, 0x00, 0x5C, 0x00, 0x6A, 0x00,
	0x6E, 0x00, 0x70, 0x00, 0x72, 0x00, 0x76, 0x00, 0x7A, 0x00, 0x7E, 0x00,
	0x82, 0x00, 0x88, 0x00, 0x8E, 0x00, 0x92, 0x00, 0x96, 0x00, 0x9A, 0x00,
	0xB0, 0x09, 0xB2, 0x09, 0xB0, 0x09, 0xB2, 0x09, 0xB4, 0x09, 0xB0, 0x09,
	0xB6, 0x09, 0xB6, 0x09, 0xB6, 0x09, 0xB8, 0x09, 0xBA, 0x09, 0xB8, 0x09,
	0xBA, 0x09, 0xBC, 0x09, 0xB8, 0x09, 0xBE, 0x09, 0xD0, 0x09, 0xD2, 0x09,
	0xD4, 0x09, 0xD6, 0x09, 0xD8, 0x09, 0xDA, 0x09, 0xBE, 0x09, 0xD0, 0x09,
	0xD2, 0x09, 0xD4, 0x09, 0xD6, 0x09, 0xD8, 0x09, 0xDA, 0x09, 0xDC, 0x09,
	0xD4, 0x09, 0xDE, 0x09, 0xDE, 0x09, 0xF4, 0x09, 0xDE, 0x09, 0xF6, 0x09,
	0xF8, 0x09, 0xF6, 0x09, 0xF8, 0x09, 0x1A, 0x0A, 0xF6, 0x09, 0x1C, 0x0A,
	0x1E, 0x0A, 0x30, 0x0A, 0x1C, 0x0A, 0x1E, 0x0A, 0x30, 0x0A, 0x32, 0x0A,
	0x1C, 0x0A, 0x34, 0x0A, 0x36, 0x0A, 0x34, 0x0A, 0x36, 0x0A, 0x38, 0x0A,
	0x36, 0x0A
};

static const UINT8 CROM_BANK4_WORLD9[] =
{
	0x30, 0x00, 0x34, 0x00, 0x38, 0x00, 0x40, 0x00, 0x48, 0x00, 0x50, 0x00,
	0x58, 0x00, 0x60, 0x00, 0x68, 0x00, 0x70, 0x00, 0x78, 0x00, 0x80, 0x00,
	0x88, 0x00, 0x90, 0x00, 0x98, 0x00, 0xA0, 0x00, 0xA6, 0x00, 0xAC, 0x00,
	0xB4, 0x00, 0xBC, 0x00, 0xC4, 0x00, 0xCC, 0x00, 0xD0, 0x00, 0xD4, 0x00,
	0xAF, 0x0B, 0xB1, 0x0B, 0xAF, 0x0B, 0xB1, 0x0B, 0xB3, 0x0B, 0xB5, 0x0B,
	0xEF, 0x0F, 0xF0, 0x0F, 0xB7, 0x0B, 0xB9, 0x0B, 0xBB, 0x0B, 0xBD, 0x0B,
	0xB7, 0x0B, 0xB9, 0x0B, 0xBB, 0x0B, 0xBD, 0x0B, 0xCF, 0x0B, 0xD1, 0x0B,
	0xF1, 0x0F, 0xF2, 0x0F, 0x47, 0x11, 0x49, 0x11, 0x4B, 0x11, 0x4D, 0x11,
	0x47, 0x11, 0x49, 0x11, 0x4B, 0x11, 0x4D, 0x11, 0x4F, 0x11, 0x51, 0x11,
	0xFD, 0x0F, 0xFE, 0x0F, 0xEF, 0x0B, 0xF1, 0x0B, 0xF3, 0x0B, 0xF5, 0x0B,
	0xEF, 0x0B, 0xF1, 0x0B, 0xF3, 0x0B, 0xF5, 0x0B, 0xF7, 0x0B, 0xF9, 0x0B,
	0xF3, 0x0F, 0xF4, 0x0F, 0xFB, 0x0B, 0xFD, 0x0B, 0x0F, 0x0C, 0x11, 0x0C,
	0xFB, 0x0B, 0xFD, 0x0B, 0x0F, 0x0C, 0x11, 0x0C, 0x13, 0x0C, 0x15, 0x0C,
	0xF5, 0x0F, 0xF6, 0x0F, 0x6B, 0x11, 0x6D, 0x11, 0x6F, 0x11, 0x6B, 0x11,
	0x6D, 0x11, 0x6F, 0x11, 0x1D, 0x0C, 0x2F, 0x0C, 0xF7, 0x0F, 0xF8, 0x0F,
	0x31, 0x0C, 0x33, 0x0C, 0x35, 0x0C, 0x37, 0x0C, 0x31, 0x0C, 0x33, 0x0C,
	0x35, 0x0C, 0x37, 0x0C, 0x39, 0x0C, 0x3B, 0x0C, 0xF9, 0x0F, 0xFA, 0x0F,
	0x3D, 0x0C, 0x4F, 0x0C, 0x3D, 0x0C, 0x4F, 0x0C, 0x51, 0x0C, 0x53, 0x0C,
	0xFB, 0x0F, 0xFC, 0x0F, 0xDC, 0x02, 0xDD, 0x02
};

static const UINT8 CROM_BANK5_WORLD0[] =
{
	0x1C, 0x00, 0x1C, 0x00, 0x32, 0x00, 0x32, 0x00, 0x48, 0x00, 0x48, 0x00,
	0xB6, 0x00, 0x5E, 0x00, 0xA0, 0x00, 0xA0, 0x00, 0x74, 0x00, 0x74, 0x00,
	0x8A, 0x00, 0x8A, 0x00, 0x01, 0x08, 0xC4, 0x19, 0x49, 0x08, 0xC4, 0x19,
	0x85, 0x08, 0xC7, 0x18, 0xC3, 0x5A, 0xB7, 0x6E, 0xD0, 0x02, 0x00, 0x01,
	0xFF, 0xFF, 0xC9, 0x08, 0xD3, 0x19, 0x09, 0x09, 0xC4, 0x19, 0x45, 0x09,
	0xC7, 0x18, 0xC3, 0x5A, 0xB7, 0x6E, 0xF0, 0x00, 0x00, 0x01, 0xFF, 0xFF,
	0x80, 0x09, 0xC5, 0x19, 0xC8, 0x09, 0xC3, 0x19, 0x04, 0x0A, 0xC7, 0x18,
	0xC3, 0x5A, 0xB7, 0x6E, 0xD0, 0x02, 0x00, 0x01, 0xFF, 0xFF, 0x41, 0x0A,
	0xC7, 0x29, 0x89, 0x0A, 0xC7, 0x29, 0xC5, 0x0A, 0xC7, 0x18, 0xC3, 0x5A,
	0xB7, 0x6E, 0xD0, 0x02, 0x00, 0x01, 0xFF, 0xFF, 0x01, 0x0B, 0x45, 0x29,
	0x49, 0x0B, 0x43, 0x29, 0x85, 0x0B, 0xC7, 0x18, 0xC3, 0x5A, 0xB7, 0x6E,
	0xD0, 0x02, 0x00, 0x01, 0xFF, 0xFF, 0x49, 0x0B, 0x47, 0x29, 0x49, 0x0B,
	0x47, 0x29, 0x85, 0x0B, 0xC7, 0x18, 0xC3, 0x5A, 0xB7, 0x6E, 0xD0, 0x02,
	0x00, 0x01, 0xFF, 0xFF, 0xC0, 0x0B, 0xC1, 0x18, 0x08, 0x0C, 0xC1, 0x18,
	0x44, 0x0C, 0xC7, 0x18, 0xC3, 0x5A, 0xB7, 0x6E, 0xD0, 0x02, 0x00, 0x01,
	0xFF, 0xFF, 0x80, 0x04, 0xC0, 0x39, 0x80, 0x0C, 0xC3, 0x39, 0xC4, 0x0C,
	0xC7, 0x18, 0xC3, 0x5A, 0xB7, 0x6E, 0x3C, 0x00, 0x00, 0x01, 0xFF, 0xFF
};

static const UINT8 CROM_BANK5_WORLD1[] =
{
	0x0C, 0x00, 0x22, 0x00, 0x38, 0x00, 0x4E, 0x00, 0x64, 0x00, 0x7A, 0x00,
	0x01, 0x08, 0xC4, 0x19, 0x49, 0x08, 0xC4, 0x19, 0x84, 0x08, 0xC7, 0x18,
	0xE6, 0x5A, 0xBF, 0x6E, 0x58, 0x02, 0x00, 0x01, 0xFF, 0xFF, 0xC1, 0x08,
	0xC4, 0x19, 0x09, 0x09, 0xC4, 0x19, 0x44, 0x09, 0xC7, 0x18, 0xEB, 0x5A,
	0xC1, 0x6E, 0x58, 0x02, 0x00, 0x01, 0xFF, 0xFF, 0x41, 0x0A, 0x45, 0x1A,
	0x89, 0x0A, 0x43, 0x1A, 0xC4, 0x0A, 0xC7, 0x18, 0xC3, 0x5A, 0xB7, 0x6E,
	0x58, 0x02, 0x00, 0x01, 0xFF, 0xFF, 0x81, 0x11, 0xC1, 0x29, 0xC9, 0x11,
	0xC1, 0x29, 0x04, 0x0A, 0xC7, 0x18, 0xC3, 0x5A, 0xB7, 0x6E, 0x58, 0x02,
	0x00, 0x01, 0xFF, 0xFF, 0x01, 0x03, 0xC0, 0x19, 0x49, 0x03, 0xC0, 0x19,
	0x84, 0x0B, 0xC7, 0x18, 0xC3, 0x5A, 0xB7, 0x6E, 0x58, 0x02, 0x00, 0x01,
	0xFF, 0xFF, 0xC9, 0x03, 0xC0, 0x39, 0x09, 0x04, 0xC0, 0x39, 0x44, 0x0C,
	0xC7, 0x18, 0xC3, 0x5A, 0xB7, 0x6E, 0x58, 0x02, 0x00, 0x01, 0xFF, 0xFF
};

static const UINT8 CROM_BANK5_WORLD2[] =
{
	0x0E, 0x00, 0x24, 0x00, 0x3A, 0x00, 0x50, 0x00, 0x66, 0x00, 0x7C, 0x00,
	0x92, 0x00, 0x01, 0x18, 0xC7, 0x39, 0x49, 0x18, 0xC5, 0x39, 0x84, 0x08,
	0xC7, 0x18, 0xC3, 0x5A, 0xB7, 0x6E, 0xE0, 0x01, 0x00, 0x01, 0xFF, 0xFF,
	0xC1, 0x08, 0xC7, 0x39, 0x09, 0x09, 0xC5, 0x39, 0x44, 0x09, 0xC7, 0x18,
	0xC3, 0x5A, 0xB7, 0x6E, 0xE0, 0x01, 0x00, 0x01, 0xFF, 0xFF, 0x81, 0x09,
	0x47, 0x49, 0xC9, 0x09, 0x45, 0x49, 0x04, 0x0A, 0xC7, 0x18, 0xC3, 0x5A,
	0xB7, 0x6E, 0xE0, 0x01, 0x00, 0x01, 0xFF, 0xFF, 0x41, 0x02, 0xC7, 0x49,
	0x89, 0x02, 0xC5, 0x49, 0xC4, 0x0A, 0xC7, 0x18, 0xC3, 0x5A, 0xB7, 0x6E,
	0xE0, 0x01, 0x00, 0x01, 0xFF, 0xFF, 0x01, 0x03, 0x43, 0x29, 0x49, 0x03,
	0x42, 0x29, 0x84, 0x0B, 0xC7, 0x18, 0xC3, 0x5A, 0xB7, 0x6E, 0xE0, 0x01,
	0x00, 0x01, 0xFF, 0xFF, 0xC1, 0x1B, 0x43, 0x29, 0x09, 0x1C, 0x42, 0x29,
	0x44, 0x0C, 0xC7, 0x18, 0xC3, 0x5A, 0xB7, 0x6E, 0xE0, 0x01, 0x00, 0x01,
	0xFF, 0xFF, 0x81, 0x0C, 0x43, 0x39, 0xC9, 0x0C, 0x41, 0x39, 0x04, 0x0D,
	0xC7, 0x18, 0xC3, 0x5A, 0xB7, 0x6E, 0xE0, 0x01, 0x00, 0x01, 0xFF, 0xFF
};

static const UINT8 CROM_BANK5_WORLD3[] =
{
	0x10, 0x00, 0x26, 0x00, 0x3C, 0x00, 0x52, 0x00, 0x68, 0x00, 0x7E, 0x00,
	0x94, 0x00, 0xAA, 0x00, 0x01, 0x08, 0xC1, 0x39, 0x49, 0x08, 0xC1, 0x39,
	0x84, 0x08, 0xC7, 0x18, 0xC3, 0x5A, 0xB7, 0x6E, 0x68, 0x01, 0x00, 0x01,
	0xFF, 0xFF, 0xC1, 0x08, 0xC1, 0x39, 0x09, 0x09, 0xC1, 0x39, 0x44, 0x09,
	0xC7, 0x18, 0xC3, 0x5A, 0xB7, 0x6E, 0x68, 0x01, 0x00, 0x01, 0xFF, 0xFF,
	0x81, 0x01, 0x40, 0x39, 0xC9, 0x01, 0x40, 0x39, 0x04, 0x0A, 0xC7, 0x18,
	0xC3, 0x5A, 0xB7, 0x6E, 0x68, 0x01, 0x00, 0x01, 0xFF, 0xFF, 0x41, 0x0A,
	0xC3, 0x39, 0x89, 0x0A, 0xC3, 0x39, 0xC4, 0x0A, 0xC7, 0x18, 0xC3, 0x5A,
	0xB7, 0x6E, 0x68, 0x01, 0x00, 0x01, 0xFF, 0xFF, 0x01, 0x03, 0x40, 0x19,
	0x49, 0x03, 0x40, 0x19, 0x84, 0x0B, 0xC7, 0x18, 0xC3, 0x5A, 0xB7, 0x6E,
	0x68, 0x01, 0x00, 0x01, 0xFF, 0xFF, 0xC1, 0x0B, 0xD3, 0x49, 0x09, 0x0C,
	0xC9, 0x49, 0x44, 0x0C, 0xC7, 0x18, 0xC3, 0x5A, 0xB7, 0x6E, 0x78, 0x00,
	0x00, 0x01, 0xFF, 0xFF, 0x81, 0x24, 0xC7, 0x1B, 0xC9, 0x24, 0xC7, 0x1B,
	0x04, 0x0D, 0xC7, 0x18, 0xC3, 0x5A, 0xB7, 0x6E, 0x68, 0x01, 0x00, 0x01,
	0xFF, 0xFF, 0x40, 0x65, 0x41, 0x29, 0x88, 0x65, 0x40, 0x29, 0xC4, 0x0D,
	0xC7, 0x18, 0xC3, 0x5A, 0xB7, 0x6E, 0x68, 0x01, 0x00, 0x01, 0xFF, 0xFF
};

static const UINT8 CROM_BANK5_WORLD4[] =
{
	0x0C, 0x00, 0x22, 0x00, 0x38, 0x00, 0x4E, 0x00, 0x64, 0x00, 0x7A, 0x00,
	0x00, 0x38, 0xC6, 0x18, 0x48, 0x38, 0xC6, 0x18, 0x84, 0x08, 0xC7, 0x18,
	0xC3, 0x5A, 0xB7, 0x6E, 0x68, 0x01, 0x00, 0x01, 0xFF, 0xFF, 0xC0, 0x98,
	0xC9, 0x18, 0x08, 0x99, 0xC9, 0x18, 0x44, 0x09, 0xC7, 0x18, 0xC3, 0x5A,
	0xB7, 0x6E, 0x68, 0x01, 0x00, 0x01, 0xFF, 0xFF, 0x80, 0x59, 0xC9, 0x18,
	0xC8, 0x59, 0xC9, 0x18, 0x04, 0x0A, 0xC7, 0x18, 0xC3, 0x5A, 0xB7, 0x6E,
	0x68, 0x01, 0x00, 0x01, 0xFF, 0xFF, 0x40, 0x3A, 0xC4, 0x18, 0x88, 0x3A,
	0xC4, 0x18, 0xC4, 0x0A, 0xC7, 0x18, 0xC3, 0x5A, 0xB7, 0x6E, 0x68, 0x01,
	0x00, 0x01, 0xFF, 0xFF, 0x00, 0x03, 0x80, 0x18, 0x48, 0x03, 0x80, 0x18,
	0x84, 0x0B, 0xC7, 0x18, 0xC3, 0x5A, 0xB7, 0x6E, 0x68, 0x01, 0x00, 0x01,
	0xFF, 0xFF, 0xC0, 0x5B, 0x85, 0x31, 0x08, 0x5C, 0x85, 0x31, 0x44, 0x0C,
	0xC7, 0x18, 0xC3, 0x5A, 0xB7, 0x6E, 0x68, 0x01, 0x00, 0x01, 0xFF, 0xFF
};

static const UINT8 CROM_BANK5_WORLD5[] =
{
	0x0C, 0x00, 0x22, 0x00, 0x38, 0x00, 0x4E, 0x00, 0x64, 0x00, 0x7A, 0x00,
	0x01, 0x18, 0x43, 0x39, 0x49, 0x18, 0x43, 0x39, 0x85, 0x08, 0xC7, 0x18,
	0xC3, 0x5A, 0xB7, 0x6E, 0x68, 0x01, 0x00, 0x01, 0xFF, 0xFF, 0xC1, 0x18,
	0xC3, 0x39, 0x09, 0x19, 0xC3, 0x39, 0x45, 0x09, 0xC3, 0x18, 0xC3, 0x5A,
	0xB7, 0x6E, 0x68, 0x01, 0x00, 0x01, 0xFF, 0xFF, 0x81, 0x19, 0x43, 0x39,
	0xC9, 0x19, 0x43, 0x39, 0x05, 0x0A, 0xC7, 0x18, 0xC3, 0x5A, 0xB7, 0x6E,
	0x68, 0x01, 0x00, 0x01, 0xFF, 0xFF, 0x41, 0x0A, 0x43, 0x19, 0x89, 0x0A,
	0x43, 0x19, 0xC5, 0x0A, 0x47, 0x19, 0xC3, 0x5A, 0xB7, 0x6E, 0x68, 0x01,
	0x00, 0x00, 0xFF, 0xFF, 0x01, 0x2B, 0xC3, 0x19, 0x49, 0x2B, 0xC2, 0x19,
	0x85, 0x0B, 0xC7, 0x18, 0xC3, 0x5A, 0xB7, 0x6E, 0x68, 0x01, 0x00, 0x01,
	0xFF, 0xFF, 0xC1, 0x0B, 0xC3, 0x18, 0x09, 0x0C, 0xC3, 0x18, 0x45, 0x0C,
	0xC7, 0x18, 0xC3, 0x5A, 0xB7, 0x6E, 0x68, 0x01, 0x00, 0x01, 0xFF, 0xFF
};

static const UINT8 CROM_BANK5_WORLD6[] =
{
	0x0E, 0x00, 0x24, 0x00, 0x3A, 0x00, 0x50, 0x00, 0x66, 0x00, 0x7C, 0x00,
	0x92, 0x00, 0x01, 0x18, 0x45, 0x39, 0x49, 0x18, 0x43, 0x39, 0x85, 0x08,
	0xC7, 0x18, 0xC3, 0x5A, 0xB7, 0x6E, 0x68, 0x01, 0x00, 0x01, 0xFF, 0xFF,
	0xC1, 0x18, 0x45, 0x39, 0x09, 0x19, 0x43, 0x39, 0x45, 0x09, 0xC7, 0x18,
	0xC3, 0x5A, 0xB7, 0x6E, 0x68, 0x01, 0x00, 0x01, 0xFF, 0xFF, 0x81, 0x19,
	0x45, 0x39, 0xC9, 0x19, 0x43, 0x39, 0x05, 0x0A, 0xC7, 0x18, 0xC3, 0x5A,
	0xB7, 0x6E, 0x68, 0x01, 0x00, 0x01, 0xFF, 0xFF, 0x41, 0x0A, 0xC7, 0x39,
	0x89, 0x0A, 0xC5, 0x39, 0xC5, 0x0A, 0xC7, 0x18, 0xC3, 0x5A, 0xB7, 0x6E,
	0x68, 0x01, 0x00, 0x00, 0xFF, 0xFF, 0x01, 0x0B, 0x05, 0x42, 0x49, 0x0B,
	0x03, 0x42, 0x85, 0x0B, 0xC7, 0x18, 0xC3, 0x5A, 0xB7, 0x6E, 0x68, 0x01,
	0x00, 0x01, 0xFF, 0xFF, 0xC1, 0x0B, 0xC5, 0x39, 0x09, 0x0C, 0xC3, 0x39,
	0x45, 0x0C, 0xC7, 0x18, 0xC3, 0x5A, 0xB7, 0x6E, 0x68, 0x01, 0x00, 0x01,
	0xFF, 0xFF, 0x81, 0x0C, 0x4D, 0x0A, 0xC9, 0x0C, 0x4B, 0x0A, 0x05, 0x0D,
	0xC7, 0x18, 0xC3, 0x5A, 0xB7, 0x6E, 0x68, 0x01, 0x00, 0x01, 0xFF, 0xFF
};

static const UINT8 CROM_BANK5_WORLD7[] =
{
	0x0E, 0x00, 0x24, 0x00, 0x3A, 0x00, 0x50, 0x00, 0x66, 0x00, 0x7C, 0x00,
	0x92, 0x00, 0x01, 0x18, 0xC5, 0x41, 0x49, 0x18, 0xC3, 0x41, 0x85, 0x08,
	0xC7, 0x18, 0xC3, 0x5A, 0xB7, 0x6E, 0xE0, 0x01, 0x00, 0x01, 0xFF, 0xFF,
	0xC1, 0x08, 0xC5, 0x41, 0x09, 0x09, 0xC3, 0x41, 0x45, 0x09, 0xC7, 0x18,
	0xC3, 0x5A, 0xB7, 0x6E, 0xE0, 0x01, 0x00, 0x01, 0xFF, 0xFF, 0x81, 0x09,
	0x45, 0x4A, 0xC9, 0x09, 0x43, 0x4A, 0x05, 0x0A, 0xC7, 0x18, 0xC3, 0x5A,
	0xB7, 0x6E, 0xE0, 0x01, 0x00, 0x01, 0xFF, 0xFF, 0x41, 0x1A, 0xC5, 0x39,
	0x81, 0x1A, 0xC3, 0x39, 0xC5, 0x0A, 0xC7, 0x18, 0xC3, 0x5A, 0xB7, 0x6E,
	0xE0, 0x01, 0x00, 0x00, 0xFF, 0xFF, 0x01, 0x0B, 0x45, 0x5A, 0x49, 0x0B,
	0x43, 0x5A, 0x85, 0x0B, 0xC7, 0x18, 0xC3, 0x5A, 0xB7, 0x6E, 0xE0, 0x01,
	0x00, 0x01, 0xFF, 0xFF, 0xC1, 0x03, 0xC5, 0x59, 0x09, 0x04, 0xC3, 0x59,
	0x45, 0x0C, 0xC7, 0x18, 0xC3, 0x5A, 0xB7, 0x6E, 0xE0, 0x01, 0x00, 0x01,
	0xFF, 0xFF, 0x80, 0x0C, 0x45, 0x29, 0xC8, 0x0C, 0x43, 0x29, 0x04, 0x0D,
	0xC7, 0x18, 0xC3, 0x5A, 0xB7, 0x6E, 0xE0, 0x01, 0x00, 0x01, 0xFF, 0xFF
};

static const UINT8 CROM_BANK5_WORLD8[] =
{
	0x10, 0x00, 0x26, 0x00, 0x3C, 0x00, 0x52, 0x00, 0x68, 0x00, 0x7E, 0x00,
	0x94, 0x00, 0xAA, 0x00, 0x01, 0x08, 0x43, 0x49, 0x49, 0x08, 0x43, 0x49,
	0x85, 0x08, 0xC7, 0x18, 0xC3, 0x5A, 0xB7, 0x6E, 0x68, 0x01, 0x00, 0x01,
	0xFF, 0xFF, 0xC1, 0x00, 0x43, 0x39, 0x09, 0x01, 0x43, 0x49, 0x45, 0x01,
	0xC7, 0x18, 0xC3, 0x5A, 0xB7, 0x6E, 0x68, 0x01, 0x00, 0x01, 0xFF, 0xFF,
	0x81, 0x09, 0xC3, 0x59, 0xC9, 0x09, 0xC3, 0x59, 0x05, 0x0A, 0xC7, 0x18,
	0xC3, 0x5A, 0xB7, 0x6E, 0x68, 0x01, 0x00, 0x01, 0xFF, 0xFF, 0x41, 0x32,
	0xC3, 0x5B, 0x89, 0x32, 0xC3, 0x5B, 0xC5, 0x0A, 0xC7, 0x18, 0xC3, 0x5A,
	0xB7, 0x6E, 0x68, 0x01, 0x00, 0x00, 0xFF, 0xFF, 0x01, 0x03, 0xC3, 0x49,
	0x49, 0x03, 0xC3, 0x49, 0x85, 0x0B, 0xC7, 0x18, 0xC3, 0x5A, 0xB7, 0x6E,
	0x68, 0x01, 0x00, 0x01, 0xFF, 0xFF, 0xC1, 0x0B, 0xC3, 0x19, 0x09, 0x0C,
	0xC3, 0x19, 0x45, 0x0C, 0xC7, 0x18, 0xC3, 0x5A, 0xB7, 0x6E, 0x68, 0x01,
	0x00, 0x01, 0xFF, 0xFF, 0x81, 0x14, 0xC5, 0x39, 0xC9, 0x14, 0xC3, 0x39,
	0x05, 0x0D, 0xC7, 0x18, 0xC3, 0x5A, 0xB7, 0x6E, 0x68, 0x01, 0x00, 0x01,
	0xFF, 0xFF, 0x41, 0x0D, 0xC7, 0x39, 0x89, 0x0D, 0xC5, 0x39, 0xC5, 0x0D,
	0xC7, 0x18, 0xC3, 0x5A, 0xB7, 0x6E, 0x68, 0x01, 0x00, 0x01, 0xFF, 0xFF
};

static const UINT8 CROM_BANK5_WORLD9[] =
{
	0x10, 0x00, 0x26, 0x00, 0x3C, 0x00, 0x52, 0x00, 0x68, 0x00, 0x7E, 0x00,
	0x94, 0x00, 0xAA, 0x00, 0x01, 0x08, 0x03, 0x1A, 0x49, 0x08, 0x03, 0x1A,
	0x85, 0x08, 0xC7, 0x18, 0xC3, 0x5A, 0xB7, 0x6E, 0x00, 0x05, 0x00, 0x01,
	0xFF, 0xFF, 0xC1, 0x18, 0xC3, 0x18, 0x09, 0x19, 0x03, 0x1A, 0x45, 0x09,
	0xC7, 0x18, 0xC3, 0x5A, 0xB7, 0x6E, 0x00, 0x05, 0x00, 0x01, 0xFF, 0xFF,
	0x81, 0x19, 0x45, 0x39, 0xC9, 0x19, 0x43, 0x39, 0x05, 0x0A, 0xC7, 0x18,
	0xC3, 0x5A, 0xB7, 0x6E, 0x00, 0x05, 0x00, 0x01, 0xFF, 0xFF, 0x41, 0x1A,
	0x45, 0x18, 0x89, 0x1A, 0x43, 0x18, 0xC5, 0x0A, 0xC7, 0x18, 0xC3, 0x5A,
	0xB7, 0x6E, 0x00, 0x05, 0x00, 0x00, 0xFF, 0xFF, 0x01, 0x1B, 0xC5, 0x18,
	0x49, 0x1B, 0xC3, 0x18, 0x85, 0x0B, 0xC7, 0x18, 0xC3, 0x5A, 0xB7, 0x6E,
	0x00, 0x05, 0x00, 0x01, 0xFF, 0xFF, 0xC1, 0x13, 0x47, 0x39, 0x09, 0x14,
	0x45, 0x39, 0x45, 0x0C, 0xC7, 0x18, 0xC3, 0x5A, 0xB7, 0x6E, 0x00, 0x05,
	0x00, 0x01, 0xFF, 0xFF, 0x81, 0x1C, 0x45, 0x19, 0xC9, 0x1C, 0x43, 0x19,
	0x05, 0x0D, 0xC7, 0x18, 0xC3, 0x5A, 0xB7, 0x6E, 0x00, 0x05, 0x00, 0x01,
	0xFF, 0xFF, 0x41, 0x0D, 0xC5, 0x18, 0x89, 0x0D, 0xC3, 0x18, 0xC5, 0x0D,
	0xC7, 0x18, 0xC3, 0x5A, 0xB7, 0x6E, 0x00, 0x05, 0x00, 0x01, 0xFF, 0xFF
};

static const UINT8 CROM_BANK6[] =
{
	0x05, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x04,
	0x82, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x21, 0x30, 0x05, 0x00, 0x00,
	0x00, 0x00, 0x42, 0x00, 0x00, 0x33, 0x04, 0x00, 0x54, 0x40, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x50, 0x40, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x04, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x22, 0x22, 0x22, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x03, 0x33, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x33, 0x20, 0x23, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x33, 0x01,
	0x00, 0x00, 0x00, 0x00, 0x22, 0x22, 0x22, 0x02, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x34, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x23, 0x02, 0x03, 0x40,
	0x00, 0x00, 0x00, 0x00, 0x22, 0x00, 0x12, 0x04, 0x00, 0x00, 0x00, 0x00,
	0x02, 0x22, 0x02, 0x04, 0x00, 0x00, 0x00, 0x00, 0x44, 0x03, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x30, 0x44, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x30, 0x53, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x22, 0x32, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x04, 0x40, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x22, 0x02, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x30, 0x30, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x12, 0x21, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x22, 0x20, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x22, 0x02, 0x02, 0x02,
	0x00, 0x00, 0x00, 0x00, 0x02, 0x22, 0x20, 0x03, 0x00, 0x00, 0x00, 0x00,
	0x12, 0x12, 0x11, 0x02, 0x00, 0x00, 0x00, 0x00, 0x03, 0x21, 0x00, 0x03,
	0x00, 0x00, 0x00, 0x00, 0x12, 0x30, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x20, 0x01, 0x22, 0x03, 0x00, 0x00, 0x00, 0x00, 0x12, 0x21, 0x22, 0x03,
	0x00, 0x00, 0x00, 0x00, 0x02, 0x02, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x10, 0x22, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x10, 0x00, 0x31,
	0x00, 0x00, 0x00, 0x00, 0x21, 0x01, 0x01, 0x11, 0x00, 0x00, 0x00, 0x00,
	0x20, 0x03, 0x02, 0x02, 0x00, 0x00, 0x00, 0x00, 0x33, 0x20, 0x00, 0x10,
	0x00, 0x00, 0x00, 0x00, 0x02, 0x20, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x30, 0x00, 0x02, 0x11, 0x00, 0x00, 0x00, 0x00
};

static const UINT8 CROM_BANK6_EXTRA[] =
{
	0x05, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 0x00, 0x00, 0x30,
	0x00, 0x00, 0x00, 0x00, 0x13, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x30, 0x60, 0x00, 0x00, 0x00, 0x00, 0x05, 0x40, 0x04, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x35, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x12, 0x02, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x22, 0x21, 0x02, 0x03,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x30, 0x03, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x33, 0x30, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x33, 0x34, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x02, 0x24, 0x22, 0x02, 0x00, 0x00, 0x00, 0x00,
	0x44, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x05, 0x03, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x01,
	0x50, 0x00, 0x04, 0x00, 0x02, 0x00, 0x02, 0x01, 0x50, 0x00, 0x04, 0x00,
	0x08, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x40, 0x21,
	0x50, 0x00, 0x00, 0x00, 0x10, 0x00, 0x04, 0x02, 0x00, 0x00, 0x00, 0x00,
	0x22, 0x02, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x03,
	0x00, 0x00, 0x00, 0x00, 0x21, 0x02, 0x12, 0x02, 0x00, 0x00, 0x00, 0x00,
	0x22, 0x02, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x22, 0x02, 0x44, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x02, 0x20, 0x52, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x11, 0x12, 0x65, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x12, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x02, 0x31, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x20, 0x32, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x22, 0x31, 0x12, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x10, 0x02, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x31, 0x22, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x20, 0x03, 0x02, 0x02, 0x00, 0x00, 0x00, 0x00, 0x33, 0x20, 0x00, 0x10,
	0x00, 0x00, 0x00, 0x00, 0x02, 0x20, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x30, 0x00, 0x02, 0x11, 0x00, 0x00, 0x00, 0x00
};

static const UINT8 CROM_BANK7_WORLD0[] = { 0x03, 0x03, 0x03, 0x03, 0x0C, 0x0C, 0x0A, 0x07, 0x0A, 0x0A, 0x08, 0x08, 0x08, 0x08 };
static const UINT8 CROM_BANK7_WORLD1[] = { 0x03, 0x05, 0x01, 0x02, 0x04, 0x04 };
static const UINT8 CROM_BANK7_WORLD2[] = { 0x03, 0x05, 0x07, 0x07, 0x08, 0x08, 0x01 };
static const UINT8 CROM_BANK7_WORLD3[] = { 0x07, 0x07, 0x07, 0x08, 0x05, 0x08, 0x07, 0x0B };
static const UINT8 CROM_BANK7_WORLD4[] = { 0x0A, 0x0B, 0x0A, 0x0A, 0x10, 0x10 };
static const UINT8 CROM_BANK7_WORLD5[] = { 0x05, 0x08, 0x04, 0x08, 0x08, 0x07 };
static const UINT8 CROM_BANK7_WORLD6[] = { 0x04, 0x07, 0x05, 0x08, 0x06, 0x08, 0x03 };
static const UINT8 CROM_BANK7_WORLD7[] = { 0x07, 0x06, 0x06, 0x05, 0x08, 0x07, 0x0A };
static const UINT8 CROM_BANK7_WORLD8[] = { 0x06, 0x05, 0x01, 0x08, 0x05, 0x05, 0x06, 0x08 };
static const UINT8 CROM_BANK7_WORLD9[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 };

static const struct CBANK CROM_BANK4[] =
{
	{ CROM_BANK4_WORLD0, sizeof CROM_BANK4_WORLD0 },
	{ CROM_BANK4_WORLD1, sizeof CROM_BANK4_WORLD1 },
	{ CROM_BANK4_WORLD2, sizeof CROM_BANK4_WORLD2 },
	{ CROM_BANK4_WORLD3, sizeof CROM_BANK4_WORLD3 },
	{ CROM_BANK4_WORLD4, sizeof CROM_BANK4_WORLD4 },
	{ CROM_BANK4_WORLD5, sizeof CROM_BANK4_WORLD5 },
	{ CROM_BANK4_WORLD6, sizeof CROM_BANK4_WORLD6 },
	{ CROM_BANK4_WORLD7, sizeof CROM_BANK4_WORLD7 },
	{ CROM_BANK4_WORLD8, sizeof CROM_BANK4_WORLD8 },
	{ CROM_BANK4_WORLD9, sizeof CROM_BANK4_WORLD9 },
};

static const struct CBANK CROM_BANK5[] =
{
	{ CROM_BANK5_WORLD0, sizeof CROM_BANK5_WORLD0 },
	{ CROM_BANK5_WORLD1, sizeof CROM_BANK5_WORLD1 },
	{ CROM_BANK5_WORLD2, sizeof CROM_BANK5_WORLD2 },
	{ CROM_BANK5_WORLD3, sizeof CROM_BANK5_WORLD3 },
	{ CROM_BANK5_WORLD4, sizeof CROM_BANK5_WORLD4 },
	{ CROM_BANK5_WORLD5, sizeof CROM_BANK5_WORLD5 },
	{ CROM_BANK5_WORLD6, sizeof CROM_BANK5_WORLD6 },
	{ CROM_BANK5_WORLD7, sizeof CROM_BANK5_WORLD7 },
	{ CROM_BANK5_WORLD8, sizeof CROM_BANK5_WORLD8 },
	{ CROM_BANK5_WORLD9, sizeof CROM_BANK5_WORLD9 },
};

static const struct CBANK CROM_BANK7[] =
{
	{ CROM_BANK7_WORLD0, sizeof CROM_BANK7_WORLD0 },
	{ CROM_BANK7_WORLD1, sizeof CROM_BANK7_WORLD1 },
	{ CROM_BANK7_WORLD2, sizeof CROM_BANK7_WORLD2 },
	{ CROM_BANK7_WORLD3, sizeof CROM_BANK7_WORLD3 },
	{ CROM_BANK7_WORLD4, sizeof CROM_BANK7_WORLD4 },
	{ CROM_BANK7_WORLD5, sizeof CROM_BANK7_WORLD5 },
	{ CROM_BANK7_WORLD6, sizeof CROM_BANK7_WORLD6 },
	{ CROM_BANK7_WORLD7, sizeof CROM_BANK7_WORLD7 },
	{ CROM_BANK7_WORLD8, sizeof CROM_BANK7_WORLD8 },
	{ CROM_BANK7_WORLD9, sizeof CROM_BANK7_WORLD9 },
};

/*************************************
 *
 * C-Chip operation
 *
 *************************************/

static const UINT16 cchip_extra_patch[][8] =
{
	{ 0x0168, 0x0078, 0x0168, 0x0168, 0x0168, 0x02D0, 0x0168, 0x003C },
	{ 0x0258, 0x0258, 0x0258, 0x0258, 0x0258, 0x0258, 0x0000, 0x0000 },
	{ 0x01E0, 0x01E0, 0x01E0, 0x01E0, 0x01E0, 0x01E0, 0x01E0, 0x0000 },
	{ 0x0168, 0x0168, 0x0168, 0x0168, 0x0168, 0x001E, 0x0168, 0x0168 },
	{ 0x0168, 0x0168, 0x0168, 0x0168, 0x0168, 0x0168, 0x0000, 0x0000 },
	{ 0x0168, 0x0168, 0x0168, 0x0168, 0x0168, 0x0168, 0x0000, 0x0000 },
	{ 0x0168, 0x0168, 0x0168, 0x0168, 0x0168, 0x0168, 0x0168, 0x0000 },
	{ 0x01E0, 0x01E0, 0x01E0, 0x01E0, 0x01E0, 0x01E0, 0x01E0, 0x0000 },
	{ 0x01A4, 0x01A4, 0x01A4, 0x01A4, 0x01A4, 0x01A4, 0x01A4, 0x01A4 },
	{ 0x0500, 0x0500, 0x0500, 0x0500, 0x0500, 0x0500, 0x0500, 0x0500 }
};

static const UINT8 cchip_goalin[15][6] =
{
	{ 0x00, 0x00, 0x28, 0x28, 0x50, 0x50 },
	{ 0x00, 0x00, 0x00, 0x00, 0x50, 0x50 },
	{ 0x00, 0x10, 0x20, 0x30, 0x00, 0x10 },
	{ 0x00, 0x10, 0x20, 0x30, 0x40, 0x50 },
	{ 0x00, 0x18, 0x30, 0x00, 0x18, 0x30 },
	{ 0x00, 0x18, 0x30, 0x30, 0x18, 0x00 },
	{ 0x00, 0x28, 0x50, 0x00, 0x28, 0x50 },
	{ 0x00, 0x30, 0x20, 0x50, 0x10, 0x20 },
	{ 0x00, 0x50, 0x00, 0x50, 0x00, 0x50 },
	{ 0x30, 0x18, 0x00, 0x30, 0x18, 0x00 },
	{ 0x30, 0x18, 0x00, 0x00, 0x18, 0x30 },
	{ 0x30, 0x00, 0x10, 0x40, 0x50, 0x20 },
	{ 0x30, 0x20, 0x10, 0x00, 0x10, 0x00 },
	{ 0x50, 0x40, 0x30, 0x20, 0x10, 0x00 },
	{ 0x50, 0x50, 0x50, 0x50, 0x00, 0x00 }
};

static const UINT16 cchip_round_height[] =
{
	0x02DC, 0x03B4, 0x0444, 0x04BC, 0x03B4, 0x0444, 0x05DC, 0x050C,
	0x0594, 0x06B4, 0x06B4, 0x0894, 0x065C, 0x06CC, 0x07BC, 0x08BC,
	0x0704, 0x059C, 0x030C, 0x0504, 0x0504, 0x0564, 0x05CC, 0x0644,
	0x080C, 0x086C, 0x08E4, 0x0954, 0x065C, 0x06D4, 0x0754, 0x07C4,
	0x06D4, 0x0754, 0x0824, 0x0884, 0x080C, 0x086C, 0x08E4, 0x096C,
	0x0999, 0x0999, 0x0999, 0x0999, 0x0999, 0x0999, 0x0999, 0x0999,
	0x0999, 0x0999
};

static const UINT32 cchip_round_address[] =
{
	0x030000, 0x030360, 0x030798, 0x030CA8, 0x0311B8, 0x0315F0, 0x031B00, 0x0321C0,
	0x032880, 0x032E68, 0x033600, 0x033D98, 0x0347B8, 0x034E78, 0x035610, 0x035E80,
	0x0368A0, 0x037038, 0x037620, 0x037A58, 0x038118, 0x038700, 0x038CE8, 0x0393A8,
	0x039A68, 0x03A2D8, 0x03AC20, 0x03B568, 0x03BF88, 0x03C648, 0x03CDE0, 0x03D650,
	0x03DEC0, 0x03E658, 0x03EEC8, 0x03F738, 0x040080, 0x0408F0, 0x041238, 0x041B80,
	0x0425A0, 0x042678, 0x042750, 0x042828, 0x042900, 0x0429D8, 0x042AB0, 0x042B88,
	0x042C60, 0x042D38
};

void rbisland_state::request_round_data(  )
{
	int round = m_CRAM[1][0x141]; /* 0...49 */

	memcpy(m_CRAM[1].get(), CROM_BANK1, sizeof CROM_BANK1);
	memcpy(m_CRAM[2].get(), CROM_BANK2, sizeof CROM_BANK2);

	m_CRAM[1][1] = cchip_round_height[round] >> 0;
	m_CRAM[1][2] = cchip_round_height[round] >> 8;

	m_CRAM[1][0x142] = cchip_round_address[round] >> 24;
	m_CRAM[1][0x143] = cchip_round_address[round] >> 16;
	m_CRAM[1][0x144] = cchip_round_address[round] >> 8;
	m_CRAM[1][0x145] = cchip_round_address[round] >> 0;

	/* set the secret room or boss flag */

	m_CRAM[1][0x148] = (round >= 40 || round % 4 == 3);
}

void rbisland_state::request_world_data(  )
{
	int world = m_CRAM[0][0x00d] / 4; /* 0...9 */

	/* the extra version has the world data swapped around */

	if (m_extra_version)
	{
		static const UINT8 world_swap[] =
		{
			8, 7, 6, 4, 0, 2, 3, 5, 1, 9
		};

		world = world_swap[world];
	}

	/* first two bytes in each bank are left unchanged  */

	memcpy(m_CRAM[4].get() + 2, CROM_BANK4[world].data, CROM_BANK4[world].size);
	memcpy(m_CRAM[5].get() + 2, CROM_BANK5[world].data, CROM_BANK5[world].size);
	memcpy(m_CRAM[7].get() + 2, CROM_BANK7[world].data, CROM_BANK7[world].size);

	/* banks 5 and 6 are different in the extra version */

	if (m_extra_version)
	{
		int i;

		for (i = 0; i < 8; i++)
		{
			UINT16 patch = cchip_extra_patch[world][i];

			if (patch != 0)
			{
				m_CRAM[5][m_CRAM[5][2] + 22 * i + 18] = patch >> 0;
				m_CRAM[5][m_CRAM[5][2] + 22 * i + 19] = patch >> 8;
			}
		}

		memcpy(m_CRAM[6].get() + 2, CROM_BANK6_EXTRA, sizeof CROM_BANK6_EXTRA);
	}
	else
	{
		memcpy(m_CRAM[6].get() + 2, CROM_BANK6, sizeof CROM_BANK6);
	}
}

void rbisland_state::request_goalin_data(  )
{
	int n = machine().rand() % 15;

	m_CRAM[1][0x14B] = 0x00; /* x coordinates */
	m_CRAM[1][0x14D] = 0x10;
	m_CRAM[1][0x14F] = 0x20;
	m_CRAM[1][0x151] = 0x38;
	m_CRAM[1][0x153] = 0x50;
	m_CRAM[1][0x155] = 0x60;

	m_CRAM[1][0x14A] = cchip_goalin[n][0]; /* y coordinates */
	m_CRAM[1][0x14C] = cchip_goalin[n][1];
	m_CRAM[1][0x14E] = cchip_goalin[n][2];
	m_CRAM[1][0x150] = cchip_goalin[n][3];
	m_CRAM[1][0x152] = cchip_goalin[n][4];
	m_CRAM[1][0x154] = cchip_goalin[n][5];
}

TIMER_CALLBACK_MEMBER(rbisland_state::cchip_timer)
{
	if (m_CRAM[1][0x100] == 1)
	{
		request_round_data();

		m_CRAM[1][0x100] = 0xFF;
	}

	if (m_CRAM[5][0x000] == 1)
	{
		request_world_data();

		m_CRAM[5][0x000] = 0xFF;
	}

	if (m_CRAM[1][0x149] == 1)
	{
		request_goalin_data();

		m_CRAM[1][0x149] = 0xFF;
	}

	coin_lockout_w(machine(), 1, m_CRAM[0][8] & 0x80);
	coin_lockout_w(machine(), 0, m_CRAM[0][8] & 0x40);
	coin_counter_w(machine(), 1, m_CRAM[0][8] & 0x20);
	coin_counter_w(machine(), 0, m_CRAM[0][8] & 0x10);

	m_CRAM[0][3] = ioport("800007")->read();    /* STARTn + SERVICE1 */
	m_CRAM[0][4] = ioport("800009")->read();    /* COINn */
	m_CRAM[0][5] = ioport("80000B")->read();    /* Player controls + TILT */
	m_CRAM[0][6] = ioport("80000D")->read();    /* Player controls (cocktail) */
}

/*************************************
 *
 * Writes to C-Chip - Important Bits
 *
 *************************************/

WRITE16_MEMBER(rbisland_state::rbisland_cchip_ctrl_w)
{
	/* value 2 is written here */
}

WRITE16_MEMBER(rbisland_state::rbisland_cchip_bank_w)
{
	m_current_bank = data & 7;
}

WRITE16_MEMBER(rbisland_state::rbisland_cchip_ram_w)
{
	data &= mem_mask;

	m_CRAM[m_current_bank][offset] = data;
}

/*************************************
 *
 * Reads from C-Chip
 *
 *************************************/

READ16_MEMBER(rbisland_state::rbisland_cchip_ctrl_r)
{
	/*
	    Bit 2 = Error signal
	    Bit 0 = Ready signal
	*/
	return 0x01; /* Return 0x05 for C-Chip error */
}

READ16_MEMBER(rbisland_state::rbisland_cchip_ram_r)
{
	return m_CRAM[m_current_bank][offset];
}

/*************************************
 *
 * C-Chip initialization
 *
 *************************************/

void rbisland_state::rbisland_cchip_init( int version )
{
	int i;

	m_extra_version = version;

	for (i = 0; i < 8; i++)
	{
		m_CRAM[i] = std::make_unique<UINT8[]>(0x400);

		save_pointer(m_CRAM[i].get(), "cchip/m_CRAM[i]", 0x400, i);
	}

	save_item(m_current_bank, "cchip/m_current_bank");

	machine().scheduler().timer_pulse(attotime::from_hz(60), timer_expired_delegate(FUNC(rbisland_state::cchip_timer),this));
}
