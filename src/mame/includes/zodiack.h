// license:BSD-3-Clause
// copyright-holders:Zsolt Vasvari

#include "cpu/z80/z80.h"
#include "machine/gen_latch.h"

class zodiack_state : public driver_device
{
public:
	zodiack_state(const machine_config &mconfig, device_type type, const char *tag)
		: driver_device(mconfig, type, tag),
			m_maincpu(*this, "maincpu"),
			m_audiocpu(*this, "audiocpu"),
			m_soundlatch(*this, "soundlatch")
	{ }

	void zodiack(machine_config &config);
	void percuss(machine_config &config);

protected:
	virtual void machine_start() override;
	virtual void machine_reset() override;

private:
	void nmi_mask_w(uint8_t data);
	void sound_nmi_enable_w(uint8_t data);
	void master_soundlatch_w(uint8_t data);
	void control_w(uint8_t data);

	// devices
	required_device<z80_device> m_maincpu;
	required_device<z80_device> m_audiocpu;
	required_device<generic_latch_8_device> m_soundlatch;

	// state
	uint8_t m_main_nmi_enabled = 0;
	uint8_t m_sound_nmi_enabled = 0;

	INTERRUPT_GEN_MEMBER(zodiack_sound_nmi_gen);
	DECLARE_WRITE_LINE_MEMBER(vblank_main_nmi_w);

	void io_map(address_map &map);
	void main_map(address_map &map);
	void sound_map(address_map &map);
};
