// license:BSD-3-Clause
// copyright-holders:Bryan McPhail
/*************************************************************************

    Bogey Manor

*************************************************************************/
#ifndef MAME_INCLUDES_BOGEYMAN_H
#define MAME_INCLUDES_BOGEYMAN_H

#pragma once

#include "sound/ay8910.h"
#include "emupal.h"
#include "tilemap.h"

class bogeyman_state : public driver_device
{
public:
	bogeyman_state(const machine_config &mconfig, device_type type, const char *tag) :
		driver_device(mconfig, type, tag),
		m_maincpu(*this, "maincpu"),
		m_gfxdecode(*this, "gfxdecode"),
		m_palette(*this, "palette"),
		m_ay1(*this, "ay1"),
		m_ay2(*this, "ay2"),
		m_videoram(*this, "videoram"),
		m_videoram2(*this, "videoram2"),
		m_colorram(*this, "colorram"),
		m_colorram2(*this, "colorram2"),
		m_spriteram(*this, "spriteram")
	{ }

	void bogeyman(machine_config &config);

protected:
	virtual void machine_start() override;
	virtual void machine_reset() override;
	virtual void video_start() override;

private:
	/* devices */
	required_device<cpu_device> m_maincpu;
	required_device<gfxdecode_device> m_gfxdecode;
	required_device<palette_device> m_palette;
	required_device<ay8910_device> m_ay1;
	required_device<ay8910_device> m_ay2;

	/* memory pointers */
	required_shared_ptr<uint8_t> m_videoram;
	required_shared_ptr<uint8_t> m_videoram2;
	required_shared_ptr<uint8_t> m_colorram;
	required_shared_ptr<uint8_t> m_colorram2;
	required_shared_ptr<uint8_t> m_spriteram;

	/* video-related */
	tilemap_t    *m_bg_tilemap = nullptr;
	tilemap_t    *m_fg_tilemap = nullptr;

	/* misc */
	int        m_psg_latch = 0;
	int        m_last_write = 0;
	int        m_colbank = 0;

	void ay8910_latch_w(uint8_t data);
	void ay8910_control_w(uint8_t data);
	void videoram_w(offs_t offset, uint8_t data);
	void colorram_w(offs_t offset, uint8_t data);
	void videoram2_w(offs_t offset, uint8_t data);
	void colorram2_w(offs_t offset, uint8_t data);
	void colbank_w(uint8_t data);

	TILE_GET_INFO_MEMBER(get_bg_tile_info);
	TILE_GET_INFO_MEMBER(get_fg_tile_info);

	void bogeyman_palette(palette_device &palette) const;
	uint32_t screen_update(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect);
	void draw_sprites(bitmap_ind16 &bitmap, const rectangle &cliprect);
	void bogeyman_map(address_map &map);
};

#endif // MAME_INCLUDES_BOGEYMAN_H
