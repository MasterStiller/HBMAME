// license:BSD-3-Clause
// copyright-holders:Alex Pasadyn, Zsolt Vasvari, Ernesto Corvi, Aaron Giles
// thanks-to:Kurt Mahan
/*************************************************************************

    Driver for Midway T-unit games.

**************************************************************************/
#ifndef MAME_INCLUDES_MIDTUNIT_H
#define MAME_INCLUDES_MIDTUNIT_H

#pragma once

#include "audio/dcs.h"
#include "audio/williams.h"
#include "video/midtunit.h"

#include "cpu/tms34010/tms34010.h"
#include "emupal.h"


class midtunit_state : public driver_device
{
public:
	midtunit_state(const machine_config &mconfig, device_type type, const char *tag) :
		driver_device(mconfig, type, tag),
		m_maincpu(*this, "maincpu"),
		m_video(*this, "video"),
		m_dcs(*this, "dcs"),
		m_palette(*this, "palette"),
		m_gfxrom(*this, "gfxrom"),
		m_cvsd_sound(*this, "cvsd"),
		m_adpcm_sound(*this, "adpcm"),
		m_nvram(*this, "nvram")
	{ }

	void tunit_core(machine_config &config);
	void tunit_adpcm(machine_config &config);
	void tunit_dcs(machine_config &config);

	void init_mktunit();
	void init_mkturbo();
	void init_nbajamte();
	void init_nbajam();
	void init_jdreddp();
	void init_mk2();

protected:
	void machine_reset() override;

	required_device<tms340x0_device> m_maincpu;
	required_device<midtunit_video_device> m_video;
	optional_device<dcs_audio_device> m_dcs;
	required_device<palette_device> m_palette;
	required_memory_region m_gfxrom;

private:
	optional_device<williams_cvsd_sound_device> m_cvsd_sound;
	optional_device<williams_adpcm_sound_device> m_adpcm_sound;

	required_shared_ptr<uint16_t> m_nvram;

	void midtunit_cmos_enable_w(uint16_t data);
	void midtunit_cmos_w(offs_t offset, uint16_t data, uint16_t mem_mask = ~0);
	uint16_t midtunit_cmos_r(offs_t offset);
	uint16_t midtunit_sound_state_r();
	uint16_t midtunit_sound_r();
	void midtunit_sound_w(offs_t offset, uint16_t data, uint16_t mem_mask = ~0);
	uint16_t mk_prot_r(offs_t offset);
	void mk_prot_w(offs_t offset, uint16_t data, uint16_t mem_mask = ~0);
	uint16_t mkturbo_prot_r();
	uint16_t mk2_prot_const_r();
	uint16_t mk2_prot_r();
	uint16_t mk2_prot_shift_r();
	void mk2_prot_w(offs_t offset, uint16_t data, uint16_t mem_mask = ~0);
	uint16_t nbajam_prot_r();
	void nbajam_prot_w(offs_t offset, uint16_t data);
	void jdredd_prot_w(offs_t offset, uint16_t data);
	uint16_t jdredd_prot_r(offs_t offset);

	void register_state_saving();
	void init_tunit_generic(int sound);
	void init_nbajam_common(int te_protection);

	/* CMOS-related variables */
	uint8_t    m_cmos_write_enable = 0;

	/* sound-related variables */
	uint8_t    m_chip_type = 0;
	uint8_t    m_fake_sound_state = 0;

	/* protection */
	uint8_t    m_mk_prot_index = 0;
	uint16_t   m_mk2_prot_data = 0;
	std::unique_ptr<uint8_t[]> m_hidden_ram;

	const uint32_t *m_nbajam_prot_table = nullptr;
	uint16_t   m_nbajam_prot_queue[5] = {};
	uint8_t    m_nbajam_prot_index = 0;

	const uint8_t *m_jdredd_prot_table = nullptr;
	uint8_t    m_jdredd_prot_index = 0;
	uint8_t    m_jdredd_prot_max = 0;

	void main_map(address_map &map);
};

#endif // MAME_INCLUDES_MIDTUNIT_H
