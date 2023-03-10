// license:BSD-3-Clause
// copyright-holders:David Haywood, Paul Priest
/*

Psikyo PS6807 (PS4):
See src/mame/drivers/psikyo4.cpp for more info

Each sprite has a flag denoting the screen to which it should be drawn.

*/

/*
Vid Regs:

0x3003fe4 -- ??xx???? vblank? 86??0000 always?
0x3003fe8 -- c0c0???? flipscreen for screen 1 and 2 resp.
             ????8080 Screen size select
0x3003fec -- a0000xxx always? is in two working games. 0x00000fff is bank select for gfx test
0x3003ff0 -- 000000ff brightness for screen 1, ffffff00 are probably separate rgb brightness (not used)
0x3003ff4 -- ffffff00 screen 1 clear colour
0x3003ff8 -- 000000ff brightness for screen 2, ffffff00 are probably separate rgb brightness (not used)
0x3003ffc -- ffffff00 screen 2 clear colour

HotDebut: 86010000 00009998 80000000 Small Screen
LodeRnDF: 86010000 00009998 a0000000 Small Screen

HotGmck:  86010000 1f201918 a0000000 Large Screen
HgKairak: 86010000 1f201918 a0000000 Large Screen
*/

#include "emu.h"
#include "includes/psikyo4.h"


/* --- SPRITES --- */
void psikyo4_state::draw_sprites(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect, uint32_t scr)
{
	/*- Sprite Format 0x0000 - 0x2bff -**

	0 hhhh --yy yyyy yyyy | wwww --xx xxxx xxxx  1  Ffpp pppp ---- -nnn | nnnn nnnn nnnn nnnn

	y = ypos
	x = xpos

	h = height
	w = width

	f = flip (x)
	F = flip (y) Unused?

	n = tile number

	p = palette

	**- End Sprite Format -*/

	gfx_element *const gfx = m_gfxdecode->gfx(0);
	uint32_t const *const source = m_spriteram;
	auto const list = util::big_endian_cast<uint16_t const>(m_spriteram.target() + 0x2c00/4 + 0x04/4); /* 0x2c00/0x2c02 what are these for, pointers? one for each screen */
	uint16_t const listlen = 0xc00/2 - 0x04/2;
	uint16_t listcntr = 0;
	bool const flipscreen = BIT(m_vidregs[1], (scr == 0 ? 31 : 23));
	uint16_t const screen_height = screen.visible_area().max_y + 1;

	while (listcntr < listlen)
	{
		uint16_t const listdat = list[listcntr];
		uint16_t const sprnum = (listdat & 0x03ff) * 2;

		/* start drawing */
		if ((listdat & 0x8000) == 0 && (listdat & 0x2000) == scr) /* draw only selected screen */
		{
			int loopnum = 0;

			int16_t ypos        = (source[sprnum + 0] & 0x03ff0000) >> 16;
			int16_t xpos        = (source[sprnum + 0] & 0x000003ff) >> 00;

			uint8_t const high  = ((source[sprnum + 0] & 0xf0000000) >> (12 + 16)) + 1;
			uint8_t const wide  = ((source[sprnum + 0] & 0x0000f000) >> 12) + 1;

			uint32_t const tnum = (source[sprnum + 1] & 0x0007ffff) >> 00;

			uint8_t const colr  = (source[sprnum + 1] & 0x3f000000) >> 24;

			bool flipx          = (source[sprnum + 1] & 0x40000000);
			bool flipy          = (source[sprnum + 1] & 0x80000000); /* Guess */

			if (ypos & 0x200) ypos -= 0x400;
			if (xpos & 0x200) xpos -= 0x400;

			if (flipscreen)
			{
				/* Screen Height depends on game */
				ypos = screen_height - ypos - high * 16;
				xpos = 40 * 8 - xpos - wide * 16;
				flipx = !flipx;
				flipy = !flipy;
			}

			int xstart, xend, xinc;
			if (flipx)  { xstart = wide - 1;  xend = -1;    xinc = -1; }
			else        { xstart = 0;         xend = wide;  xinc = +1; }

			int ystart, yend, yinc;
			if (flipy)  { ystart = high - 1;  yend = -1;     yinc = -1; }
			else        { ystart = 0;         yend = high;   yinc = +1; }

			for (int j = ystart; j != yend; j += yinc)
			{
				for (int i = xstart; i != xend; i += xinc)
				{
					gfx->transpen(bitmap,cliprect, tnum + loopnum, colr, flipx, flipy, xpos + 16 * i, ypos + 16 * j, 0);
					loopnum++;
				}
			}
		}
		/* end drawing */
		listcntr++;
		if (listdat & 0x4000)
			break;
	}
}

void psikyo4_state::video_start()
{
	m_gfxdecode->gfx(0)->set_granularity(32); /* 256 colour sprites with palette selectable on 32 colour boundaries */
}
