// license:BSD-3-Clause
// copyright-holders:Pierpaolo Prazzoli
#ifndef MAME_INCLUDES_ROLLRACE_H
#define MAME_INCLUDES_ROLLRACE_H

#pragma once

#include "emupal.h"
#include "tilemap.h"

class rollrace_state : public driver_device
{
public:
	rollrace_state(const machine_config &mconfig, device_type type, const char *tag) :
		driver_device(mconfig, type, tag),
		m_maincpu(*this, "maincpu"),
		m_gfxdecode(*this, "gfxdecode"),
		m_palette(*this, "palette"),
		m_videoram(*this, "videoram"),
		m_colorram(*this, "colorram"),
		m_spriteram(*this, "spriteram")
	{ }

	void rollace2(machine_config &config);
	void rollrace(machine_config &config);

protected:
	virtual void machine_start() override;
	virtual void video_start() override;

private:
	required_device<cpu_device> m_maincpu;
	required_device<gfxdecode_device> m_gfxdecode;
	required_device<palette_device> m_palette;
	required_shared_ptr<uint8_t> m_videoram;
	required_shared_ptr<uint8_t> m_colorram;
	required_shared_ptr<uint8_t> m_spriteram;

	tilemap_t *m_fg_tilemap = nullptr;
	int m_bkgpage = 0;
	int m_bkgflip = 0;
	int m_chrbank = 0;
	int m_bkgpen = 0;
	int m_bkgcol = 0;
	int m_flipy = 0;
	int m_flipx = 0;
	int m_spritebank = 0;

	uint8_t m_nmi_mask = 0;
	uint8_t m_sound_nmi_mask = 0;

	uint8_t fake_d800_r();
	void fake_d800_w(uint8_t data);
	DECLARE_WRITE_LINE_MEMBER(nmi_mask_w);
	void sound_nmi_mask_w(uint8_t data);
	DECLARE_WRITE_LINE_MEMBER(coin_counter_1_w);
	DECLARE_WRITE_LINE_MEMBER(coin_counter_2_w);
	DECLARE_WRITE_LINE_MEMBER(charbank_0_w);
	DECLARE_WRITE_LINE_MEMBER(charbank_1_w);
	void bkgpen_w(uint8_t data);
	DECLARE_WRITE_LINE_MEMBER(spritebank_w);
	void backgroundpage_w(uint8_t data);
	void backgroundcolor_w(uint8_t data);
	void flipy_w(uint8_t data);
	DECLARE_WRITE_LINE_MEMBER(flipx_w);
	void vram_w(offs_t offset, uint8_t data);
	void cram_w(offs_t offset, uint8_t data);
	TILE_GET_INFO_MEMBER(get_fg_tile_info);
	void tilemap_refresh_flip();

	void rollrace_palette(palette_device &palette) const;

	uint32_t screen_update(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect);

	DECLARE_WRITE_LINE_MEMBER(vblank_irq);
	INTERRUPT_GEN_MEMBER(sound_timer_irq);
	void rollrace_map(address_map &map);
	void rollrace_sound_map(address_map &map);
};

#endif // MAME_INCLUDES_ROLLRACE_H
