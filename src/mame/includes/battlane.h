// license:BSD-3-Clause
// copyright-holders:Paul Leaman
/***************************************************************************

    Battle Lane Vol. 5

***************************************************************************/

#include "emupal.h"
#include "tilemap.h"

class battlane_state : public driver_device
{
public:
	battlane_state(const machine_config &mconfig, device_type type, const char *tag) :
		driver_device(mconfig, type, tag),
		m_tileram(*this, "tileram"),
		m_spriteram(*this, "spriteram"),
		m_maincpu(*this, "maincpu"),
		m_subcpu(*this, "sub"),
		m_gfxdecode(*this, "gfxdecode"),
		m_palette(*this, "palette")
	{ }

	void battlane(machine_config &config);

protected:
	virtual void machine_start() override;
	virtual void machine_reset() override;
	virtual void video_start() override;

private:
	/* memory pointers */
	required_shared_ptr<uint8_t> m_tileram;
	required_shared_ptr<uint8_t> m_spriteram;

	/* video-related */
	tilemap_t     *m_bg_tilemap = nullptr;
	bitmap_ind8 m_screen_bitmap = 0;
	int         m_video_ctrl = 0;
	int         m_cpu_control = 0;  /* CPU interrupt control register */

	/* devices */
	required_device<cpu_device> m_maincpu;
	required_device<cpu_device> m_subcpu;
	required_device<gfxdecode_device> m_gfxdecode;
	required_device<palette_device> m_palette;

	void battlane_cpu_command_w(uint8_t data);
	void battlane_palette_w(offs_t offset, uint8_t data);
	void battlane_scrollx_w(uint8_t data);
	void battlane_scrolly_w(uint8_t data);
	void battlane_tileram_w(offs_t offset, uint8_t data);
	void battlane_spriteram_w(offs_t offset, uint8_t data);
	void battlane_bitmap_w(offs_t offset, uint8_t data);
	void battlane_video_ctrl_w(uint8_t data);
	TILE_GET_INFO_MEMBER(get_tile_info_bg);
	TILEMAP_MAPPER_MEMBER(battlane_tilemap_scan_rows_2x2);
	uint32_t screen_update_battlane(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect);
	INTERRUPT_GEN_MEMBER(battlane_cpu1_interrupt);
	void draw_sprites(bitmap_ind16 &bitmap, const rectangle &cliprect);
	void draw_fg_bitmap(bitmap_ind16 &bitmap, const rectangle &cliprect);
	void battlane_map(address_map &map);
};
