// license:BSD-3-Clause
// copyright-holders:Aaron Giles
/***************************************************************************

    Atari "Stella on Steroids" hardware

****************************************************************************/

#include "emu.h"
#include "includes/beathead.h"



/*************************************
 *
 *  Video start/stop
 *
 *************************************/

void beathead_state::video_start()
{
	save_item(NAME(m_finescroll));
	save_item(NAME(m_vram_latch_offset));
	save_item(NAME(m_hsyncram_offset));
	save_item(NAME(m_hsyncram_start));
	save_item(NAME(m_hsyncram));

	m_hsyncram_offset = 0;
}



/*************************************
 *
 *  VRAM handling
 *
 *************************************/

void beathead_state::vram_transparent_w(offs_t offset, uint32_t data, uint32_t mem_mask)
{
	/* writes to this area appear to handle transparency */
	if (!(data & 0x000000ff)) mem_mask &= ~0x000000ff;
	if (!(data & 0x0000ff00)) mem_mask &= ~0x0000ff00;
	if (!(data & 0x00ff0000)) mem_mask &= ~0x00ff0000;
	if (!(data & 0xff000000)) mem_mask &= ~0xff000000;
	COMBINE_DATA(&m_videoram[offset]);
}


void beathead_state::vram_bulk_w(offs_t offset, uint32_t data, uint32_t mem_mask)
{
	/* it appears that writes to this area pass in a mask for 4 words in VRAM */
	/* allowing them to be filled from a preset latch */
	offset &= ~3;
	data = data & mem_mask & 0x0f0f0f0f;

	/* for now, just handle the bulk fill case; the others we'll catch later */
	if (data == 0x0f0f0f0f)
		m_videoram[offset+0] = m_videoram[offset+1] = m_videoram[offset+2] = m_videoram[offset+3] = *m_vram_bulk_latch;
	else
		logerror("Detected bulk VRAM write with mask %08x\n", data);
}


void beathead_state::vram_latch_w(offs_t offset, uint32_t data)
{
	/* latch the address */
	m_vram_latch_offset = (4 * offset) & 0x7ffff;
}


void beathead_state::vram_copy_w(offs_t offset, uint32_t data)
{
	/* copy from VRAM to VRAM, for 1024 bytes */
	offs_t dest_offset = (4 * offset) & 0x7ffff;
	memcpy(&m_videoram[dest_offset / 4], &m_videoram[m_vram_latch_offset / 4], 0x400);
}



/*************************************
 *
 *  Scroll offset handling
 *
 *************************************/

void beathead_state::finescroll_w(offs_t offset, uint32_t data, uint32_t mem_mask)
{
	uint32_t oldword = m_finescroll;
	uint32_t newword = COMBINE_DATA(&m_finescroll);

	/* if VBLANK is going off on a scanline other than the last, suspend time */
	if ((oldword & 8) && !(newword & 8) && m_screen->vpos() != 261)
	{
		logerror("Suspending time! (scanline = %d)\n", m_screen->vpos());
		m_maincpu->set_input_line(INPUT_LINE_HALT, ASSERT_LINE);
	}
}



/*************************************
 *
 *  HSYNC RAM handling
 *
 *************************************/

uint32_t beathead_state::hsync_ram_r(offs_t offset)
{
	/* offset 0 is probably write-only */
	if (offset == 0)
		logerror("%08X:Unexpected HSYNC RAM read at offset 0\n", m_maincpu->pcbase());

	/* offset 1 reads the data */
	else
		return m_hsyncram[m_hsyncram_offset];

	return 0;
}

void beathead_state::hsync_ram_w(offs_t offset, uint32_t data, uint32_t mem_mask)
{
	/* offset 0 selects the address, and can specify the start address */
	if (offset == 0)
	{
		COMBINE_DATA(&m_hsyncram_offset);
		if (m_hsyncram_offset & 0x800)
			m_hsyncram_start = m_hsyncram_offset & 0x7ff;
	}

	/* offset 1 writes the data */
	else
		COMBINE_DATA(&m_hsyncram[m_hsyncram_offset]);
}



/*************************************
 *
 *  Main screen refresher
 *
 *************************************/

uint32_t beathead_state::screen_update(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect)
{
	const auto videoram = util::little_endian_cast<const uint8_t>(m_videoram.target());

	// generate the final screen
	for (int y = cliprect.top(); y <= cliprect.bottom(); y++)
	{
		const pen_t pen_base = (*m_palette_select & 0x7f) * 256;
		uint16_t scanline[336];

		if (m_finescroll & 8)
		{
			// blanking
			for (int x = cliprect.left(); x <= cliprect.right(); x++)
				scanline[x] = pen_base;
		}
		else
		{
			// non-blanking
			const offs_t scanline_offset = m_vram_latch_offset + (m_finescroll & 3);
			offs_t src = scanline_offset + cliprect.left();

			// unswizzle the scanline first
			for (int x = cliprect.left(); x <= cliprect.right(); x++)
				scanline[x] = pen_base | videoram[src++];
		}

		// then draw it
		draw_scanline16(bitmap, cliprect.left(), y, cliprect.width(), &scanline[cliprect.left()], nullptr);
	}
	return 0;
}
