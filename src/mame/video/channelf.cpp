// license:GPL-2.0+
// copyright-holders:Juergen Buchmueller, Frank Palazzolo, Sean Riddle
#include "emu.h"
#include "includes/channelf.h"

static constexpr rgb_t channelf_pens[] =
{
	{ 0x10, 0x10, 0x10 }, // black
	{ 0xfd, 0xfd, 0xfd }, // white
	{ 0xff, 0x31, 0x53 }, // red
	{ 0x02, 0xcc, 0x5d }, // green
	{ 0x4b, 0x3f, 0xf3 }, // blue
	{ 0xe0, 0xe0, 0xe0 }, // ltgray
	{ 0x91, 0xff, 0xa6 }, // ltgreen
	{ 0xce, 0xd0, 0xff }  // ltblue
};

#define BLACK   0
#define WHITE   1
#define RED     2
#define GREEN   3
#define BLUE    4
#define LTGRAY  5
#define LTGREEN 6
#define LTBLUE  7

static const uint16_t colormap[] = {
	BLACK,   WHITE, WHITE, WHITE,
	LTBLUE,  BLUE,  RED,   GREEN,
	LTGRAY,  BLUE,  RED,   GREEN,
	LTGREEN, BLUE,  RED,   GREEN,
};

/* Initialise the palette */
void channelf_state::channelf_palette(palette_device &palette) const
{
	palette.set_pen_colors(0, channelf_pens);
}

void channelf_state::video_start()
{
	m_p_videoram = memregion("vram")->base();
}

int channelf_state::recalc_palette_offset(int reg1, int reg2)
{
	/* Note: This is based on the decoding they used to   */
	/*       determine which palette this line is using   */

	return ((reg2&0x2)|(reg1>>1)) << 2;
}

uint32_t channelf_state::screen_update_ntsc(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect)
{
	return screen_update_channelf(screen, bitmap, cliprect, 4);
}

uint32_t channelf_state::screen_update_pal(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect)
{
	return screen_update_channelf(screen, bitmap, cliprect, 5);
}

uint32_t channelf_state::screen_update_channelf(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect, int y_rpt)
{
	uint16_t ma=0;

	for (uint8_t y = 0; y < 64; y++)
	{
		int const palette_offset = recalc_palette_offset(m_p_videoram[y*128+125]&3, m_p_videoram[y*128+126]&3);

		int y_end_pos = std::min(bitmap.height(), (y * y_rpt) + y_rpt);
		for (int y_pos = y * y_rpt; y_pos < y_end_pos; y_pos++)
		{
			uint16_t *p = &bitmap.pix(y_pos);
			for (uint16_t x = ma; x < ma + 128; x++)
			{
				uint8_t const col = palette_offset+(m_p_videoram[x|(y<<7)]&3);
				*p++ = colormap[col];
				*p++ = colormap[col];
			}
		}
		ma+=128;
	}
	return 0;
}
