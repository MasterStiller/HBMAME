// license:BSD-3-Clause
// copyright-holders:David Haywood

#include "emu.h"
#include "includes/megadriv.h"
#include "sound/sn76496.h"

#include "imagedev/chd_cd.h"

#include "bus/generic/slot.h"
#include "bus/generic/carts.h"

#include "formats/imageutl.h"
#include "softlist.h"

/*************************************
 *
 *  Input handlers
 *
 *************************************/

/* These overwrite the MAME ones in DRIVER_INIT */
/* They're needed to give the users the choice between different controllers */
uint8_t md_cons_state::mess_md_io_read_data_port(offs_t offset)
{
	int portnum = offset;

	uint8_t retdata;
	int controller;
	uint8_t helper_6b = (m_megadrive_io_ctrl_regs[portnum] & 0x3f) | 0xc0; // bits 6 & 7 always come from megadrive_io_data_regs
	uint8_t helper_3b = (m_megadrive_io_ctrl_regs[portnum] & 0x7f) | 0x80; // bit 7 always comes from megadrive_io_data_regs

	switch (portnum)
	{
		case 0:
			controller = (m_io_ctrlr->read() & 0x0f);
			break;

		case 1:
			controller = (m_io_ctrlr->read() & 0xf0);
			break;

		default:
			controller = 0;
			break;
	}

	/* Are we using a 6 buttons Joypad? */
	if (controller)
	{
		if (m_megadrive_io_data_regs[portnum] & 0x40)
		{
			if (m_io_stage[portnum] == 2)
			{
				/* here we read B, C & the additional buttons */
				retdata = (m_megadrive_io_data_regs[portnum] & helper_6b) |
							((((m_io_pad6b[0][portnum] ? m_io_pad6b[0][portnum]->read() : 0) & 0x30) |
								((m_io_pad6b[1][portnum] ? m_io_pad6b[1][portnum]->read() : 0) & 0x0f)) & ~helper_6b);
			}
			else
			{
				/* here we read B, C & the directional buttons */
				retdata = (m_megadrive_io_data_regs[portnum] & helper_6b) |
							(((m_io_pad6b[0][portnum] ? m_io_pad6b[0][portnum]->read() : 0) & 0x3f) & ~helper_6b);
			}
		}
		else
		{
			if (m_io_stage[portnum] == 1)
			{
				/* here we read ((Start & A) >> 2) | 0x00 */
				retdata = (m_megadrive_io_data_regs[portnum] & helper_6b) |
							((((m_io_pad6b[0][portnum] ? m_io_pad6b[0][portnum]->read() : 0) & 0xc0) >> 2) & ~helper_6b);
			}
			else if (m_io_stage[portnum]==2)
			{
				/* here we read ((Start & A) >> 2) | 0x0f */
				retdata = (m_megadrive_io_data_regs[portnum] & helper_6b) |
							(((((m_io_pad6b[0][portnum] ? m_io_pad6b[0][portnum]->read() : 0) & 0xc0) >> 2) | 0x0f) & ~helper_6b);
			}
			else
			{
				/* here we read ((Start & A) >> 2) | Up and Down */
				retdata = (m_megadrive_io_data_regs[portnum] & helper_6b) |
							(((((m_io_pad6b[0][portnum] ? m_io_pad6b[0][portnum]->read() : 0) & 0xc0) >> 2) |
								((m_io_pad6b[0][portnum] ? m_io_pad6b[0][portnum]->read() : 0) & 0x03)) & ~helper_6b);
			}
		}

	//  osd_printf_debug("read io data port stage %d port %d %02x\n",mess_io_stage[portnum],portnum,retdata);

		retdata |= (retdata << 8);
	}
	/* Otherwise it's a 3 buttons Joypad */
	else
	{
		uint8_t svp_test = 0;
		if (m_cart)
			svp_test = m_cart->read_test();

		// handle test input for SVP test
		if (portnum == 0 && svp_test)
		{
			retdata = (m_megadrive_io_data_regs[0] & 0xc0);
		}
		else if (m_megadrive_io_data_regs[portnum] & 0x40)
		{
			/* here we read B, C & the directional buttons */
			retdata = (m_megadrive_io_data_regs[portnum] & helper_3b) |
						((((m_io_pad3b[portnum] ? m_io_pad3b[portnum]->read() : 0) & 0x3f) | 0x40) & ~helper_3b);
		}
		else
		{
			/* here we read ((Start & A) >> 2) | Up and Down */
			retdata = (m_megadrive_io_data_regs[portnum] & helper_3b) |
						(((((m_io_pad3b[portnum] ? m_io_pad3b[portnum]->read() : 0) & 0xc0) >> 2) |
							((m_io_pad3b[portnum] ? m_io_pad3b[portnum]->read() : 0) & 0x03) | 0x40) & ~helper_3b);
		}
	}

	return retdata;
}


void md_cons_state::mess_md_io_write_data_port(offs_t offset, uint16_t data)
{
	int portnum = offset;
	int controller;

	switch (portnum)
	{
		case 0:
			controller = (m_io_ctrlr->read() & 0x0f);
			break;

		case 1:
			controller = (m_io_ctrlr->read() & 0xf0);
			break;

		default:
			controller = 0;
			break;
	}

	if (controller)
	{
		if (m_megadrive_io_ctrl_regs[portnum] & 0x40)
		{
			if (((m_megadrive_io_data_regs[portnum] & 0x40) == 0x00) && ((data & 0x40) == 0x40))
			{
				m_io_stage[portnum]++;
				m_io_timeout[portnum]->adjust(m_maincpu->cycles_to_attotime(8192));
			}

		}
	}
	m_megadrive_io_data_regs[portnum] = data;
	//osd_printf_debug("Writing IO Data Register #%d data %04x\n",portnum,data);
}


/*************************************
 *
 *  Input ports
 *
 *************************************/


static INPUT_PORTS_START( md_base )
	PORT_START("PAD1_3B")       /* Joypad 1 (3 button + start) NOT READ DIRECTLY */
	PORT_BIT( 0x0001, IP_ACTIVE_LOW, IPT_JOYSTICK_UP ) PORT_8WAY PORT_PLAYER(1) PORT_CONDITION("CTRLSEL", 0x0f, EQUALS, 0x00)
	PORT_BIT( 0x0002, IP_ACTIVE_LOW, IPT_JOYSTICK_DOWN ) PORT_8WAY PORT_PLAYER(1) PORT_CONDITION("CTRLSEL", 0x0f, EQUALS, 0x00)
	PORT_BIT( 0x0004, IP_ACTIVE_LOW, IPT_JOYSTICK_LEFT ) PORT_8WAY PORT_PLAYER(1) PORT_CONDITION("CTRLSEL", 0x0f, EQUALS, 0x00)
	PORT_BIT( 0x0008, IP_ACTIVE_LOW, IPT_JOYSTICK_RIGHT ) PORT_8WAY PORT_PLAYER(1) PORT_CONDITION("CTRLSEL", 0x0f, EQUALS, 0x00)
	PORT_BIT( 0x0010, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_PLAYER(1) PORT_NAME("P1 B") PORT_CONDITION("CTRLSEL", 0x0f, EQUALS, 0x00)
	PORT_BIT( 0x0020, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_PLAYER(1) PORT_NAME("P1 C") PORT_CONDITION("CTRLSEL", 0x0f, EQUALS, 0x00)
	PORT_BIT( 0x0040, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_PLAYER(1) PORT_NAME("P1 A") PORT_CONDITION("CTRLSEL", 0x0f, EQUALS, 0x00)
	PORT_BIT( 0x0080, IP_ACTIVE_LOW, IPT_START ) PORT_PLAYER(1) PORT_CONDITION("CTRLSEL", 0x0f, EQUALS, 0x00)

	PORT_START("PAD2_3B")       /* Joypad 2 (3 button + start) NOT READ DIRECTLY */
	PORT_BIT( 0x0001, IP_ACTIVE_LOW, IPT_JOYSTICK_UP ) PORT_8WAY PORT_PLAYER(2) PORT_CONDITION("CTRLSEL", 0xf0, EQUALS, 0x00)
	PORT_BIT( 0x0002, IP_ACTIVE_LOW, IPT_JOYSTICK_DOWN ) PORT_8WAY PORT_PLAYER(2) PORT_CONDITION("CTRLSEL", 0xf0, EQUALS, 0x00)
	PORT_BIT( 0x0004, IP_ACTIVE_LOW, IPT_JOYSTICK_LEFT ) PORT_8WAY PORT_PLAYER(2) PORT_CONDITION("CTRLSEL", 0xf0, EQUALS, 0x00)
	PORT_BIT( 0x0008, IP_ACTIVE_LOW, IPT_JOYSTICK_RIGHT ) PORT_8WAY PORT_PLAYER(2) PORT_CONDITION("CTRLSEL", 0xf0, EQUALS, 0x00)
	PORT_BIT( 0x0010, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_PLAYER(2) PORT_NAME("P2 B") PORT_CONDITION("CTRLSEL", 0xf0, EQUALS, 0x00)
	PORT_BIT( 0x0020, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_PLAYER(2) PORT_NAME("P2 C") PORT_CONDITION("CTRLSEL", 0xf0, EQUALS, 0x00)
	PORT_BIT( 0x0040, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_PLAYER(2) PORT_NAME("P2 A") PORT_CONDITION("CTRLSEL", 0xf0, EQUALS, 0x00)
	PORT_BIT( 0x0080, IP_ACTIVE_LOW, IPT_START ) PORT_PLAYER(2) PORT_CONDITION("CTRLSEL", 0xf0, EQUALS, 0x00)

	PORT_START("PAD1_6B")       /* Joypad 1 (6 button + start + mode) NOT READ DIRECTLY */
	PORT_BIT( 0x0001, IP_ACTIVE_LOW, IPT_JOYSTICK_UP ) PORT_8WAY PORT_PLAYER(1) PORT_CONDITION("CTRLSEL", 0x0f, EQUALS, 0x01)
	PORT_BIT( 0x0002, IP_ACTIVE_LOW, IPT_JOYSTICK_DOWN ) PORT_8WAY PORT_PLAYER(1) PORT_CONDITION("CTRLSEL", 0x0f, EQUALS, 0x01)
	PORT_BIT( 0x0004, IP_ACTIVE_LOW, IPT_JOYSTICK_LEFT ) PORT_8WAY PORT_PLAYER(1) PORT_CONDITION("CTRLSEL", 0x0f, EQUALS, 0x01)
	PORT_BIT( 0x0008, IP_ACTIVE_LOW, IPT_JOYSTICK_RIGHT ) PORT_8WAY PORT_PLAYER(1) PORT_CONDITION("CTRLSEL", 0x0f, EQUALS, 0x01)
	PORT_BIT( 0x0010, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_PLAYER(1) PORT_NAME("P1 B") PORT_CONDITION("CTRLSEL", 0x0f, EQUALS, 0x01)
	PORT_BIT( 0x0020, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_PLAYER(1) PORT_NAME("P1 C") PORT_CONDITION("CTRLSEL", 0x0f, EQUALS, 0x01)
	PORT_BIT( 0x0040, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_PLAYER(1) PORT_NAME("P1 A") PORT_CONDITION("CTRLSEL", 0x0f, EQUALS, 0x01)
	PORT_BIT( 0x0080, IP_ACTIVE_LOW, IPT_START ) PORT_PLAYER(1) PORT_CONDITION("CTRLSEL", 0x0f, EQUALS, 0x01)

	PORT_START("EXTRA1")    /* Extra buttons for Joypad 1 (6 button + start + mode) NOT READ DIRECTLY */
	PORT_BIT( 0x0001, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_PLAYER(1) PORT_NAME("P1 Z") PORT_CONDITION("CTRLSEL", 0x0f, EQUALS, 0x01)
	PORT_BIT( 0x0002, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_PLAYER(1) PORT_NAME("P1 Y") PORT_CONDITION("CTRLSEL", 0x0f, EQUALS, 0x01)
	PORT_BIT( 0x0004, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_PLAYER(1) PORT_NAME("P1 X") PORT_CONDITION("CTRLSEL", 0x0f, EQUALS, 0x01)
	PORT_BIT( 0x0008, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_PLAYER(1) PORT_NAME("P1 Mode") PORT_CONDITION("CTRLSEL", 0x0f, EQUALS, 0x01)

	PORT_START("PAD2_6B")       /* Joypad 2 (6 button + start + mode) NOT READ DIRECTLY */
	PORT_BIT( 0x0001, IP_ACTIVE_LOW, IPT_JOYSTICK_UP ) PORT_8WAY PORT_PLAYER(2) PORT_CONDITION("CTRLSEL", 0xf0, EQUALS, 0x10)
	PORT_BIT( 0x0002, IP_ACTIVE_LOW, IPT_JOYSTICK_DOWN ) PORT_8WAY PORT_PLAYER(2) PORT_CONDITION("CTRLSEL", 0xf0, EQUALS, 0x10)
	PORT_BIT( 0x0004, IP_ACTIVE_LOW, IPT_JOYSTICK_LEFT ) PORT_8WAY PORT_PLAYER(2) PORT_CONDITION("CTRLSEL", 0xf0, EQUALS, 0x10)
	PORT_BIT( 0x0008, IP_ACTIVE_LOW, IPT_JOYSTICK_RIGHT ) PORT_8WAY PORT_PLAYER(2) PORT_CONDITION("CTRLSEL", 0xf0, EQUALS, 0x10)
	PORT_BIT( 0x0010, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_PLAYER(2) PORT_NAME("P2 B") PORT_CONDITION("CTRLSEL", 0xf0, EQUALS, 0x10)
	PORT_BIT( 0x0020, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_PLAYER(2) PORT_NAME("P2 C") PORT_CONDITION("CTRLSEL", 0xf0, EQUALS, 0x10)
	PORT_BIT( 0x0040, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_PLAYER(2) PORT_NAME("P2 A") PORT_CONDITION("CTRLSEL", 0xf0, EQUALS, 0x10)
	PORT_BIT( 0x0080, IP_ACTIVE_LOW, IPT_START ) PORT_PLAYER(2) PORT_CONDITION("CTRLSEL", 0xf0, EQUALS, 0x10)

	PORT_START("EXTRA2")    /* Extra buttons for Joypad 2 (6 button + start + mode) NOT READ DIRECTLY */
	PORT_BIT( 0x0001, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_PLAYER(2) PORT_NAME("P2 Z") PORT_CONDITION("CTRLSEL", 0xf0, EQUALS, 0x10)
	PORT_BIT( 0x0002, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_PLAYER(2) PORT_NAME("P2 Y") PORT_CONDITION("CTRLSEL", 0xf0, EQUALS, 0x10)
	PORT_BIT( 0x0004, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_PLAYER(2) PORT_NAME("P2 X") PORT_CONDITION("CTRLSEL", 0xf0, EQUALS, 0x10)
	PORT_BIT( 0x0008, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_PLAYER(2) PORT_NAME("P2 Mode") PORT_CONDITION("CTRLSEL", 0xf0, EQUALS, 0x10)

	PORT_START("RESET")     /* Buttons on Genesis Console */
	PORT_BIT( 0x0001, IP_ACTIVE_HIGH, IPT_SERVICE1 ) PORT_NAME("Reset Button") PORT_IMPULSE(1) // reset, resets 68k (and..?)
INPUT_PORTS_END

static INPUT_PORTS_START( md )
	PORT_INCLUDE( md_base )

	PORT_START("CTRLSEL")   /* Controller selection */
	PORT_CONFNAME( 0x0f, 0x00, "Player 1 Controller" )
	PORT_CONFSETTING( 0x00, "Joystick 3 Buttons" )
	PORT_CONFSETTING( 0x01, "Joystick 6 Buttons" )
//  PORT_CONFSETTING( 0x02, "Sega Mouse" )
/* there exists both a 2 buttons version of the Mouse (Jpn ver, to be used with RPGs, it
    can aslo be used as trackball) and a 3 buttons version (US ver, no trackball feats.) */
//  PORT_CONFSETTING( 0x03, "Sega Menacer" )
//  PORT_CONFSETTING( 0x04, "Konami Justifier" )
//  PORT_CONFSETTING( 0x05, "Team Player (Sega Multitap)" )
//  PORT_CONFSETTING( 0x06, "4-Play (EA Multitap)" )
//  PORT_CONFSETTING( 0x07, "J-Cart" )
	PORT_CONFNAME( 0xf0, 0x00, "Player 2 Controller" )
	PORT_CONFSETTING( 0x00, "Joystick 3 Buttons" )
	PORT_CONFSETTING( 0x10, "Joystick 6 Buttons" )
INPUT_PORTS_END

static INPUT_PORTS_START( megajet )
	PORT_INCLUDE( md_base )

	PORT_START("CTRLSEL")   /* Fixed controller setting for Player 1 */
	PORT_CONFNAME( 0x0f, 0x01, "Player 1 Controller" ) // Fixed
	PORT_CONFSETTING( 0x01, "Joystick 6 Buttons" )
	PORT_CONFNAME( 0xf0, 0x00, "Player 2 Controller" )
	PORT_CONFSETTING( 0x00, "Joystick 3 Buttons" )
	PORT_CONFSETTING( 0x10, "Joystick 6 Buttons" )
INPUT_PORTS_END

static INPUT_PORTS_START( gen_nomd )
	PORT_INCLUDE( megajet )

	PORT_MODIFY("RESET")     /* No reset button */
	PORT_BIT( 0x0001, IP_ACTIVE_HIGH, IPT_UNUSED )
INPUT_PORTS_END


/*************************************
 *
 *  Machine driver
 *
 *************************************/

void md_cons_state::machine_start()
{
	static const char *const pad6names[2][4] = {
		{ "PAD1_6B", "PAD2_6B", "UNUSED", "UNUSED" },
		{ "EXTRA1", "EXTRA2", "UNUSED", "UNUSED" }
	};
	static const char *const pad3names[4] = { "PAD1_3B", "PAD2_3B", "UNUSED", "UNUSED" };

	m_io_ctrlr = ioport("CTRLSEL");

	for (int i = 0; i < 4; i++)
	{
		m_io_pad3b[i] = ioport(pad3names[i]);
		m_io_pad6b[0][i] = ioport(pad6names[0][i]);
		m_io_pad6b[1][i] = ioport(pad6names[1][i]);
	}

	// setup timers for 6 button pads
	for (int i = 0; i < 3; i++)
		m_io_timeout[i] = timer_alloc(FUNC(md_base_state::io_timeout_timer_callback), this);

	m_vdp->stop_timers();

	if (m_cart)
		m_cart->save_nvram();

	if (m_z80snd)
		m_genz80.z80_run_timer = timer_alloc(FUNC(md_base_state::megadriv_z80_run_state), this);
}

void md_cons_state::install_cartslot()
{
	if (m_cart)
	{
		// for now m_cartslot is only in MD and not 32x and SegaCD
		m_maincpu->space(AS_PROGRAM).install_read_handler(0x000000, 0x7fffff, read16sm_delegate(*m_cart, FUNC(base_md_cart_slot_device::read)));
		m_maincpu->space(AS_PROGRAM).install_write_handler(0x000000, 0x7fffff, write16s_delegate(*m_cart, FUNC(base_md_cart_slot_device::write)));
		m_maincpu->space(AS_PROGRAM).install_readwrite_handler(0xa13000, 0xa130ff, read16sm_delegate(*m_cart, FUNC(base_md_cart_slot_device::read_a13)), write16sm_delegate(*m_cart, FUNC(base_md_cart_slot_device::write_a13)));
		m_maincpu->space(AS_PROGRAM).install_readwrite_handler(0xa15000, 0xa150ff, read16sm_delegate(*m_cart, FUNC(base_md_cart_slot_device::read_a15)), write16sm_delegate(*m_cart, FUNC(base_md_cart_slot_device::write_a15)));
//      m_maincpu->space(AS_PROGRAM).install_write_handler(0xa14000, 0xa14003, write16sm_delegate(*m_cart, FUNC(base_md_cart_slot_device::write_tmss_bank)));
	}
}

uint16_t md_cons_state::tmss_r(offs_t offset)
{
	if (offset < 0x4000 / 2)
		return m_tmss[offset];

	return 0xffff;
}

void md_cons_state::tmss_swap_w(uint16_t data)
{
	if (data & 0x0001)
	{
		install_cartslot();
		m_maincpu->space(AS_PROGRAM).install_write_handler(0xa14100, 0xa14101, write16smo_delegate(*this, FUNC(md_cons_state::tmss_swap_w)));
	}
	else
	{
		install_tmss();
	}
}


void md_cons_state::install_tmss()
{
	m_maincpu->space(AS_PROGRAM).unmap_readwrite(0x000000, 0x7fffff);
	m_maincpu->space(AS_PROGRAM).install_read_handler(0x000000, 0x7fffff, read16sm_delegate(*this, FUNC(md_cons_state::tmss_r)));

	m_maincpu->space(AS_PROGRAM).install_write_handler(0xa14100, 0xa14101, write16smo_delegate(*this, FUNC(md_cons_state::tmss_swap_w)));

}


void md_cons_slot_state::machine_start()
{
	md_cons_state::machine_start();

	// the SVP introduces some kind of DMA 'lag', which we have to compensate for, this is obvious even on gfx DMAd from ROM (the Speedometer)
	if (m_cart && (m_cart->get_type() == SEGA_SVP))
		m_vdp->set_dma_delay(2);

	if (m_tmss)
	{
		install_tmss();
	}
	else
	{
		install_cartslot();
	}
}

void md_cons_state::machine_reset()
{
	md_base_state::machine_reset();

	// if the system has a 32x, pause the extra CPUs until they are actually turned on
	if (m_32x)
		m_32x->pause_cpu();
}

void md_cons_cd_state::machine_start()
{
	md_cons_state::machine_start();

	// the segaCD introduces some kind of DMA 'lag', which we have to compensate for,
	// at least when reading wordram? we might need to check what mode we're in the DMA...
	m_vdp->set_dma_delay(2);
}

// same as screen_eof_megadriv but with addition of 32x and SegaCD/MegaCD pieces
WRITE_LINE_MEMBER(md_cons_state::screen_vblank_console)
{
	if (m_io_reset.read_safe(0) & 0x01)
		m_maincpu->pulse_input_line(INPUT_LINE_RESET, attotime::zero);

	// rising edge
	if (state)
	{
		if (!m_vdp->m_use_alt_timing)
		{
			bool mode3 = (m_vdp->get_imode() == 3);
			m_vdp->vdp_handle_eof();
			m_vdp->m_megadriv_scanline_timer->adjust(attotime::zero);

			if (m_32x)
				m_32x->screen_eof(mode3);

			if (m_segacd)
				m_segacd->update_total_scanlines(mode3);
		}
	}
}

void md_cons_slot_state::ms_megadriv(machine_config &config)
{
	md_ntsc(config);

	subdevice<screen_device>("megadriv")->screen_vblank().set(FUNC(md_cons_state::screen_vblank_console));

	MD_CART_SLOT(config, m_cart, md_cart, nullptr).set_must_be_loaded(true);
	SOFTWARE_LIST(config, "cart_list").set_original("megadriv");
}

void md_cons_slot_state::ms_megadpal(machine_config &config)
{
	md_pal(config);

	subdevice<screen_device>("megadriv")->screen_vblank().set(FUNC(md_cons_state::screen_vblank_console));

	MD_CART_SLOT(config, m_cart, md_cart, nullptr).set_must_be_loaded(true);
	SOFTWARE_LIST(config, "cart_list").set_original("megadriv");
}

void md_cons_slot_state::ms_megadriv2(machine_config &config)
{
	md2_ntsc(config);

	subdevice<screen_device>("megadriv")->screen_vblank().set(FUNC(md_cons_state::screen_vblank_console));

	MD_CART_SLOT(config, m_cart, md_cart, nullptr).set_must_be_loaded(true);
	SOFTWARE_LIST(config, "cart_list").set_original("megadriv");
}

void md_cons_slot_state::genesis_tmss(machine_config &config)
{
	ms_megadriv(config);
	subdevice<software_list_device>("cart_list")->set_filter("TMSS");
}

void md_cons_state::dcat16_megadriv(machine_config &config)
{
	dcat16_megadriv_base(config);

	subdevice<screen_device>("megadriv")->screen_vblank().set(FUNC(md_cons_state::screen_vblank_console));

//  has SD card slot instead?
//  MD_CART_SLOT(config, m_cart, md_cart, nullptr).set_must_be_loaded(true);
//  SOFTWARE_LIST(config, "cart_list").set_original("megadriv");
}

/*************************************
 *
 *  ROM definition(s)
 *
 *************************************/


/* we don't use the bios rom (it's not needed and only provides security on early models) */

ROM_START(genesis)
	ROM_REGION(MD_CPU_REGION_SIZE, "maincpu", ROMREGION_ERASEFF)
	ROM_REGION( 0x10000, "soundcpu", ROMREGION_ERASEFF)
ROM_END

ROM_START(megadriv)
	ROM_REGION(MD_CPU_REGION_SIZE, "maincpu", ROMREGION_ERASEFF)
	ROM_REGION( 0x10000, "soundcpu", ROMREGION_ERASEFF)
ROM_END

ROM_START(megadrij)
	ROM_REGION(MD_CPU_REGION_SIZE, "maincpu", ROMREGION_ERASEFF)
	ROM_REGION( 0x10000, "soundcpu", ROMREGION_ERASEFF)
ROM_END

ROM_START(genesis_tmss)
	ROM_REGION(MD_CPU_REGION_SIZE, "maincpu", ROMREGION_ERASEFF)
	ROM_REGION( 0x10000, "soundcpu", ROMREGION_ERASEFF)

	ROM_REGION16_BE(0x4000, "tmss", ROMREGION_ERASEFF)
	ROM_LOAD( "tmss_usa.bin", 0x0000,  0x4000, CRC(5f5e64eb) SHA1(453fca4e1db6fae4a10657c4451bccbb71955628) )
ROM_END

ROM_START(dcat16)
	ROM_REGION(0x800000, "maincpu", ROMREGION_ERASEFF)
	ROM_LOAD16_WORD_SWAP( "mg6025.u1", 0x0000,  0x800000, CRC(5453d673) SHA1(b9f8d849cbed81fe73525229f4897ccaeeb7a833) )

	ROM_REGION( 0x10000, "soundcpu", ROMREGION_ERASEFF)
ROM_END

ROM_START(megajet)
	ROM_REGION(MD_CPU_REGION_SIZE, "maincpu", ROMREGION_ERASEFF)
	ROM_REGION( 0x10000, "soundcpu", ROMREGION_ERASEFF)
ROM_END

ROM_START(gen_nomd)
	ROM_REGION(MD_CPU_REGION_SIZE, "maincpu", ROMREGION_ERASEFF)
	ROM_REGION( 0x10000, "soundcpu", ROMREGION_ERASEFF)
ROM_END

/*************************************
 *
 *  Driver initialization
 *
 *************************************/

void md_cons_state::init_mess_md_common()
{
	m_megadrive_io_read_data_port_ptr = read8sm_delegate(*this, FUNC(md_cons_state::mess_md_io_read_data_port));
	m_megadrive_io_write_data_port_ptr = write16sm_delegate(*this, FUNC(md_cons_state::mess_md_io_write_data_port));
}

void md_cons_state::init_genesis()
{
	init_megadriv();
	init_mess_md_common();

	if (m_32x)
	{
		m_32x->set_32x_pal(false);
		m_32x->set_framerate(60);
		m_32x->set_total_scanlines(262);
	}
	if (m_segacd)
	{
		m_segacd->set_framerate(60);
		m_segacd->set_total_scanlines(262);
	}

	m_version_hi_nibble = 0x80; // Export NTSC
	if (!m_segacd)
		m_version_hi_nibble |= 0x20;
}

void md_cons_state::init_md_eur()
{
	init_megadrie();
	init_mess_md_common();

	if (m_32x)
	{
		m_32x->set_32x_pal(true);
		m_32x->set_framerate(50);
		m_32x->set_total_scanlines(313);
	}
	if (m_segacd)
	{
		m_segacd->set_framerate(50);
		m_segacd->set_total_scanlines(313);
	}

	m_version_hi_nibble = 0xc0; // Export PAL
	if (!m_segacd)
		m_version_hi_nibble |= 0x20;
}

void md_cons_state::init_md_jpn()
{
	init_megadrij();
	init_mess_md_common();

	if (m_32x)
	{
		m_32x->set_32x_pal(false);
		m_32x->set_framerate(60);
		m_32x->set_total_scanlines(262);
	}
	if (m_segacd)
	{
		m_segacd->set_framerate(60);
		m_segacd->set_total_scanlines(262);
	}

	m_version_hi_nibble = 0x00; // JPN NTSC
	if (!m_segacd)
		m_version_hi_nibble |= 0x20;
}

/****************************************** 32X emulation ****************************************/

DEVICE_IMAGE_LOAD_MEMBER( md_cons_state::_32x_cart )
{
	uint32_t length;
	std::vector<uint8_t> temp_copy;
	uint16_t *ROM16;
	uint32_t *ROM32;
	int i;

	if (!image.loaded_through_softlist())
	{
		length = image.length();
		temp_copy.resize(length);
		image.fread(&temp_copy[0], length);
	}
	else
	{
		length = image.get_software_region_length("rom");
		temp_copy.resize(length);
		memcpy(&temp_copy[0], image.get_software_region("rom"), length);
	}

	/* Copy the cart image in the locations the driver expects */
	// Notice that, by using pick_integer, we are sure the code works on both LE and BE machines
	ROM16 = (uint16_t *) memregion("gamecart")->base();
	for (i = 0; i < length; i += 2)
		ROM16[i / 2] = pick_integer_be(&temp_copy[0], i, 2);

	ROM32 = (uint32_t *) memregion("gamecart_sh2")->base();
	for (i = 0; i < length; i += 4)
		ROM32[i / 4] = pick_integer_be(&temp_copy[0], i, 4);

	ROM16 = (uint16_t *) memregion("maincpu")->base();
	for (i = 0x00; i < length; i += 2)
		ROM16[i / 2] = pick_integer_be(&temp_copy[0], i, 2);

	return image_init_result::PASS;
}


void md_cons_state::_32x_scanline_callback(int x, uint32_t priority, uint32_t &lineptr)
{
	if (m_32x)
		m_32x->render_videobuffer_to_screenbuffer(x, priority, lineptr);
}

void md_cons_state::_32x_interrupt_callback(int scanline, int irq6)
{
	if (m_32x)
		m_32x->interrupt_cb(scanline, irq6);
}

void md_cons_state::_32x_scanline_helper_callback(int scanline)
{
	if (m_32x)
		m_32x->render_videobuffer_to_screenbuffer_helper(scanline);
}

void md_cons_state::genesis_32x(machine_config &config)
{
	md_ntsc(config);

	m_vdp->set_md_32x_scanline(FUNC(md_cons_state::_32x_scanline_callback));
	m_vdp->set_md_32x_scanline_helper(FUNC(md_cons_state::_32x_scanline_helper_callback));
	m_vdp->set_md_32x_interrupt(FUNC(md_cons_state::_32x_interrupt_callback));
	m_vdp->reset_routes();
	m_vdp->add_route(ALL_OUTPUTS, "lspeaker", (0.50)/2);
	m_vdp->add_route(ALL_OUTPUTS, "rspeaker", (0.50)/2);

	SEGA_32X_NTSC(config, m_32x, (MASTER_CLOCK_NTSC * 3) / 7, m_maincpu, m_scan_timer);
	m_32x->set_screen("megadriv");
	m_32x->add_route(0, "lspeaker", 1.00);
	m_32x->add_route(1, "rspeaker", 1.00);

	subdevice<screen_device>("megadriv")->screen_vblank().set(FUNC(md_cons_state::screen_vblank_console));

	// we need to remove and re-add the YM because the balance is different
	// due to MAME having severe issues if the dac output is > 0.40? (sound is corrupted even if DAC is silent?!)
	m_ymsnd->reset_routes();
	m_ymsnd->add_route(0, "lspeaker", (0.50)/2);
	m_ymsnd->add_route(1, "rspeaker", (0.50)/2);

	generic_cartslot_device &cartslot(GENERIC_CARTSLOT(config, "cartslot", generic_plain_slot, "_32x_cart", "32x,bin"));
	cartslot.set_must_be_loaded(true);
	cartslot.set_device_load(FUNC(md_cons_state::_32x_cart));

	SOFTWARE_LIST(config, "cart_list").set_original("32x").set_filter("NTSC-U");
}


void md_cons_state::mdj_32x(machine_config &config)
{
	md_ntsc(config);

	m_vdp->set_md_32x_scanline(FUNC(md_cons_state::_32x_scanline_callback));
	m_vdp->set_md_32x_scanline_helper(FUNC(md_cons_state::_32x_scanline_helper_callback));
	m_vdp->set_md_32x_interrupt(FUNC(md_cons_state::_32x_interrupt_callback));
	m_vdp->reset_routes();
	m_vdp->add_route(ALL_OUTPUTS, "lspeaker", (0.50)/2);
	m_vdp->add_route(ALL_OUTPUTS, "rspeaker", (0.50)/2);

	SEGA_32X_NTSC(config, m_32x, (MASTER_CLOCK_NTSC * 3) / 7, m_maincpu, m_scan_timer);
	m_32x->set_screen("megadriv");
	m_32x->add_route(0, "lspeaker", 1.00);
	m_32x->add_route(1, "rspeaker", 1.00);

	subdevice<screen_device>("megadriv")->screen_vblank().set(FUNC(md_cons_state::screen_vblank_console));

	// we need to remove and re-add the sound system because the balance is different
	// due to MAME having severe issues if the dac output is > 0.40? (sound is corrupted even if DAC is silent?!)
	m_ymsnd->reset_routes();
	m_ymsnd->add_route(0, "lspeaker", (0.50)/2);
	m_ymsnd->add_route(1, "rspeaker", (0.50)/2);

	generic_cartslot_device &cartslot(GENERIC_CARTSLOT(config, "cartslot", generic_plain_slot, "_32x_cart", "32x,bin"));
	cartslot.set_must_be_loaded(true);
	cartslot.set_device_load(FUNC(md_cons_state::_32x_cart));

	SOFTWARE_LIST(config, "cart_list").set_original("32x").set_filter("NTSC-J");
}


void md_cons_state::md_32x(machine_config &config)
{
	md_pal(config);

	m_vdp->set_md_32x_scanline(FUNC(md_cons_state::_32x_scanline_callback));
	m_vdp->set_md_32x_scanline_helper(FUNC(md_cons_state::_32x_scanline_helper_callback));
	m_vdp->set_md_32x_interrupt(FUNC(md_cons_state::_32x_interrupt_callback));
	m_vdp->reset_routes();
	m_vdp->add_route(ALL_OUTPUTS, "lspeaker", (0.50)/2);
	m_vdp->add_route(ALL_OUTPUTS, "rspeaker", (0.50)/2);

	SEGA_32X_PAL(config, m_32x, (MASTER_CLOCK_PAL * 3) / 7, m_maincpu, m_scan_timer);
	m_32x->set_screen("megadriv");
	m_32x->add_route(0, "lspeaker", 1.00);
	m_32x->add_route(1, "rspeaker", 1.00);

	subdevice<screen_device>("megadriv")->screen_vblank().set(FUNC(md_cons_state::screen_vblank_console));

	// we need to remove and re-add the sound system because the balance is different
	// due to MAME having severe issues if the dac output is > 0.40? (sound is corrupted even if DAC is silent?!)
	m_ymsnd->reset_routes();
	m_ymsnd->add_route(0, "lspeaker", (0.50)/2);
	m_ymsnd->add_route(1, "rspeaker", (0.50)/2);

	generic_cartslot_device &cartslot(GENERIC_CARTSLOT(config, "cartslot", generic_plain_slot, "_32x_cart", "32x,bin"));
	cartslot.set_must_be_loaded(true);
	cartslot.set_device_load(FUNC(md_cons_state::_32x_cart));

	SOFTWARE_LIST(config, "cart_list").set_original("32x").set_filter("PAL");
}



#define _32X_ROMS \
	ROM_REGION16_BE( 0x400000, "gamecart", ROMREGION_ERASE00 ) /* 68000 Code */ \
	ROM_REGION32_BE( 0x400000, "gamecart_sh2", ROMREGION_ERASE00 ) /* Copy for the SH2 */ \
	ROM_REGION16_BE( 0x400000, "32x_68k_bios", 0 ) /* 68000 Code */ \
	ROM_LOAD( "32x_g_bios.bin", 0x000000,  0x000100, CRC(5c12eae8) SHA1(dbebd76a448447cb6e524ac3cb0fd19fc065d944) ) \
	ROM_REGION16_BE( 0x400000, "maincpu", ROMREGION_ERASE00 ) \
	/* temp, rom should only be visible here when one of the regs is set, tempo needs it */ \
	ROM_COPY( "32x_68k_bios", 0x0, 0x0, 0x100) \
	ROM_REGION32_BE( 0x400000, "master", 0 ) /* SH2 Code */ \
	ROM_SYSTEM_BIOS( 0, "retail", "Mars Version 1.0 (retail)" ) \
	ROMX_LOAD( "32x_m_bios.bin", 0x000000,  0x000800, CRC(dd9c46b8) SHA1(1e5b0b2441a4979b6966d942b20cc76c413b8c5e), ROM_BIOS(0) ) \
	ROM_SYSTEM_BIOS( 1, "sdk", "Mars Version 1.0 (early sdk)" ) \
	ROMX_LOAD( "32x_m_bios_sdk.bin", 0x000000,  0x000800, BAD_DUMP CRC(c7102c53) SHA1(ed73a47f186b373b8eff765f84ef26c3d9ef6cb0), ROM_BIOS(1) ) \
	ROM_REGION32_BE( 0x400000, "slave", 0 ) /* SH2 Code */ \
	ROM_LOAD( "32x_s_bios.bin", 0x000000,  0x000400, CRC(bfda1fe5) SHA1(4103668c1bbd66c5e24558e73d4f3f92061a109a) )

ROM_START( 32x )
	_32X_ROMS
ROM_END

ROM_START( 32xe )
	_32X_ROMS
ROM_END

ROM_START( 32xj )
	_32X_ROMS
ROM_END


/****************************************** SegaCD emulation ****************************************/

void md_cons_cd_state::genesis_scd(machine_config &config)
{
	md_ntsc(config);

	subdevice<screen_device>("megadriv")->screen_vblank().set(FUNC(md_cons_state::screen_vblank_console));

	SEGA_SEGACD_US(config, m_segacd, 0);
	m_segacd->set_palette("gen_vdp:gfx_palette");
	m_segacd->set_hostcpu(m_maincpu);
	m_segacd->set_screen("megadriv");

	config.set_perfect_quantum("segacd:segacd_68k"); // perfect sync to the fastest cpu

	CDROM(config, "cdrom").set_interface("scd_cdrom");

	SOFTWARE_LIST(config, "cd_list").set_original("segacd");
}

void md_cons_cd_state::genesis2_scd(machine_config &config)
{
	md2_ntsc(config);

	subdevice<screen_device>("megadriv")->screen_vblank().set(FUNC(md_cons_state::screen_vblank_console));

	SEGA_SEGACD_US(config, m_segacd, 0);
	m_segacd->set_palette("gen_vdp:gfx_palette");
	m_segacd->set_hostcpu(m_maincpu);
	m_segacd->set_screen("megadriv");

	config.set_perfect_quantum("segacd:segacd_68k"); // perfect sync to the fastest cpu

	CDROM(config, "cdrom").set_interface("scd_cdrom");

	SOFTWARE_LIST(config, "cd_list").set_original("segacd");
}

void md_cons_cd_state::md_scd(machine_config &config)
{
	md_pal(config);

	subdevice<screen_device>("megadriv")->screen_vblank().set(FUNC(md_cons_state::screen_vblank_console));

	SEGA_SEGACD_EUROPE(config, m_segacd, 0);
	m_segacd->set_palette("gen_vdp:gfx_palette");
	m_segacd->set_hostcpu(m_maincpu);
	m_segacd->set_screen("megadriv");

	config.set_perfect_quantum("segacd:segacd_68k"); // perfect sync to the fastest cpu

	CDROM(config, "cdrom").set_interface("scd_cdrom");

	SOFTWARE_LIST(config, "cd_list").set_original("megacd");
}

void md_cons_cd_state::md2_scd(machine_config &config)
{
	md2_pal(config);

	subdevice<screen_device>("megadriv")->screen_vblank().set(FUNC(md_cons_state::screen_vblank_console));

	SEGA_SEGACD_EUROPE(config, m_segacd, 0);
	m_segacd->set_palette("gen_vdp:gfx_palette");
	m_segacd->set_hostcpu(m_maincpu);
	m_segacd->set_screen("megadriv");

	config.set_perfect_quantum("segacd:segacd_68k"); // perfect sync to the fastest cpu

	CDROM(config, "cdrom").set_interface("scd_cdrom");

	SOFTWARE_LIST(config, "cd_list").set_original("megacd");
}

void md_cons_cd_state::mdj_scd(machine_config &config)
{
	md_ntsc(config);

	subdevice<screen_device>("megadriv")->screen_vblank().set(FUNC(md_cons_state::screen_vblank_console));

	SEGA_SEGACD_JAPAN(config, m_segacd, 0);
	m_segacd->set_palette("gen_vdp:gfx_palette");
	m_segacd->set_hostcpu(m_maincpu);
	m_segacd->set_screen("megadriv");

	config.set_perfect_quantum("segacd:segacd_68k"); // perfect sync to the fastest cpu

	CDROM(config, "cdrom").set_interface("scd_cdrom");

	SOFTWARE_LIST(config, "cd_list").set_original("megacdj");
}

void md_cons_cd_state::md2j_scd(machine_config &config)
{
	md2_ntsc(config);

	subdevice<screen_device>("megadriv")->screen_vblank().set(FUNC(md_cons_state::screen_vblank_console));

	SEGA_SEGACD_JAPAN(config, m_segacd, 0);
	m_segacd->set_palette("gen_vdp:gfx_palette");
	m_segacd->set_hostcpu(m_maincpu);
	m_segacd->set_screen("megadriv");

	config.set_perfect_quantum("segacd:segacd_68k"); // perfect sync to the fastest cpu

	CDROM(config, "cdrom").set_interface("scd_cdrom");

	SOFTWARE_LIST(config, "cd_list").set_original("megacdj");
}

/******************SEGA CD + 32X****************************/

void md_cons_cd_state::genesis_32x_scd(machine_config &config)
{
	genesis_32x(config);

	SEGA_SEGACD_US(config, m_segacd, 0);
	m_segacd->set_palette("gen_vdp:gfx_palette");
	m_segacd->set_hostcpu(m_maincpu);
	m_segacd->set_screen("megadriv");

	config.set_perfect_quantum("segacd:segacd_68k"); // perfect sync to the fastest cpu

	CDROM(config, "cdrom").set_interface("scd_cdrom");

	config.device_remove("cartslot");
	GENERIC_CARTSLOT(config, "cartslot", generic_plain_slot, "_32x_cart", "32x,bin").set_device_load(FUNC(md_cons_state::_32x_cart));

	//config.m_perfect_cpu_quantum = subtag("32x_master_sh2");
	SOFTWARE_LIST(config, "cd_list").set_original("segacd");
}

void md_cons_cd_state::md_32x_scd(machine_config &config)
{
	md_32x(config);

	SEGA_SEGACD_EUROPE(config, m_segacd, 0);
	m_segacd->set_palette("gen_vdp:gfx_palette");
	m_segacd->set_hostcpu(m_maincpu);
	m_segacd->set_screen("megadriv");

	config.set_perfect_quantum("segacd:segacd_68k"); // perfect sync to the fastest cpu

	CDROM(config, "cdrom").set_interface("scd_cdrom");

	config.device_remove("cartslot");
	GENERIC_CARTSLOT(config, "cartslot", generic_plain_slot, "_32x_cart", "32x,bin").set_device_load(FUNC(md_cons_state::_32x_cart));

	//config.m_perfect_cpu_quantum = subtag("32x_master_sh2");
	SOFTWARE_LIST(config, "cd_list").set_original("megacd");
}

void md_cons_cd_state::mdj_32x_scd(machine_config &config)
{
	mdj_32x(config);

	SEGA_SEGACD_JAPAN(config, m_segacd, 0);
	m_segacd->set_palette("gen_vdp:gfx_palette");
	m_segacd->set_hostcpu(m_maincpu);
	m_segacd->set_screen("megadriv");

	config.set_perfect_quantum("segacd:segacd_68k"); // perfect sync to the fastest cpu

	CDROM(config, "cdrom").set_interface("scd_cdrom");

	config.device_remove("cartslot");
	GENERIC_CARTSLOT(config, "cartslot", generic_plain_slot, "_32x_cart", "32x,bin").set_device_load(FUNC(md_cons_state::_32x_cart));

	//config.m_perfect_cpu_quantum = subtag("32x_master_sh2");
	SOFTWARE_LIST(config, "cd_list").set_original("megacdj");
}

/* We need proper names for most of these BIOS ROMs! */
ROM_START( segacd )
	ROM_REGION16_BE( 0x400000, "maincpu", ROMREGION_ERASE00 )
	/* v1.10 confirmed dump by dead_screem */
	ROM_LOAD( "mpr-15045b.bin", 0x000000,  0x020000, CRC(c6d10268) SHA1(f4f315adcef9b8feb0364c21ab7f0eaf5457f3ed) )
ROM_END

ROM_START( megacd )
	ROM_REGION16_BE( 0x400000, "maincpu", ROMREGION_ERASE00 )
	/* v1.00, confirmed good dump */
	ROM_LOAD( "megacd_model1_bios_1_00_e.bin", 0x000000,  0x020000, CRC(529ac15a) SHA1(f891e0ea651e2232af0c5c4cb46a0cae2ee8f356) )
ROM_END

ROM_START( megacdj )
	ROM_REGION16_BE( 0x400000, "maincpu", ROMREGION_ERASE00 )
	ROM_DEFAULT_BIOS("v100g")   // this seems the only revision where the cursor in CD menu works, allowing to boot games
	/* Confirmed by ElBarto */
	ROM_SYSTEM_BIOS(0, "v100s", "v1.00S")
	ROMX_LOAD( "mpr-14088h.bin", 0x000000,  0x020000, CRC(3773d5aa) SHA1(bbf729a1aaa1667b783749299e1ad932aaf5f253), ROM_BIOS(0) | ROM_GROUPWORD | ROM_REVERSE)
	/* Confirmed by ElBarto */
	ROM_SYSTEM_BIOS(1, "v100g", "v1.00G")
	ROMX_LOAD( "epr-14088b.bin", 0x000000,  0x020000, CRC(69ed6ccd) SHA1(27d11c3836506f01ee81cd142c0cd8b51abebbd2), ROM_BIOS(1) | ROM_GROUPWORD | ROM_REVERSE)
	/* Confirmed by ElBarto */
	ROM_SYSTEM_BIOS(2, "v100l", "v1.00L")
	ROMX_LOAD( "mpr-14088c.bin", 0x000000,  0x020000, CRC(03134289) SHA1(d60cb5a53f26d6b13e354bc149217587f2301718), ROM_BIOS(2) | ROM_GROUPWORD | ROM_REVERSE)
	/* Confirmed by ElBarto */
	ROM_SYSTEM_BIOS(3, "v100o", "v1.00O")
	ROMX_LOAD( "epr-14088d.bin", 0x000000,  0x020000, CRC(dfa95ee9) SHA1(e13666c76fa0a2e94e2f651b26b0fd625bf55f07), ROM_BIOS(3) | ROM_GROUPWORD | ROM_REVERSE)
	ROM_SYSTEM_BIOS(4, "v100p", "v1.00P")   // CRC: e2e70bc8 when byteswapped
	ROMX_LOAD( "epr-14088e.bin", 0x000000,  0x020000, CRC(9d2da8f2) SHA1(4846f448160059a7da0215a5df12ca160f26dd69), ROM_BIOS(4) )
	// EEPROM had no SEGA's label, might be 14088(no rev) or 14088A
	ROM_SYSTEM_BIOS(5, "v100c", "v1.00C")   // CRC: c3b60c13 when byteswapped
	ROMX_LOAD( "100c.bin",       0x000000,  0x020000, CRC(41af44c4) SHA1(f30d109d1c2f7c9feaf38600c65834261db73d1f), ROM_BIOS(5) )
	ROM_SYSTEM_BIOS(6, "v111", "v1.11")   // CRC: e0a6179b when byteswapped
	ROMX_LOAD( "mpr-14837.bin",  0x000000,  0x020000, CRC(4be18ff6) SHA1(204758d5a64c24e96e1a9fe6bd82e1878fef7ade), ROM_BIOS(6) )
ROM_END

/* Asia bios, when run in USA region will show :
ERROR!
THIS IS A PAL-COMPATIBLE MEGA CD
FOR EXCLUSIVE USE IN SOUTHEAST ASIA.

Confirmed by Jakovasaur
*/
ROM_START( megacda )
	ROM_REGION16_BE( 0x400000, "maincpu", ROMREGION_ERASE00 )
	ROM_LOAD( "epr-14536h.bin", 0x000000,  0x020000, CRC(550f30bb) SHA1(e4193c6ae44c3cea002707d2a88f1fbcced664de))
ROM_END

ROM_START( segacd2 )
	ROM_REGION16_BE( 0x400000, "maincpu", ROMREGION_ERASE00 )
	ROM_SYSTEM_BIOS(0, "v211x", "Model 2 v2.11X")
	ROMX_LOAD( "mpr-15764-t.bin", 0x000000,  0x020000, CRC(2e49d72c) SHA1(328a3228c29fba244b9db2055adc1ec4f7a87e6b), ROM_BIOS(0) )
	ROM_SYSTEM_BIOS(1, "v200", "Model 2 v2.00") /* verified dump */
	ROMX_LOAD( "us_scd2_930314.bin", 0x000000,  0x020000, CRC(8af65f58) SHA1(5a8c4b91d3034c1448aac4b5dc9a6484fce51636), ROM_BIOS(1) )
	/* this is reportedly a bad dump, it has many differences from the verified dump and does not boot in Kega */
	/* ROMX_LOAD( "segacd_model2_bios_2_00_u.bin", 0x000000,  0x020000, CRC(340b4be4) SHA1(bd3ee0c8ab732468748bf98953603ce772612704), ROM_BIOS(1) ) */
	ROM_SYSTEM_BIOS(2, "v200w", "Model 2 v2.00W")
	ROMX_LOAD( "segacd_model2_bios_2_00w_u.bin", 0x000000,  0x020000, CRC(9f6f6276) SHA1(5adb6c3af218c60868e6b723ec47e36bbdf5e6f0), ROM_BIOS(2) )
ROM_END

/* v2.00, v2.00w confirmed good dumps by ElBarto */
/* v2.11x confirmed good dump by TwistedTom */
ROM_START( megacd2 )
	ROM_REGION16_BE( 0x400000, "maincpu", ROMREGION_ERASE00 )
	ROM_SYSTEM_BIOS(0, "v211x", "v2.11X")   // MK-4102A-50 Sony pcb
	ROMX_LOAD( "mpr-15811-t.bin", 0x000000,  0x020000, CRC(391a80d2) SHA1(aa4bb1803b06714fbc04d8a209161876796dd511), ROM_BIOS(0) | ROM_GROUPWORD | ROM_REVERSE )
	ROM_SYSTEM_BIOS(1, "v200w", "v2.00W")   // MK-4102-50 Funai pcb
	ROMX_LOAD( "mpr-15512a.bin", 0x000000,  0x020000, CRC(53f1757c) SHA1(67bf3970ca5a05fd5ce3d6c446789c5d971b98a4), ROM_BIOS(1) | ROM_GROUPWORD | ROM_REVERSE )
	ROM_SYSTEM_BIOS(2, "v200", "v2.00")     // ?
	ROMX_LOAD( "mpr-15512.bin", 0x000000,  0x020000, CRC(cb76f114) SHA1(939f173cadc41e996a3c34498da1bf55e7e18ff8), ROM_BIOS(2) | ROM_GROUPWORD | ROM_REVERSE )
ROM_END

/* Confirmed good dump by ElBarto */
ROM_START( megacd2j )
	ROM_REGION16_BE( 0x400000, "maincpu", ROMREGION_ERASE00 )
	ROM_SYSTEM_BIOS(0, "v200c", "v2.00C")
	ROMX_LOAD( "mpr-15398.bin", 0x000000,  0x020000, CRC(1e4344e6) SHA1(4d1251a6973d932e734ae5e8c6b9b55eb40e4143), ROM_BIOS(0) | ROM_GROUPWORD | ROM_REVERSE )
ROM_END

ROM_START( aiwamcd )
	ROM_REGION16_BE( 0x400000, "maincpu", ROMREGION_ERASE00 )
	/* v2.11 */
	ROM_LOAD( "mpr-15768-t.bin", 0x000000,  0x020000, CRC(8052c7a0) SHA1(219d284dcf63ce366a4dc6d1ff767a0d2eea283d) )
ROM_END

ROM_START( laseract )
	ROM_REGION16_BE( 0x400000, "maincpu", ROMREGION_ERASE00 )
	ROM_SYSTEM_BIOS(0, "v104", "v1.04")
	ROMX_LOAD( "laseractive_bios_1_04_u.bin", 0x000000,  0x020000, CRC(50cd3d23) SHA1(aa811861f8874775075bd3f53008c8aaf59b07db), ROM_BIOS(0) )
	ROM_SYSTEM_BIOS(1, "v102", "v1.02")
	ROMX_LOAD( "laseractive_bios_1_02_u.bin", 0x000000,  0x020000, CRC(3b10cf41) SHA1(8af162223bb12fc19b414f126022910372790103), ROM_BIOS(1) )
ROM_END

ROM_START( laseractj )
	ROM_REGION16_BE( 0x400000, "maincpu", ROMREGION_ERASE00 )
	ROM_SYSTEM_BIOS(0, "v105", "v1.05")
	ROMX_LOAD( "mega-ld 1.05 bios.bin", 0x000000,  0x020000, CRC(474aaa44) SHA1(b3b1d880e288b6dc79eec0ff1b0480c229ec141d), ROM_BIOS(0) )
	ROM_SYSTEM_BIOS(1, "v102", "v1.02")
	ROMX_LOAD( "laseractive_bios_1_02_j.bin", 0x000000,  0x020000, CRC(00eedb3a) SHA1(26237b333db4a4c6770297fa5e655ea95840d5d9), ROM_BIOS(1) )
ROM_END

ROM_START( xeye )
	ROM_REGION16_BE( 0x400000, "maincpu", ROMREGION_ERASE00 )
	/* v2.00 (US), confirmed good with a chip dump */
	ROM_LOAD( "g304.bin", 0x000000,  0x020000, CRC(290f8e33) SHA1(651f14d5a5e0ecb974a60c0f43b1d2006323fb09) )
ROM_END

ROM_START( wmega )
	ROM_REGION16_BE( 0x400000, "maincpu", ROMREGION_ERASE00 )
	/* v1.00 (Japan NTSC) Sega BIOS, chip-dumped */
	ROM_LOAD( "g301.bin", 0x000000,  0x020000, CRC(d21fe71d) SHA1(3fc9358072f74bd24e3e297ea11b2bf15a7af891) )
ROM_END

ROM_START( wmegam2 )
	ROM_REGION16_BE( 0x400000, "maincpu", ROMREGION_ERASE00 )
	/* v2.00 */
	ROM_LOAD( "wondermega_m2_bios_2_00_j.bin", 0x000000,  0x020000, CRC(2b19972f) SHA1(b3f32e409bd5508c89ed8be33d41a58d791d0e5d) )
ROM_END

ROM_START( cdx )
	ROM_REGION16_BE( 0x400000, "maincpu", ROMREGION_ERASE00 )
	/* v2.21X */
	ROM_LOAD( "segacdx_bios_2_21_u.bin", 0x000000,  0x020000, CRC(d48c44b5) SHA1(2b125c0545afa089b617f2558e686ea723bdc06e) )
ROM_END

ROM_START( multmega )
	ROM_REGION16_BE( 0x400000, "maincpu", ROMREGION_ERASE00 )
	/* v2.21X */
	ROM_LOAD( "opr-16140.bin", 0x000000,  0x020000, CRC(aacb851e) SHA1(75548ac9aaa6e81224499f9a1403b2b42433f5b7) )
	/* the below was marked "confirmed good dump", but 0x72 and 0x73 are 0xFF, indicating a bad dump made from memory */
	/* ROM_LOAD( "multimega_bios_2_21_e.bin", 0x000000,  0x020000, CRC(34d3cce1) SHA1(73fc9c014ad803e9e7d8076b3642a8a5224b3e51) ) */
ROM_END

/* some games use the 32x and SegaCD together to give better quality FMV */
ROM_START( 32x_scd )
	ROM_REGION16_BE( 0x400000, "maincpu", ROMREGION_ERASE00 )

	ROM_REGION16_BE( 0x400000, "gamecart", 0 ) /* 68000 Code */
	ROM_LOAD( "mpr-15764-t.bin", 0x000000,  0x020000, CRC(2e49d72c) SHA1(328a3228c29fba244b9db2055adc1ec4f7a87e6b) )

	ROM_REGION32_BE( 0x400000, "gamecart_sh2", 0 ) /* Copy for the SH2 */
	ROM_COPY( "gamecart", 0x000000, 0x0, 0x400000)

	ROM_REGION16_BE( 0x400000, "32x_68k_bios", 0 ) /* 68000 Code */
	ROM_LOAD( "32x_g_bios.bin", 0x000000,  0x000100, CRC(5c12eae8) SHA1(dbebd76a448447cb6e524ac3cb0fd19fc065d944) )

	ROM_REGION32_BE( 0x400000, "master", 0 ) /* SH2 Code */
	ROM_LOAD( "32x_m_bios.bin", 0x000000,  0x000800, CRC(dd9c46b8) SHA1(1e5b0b2441a4979b6966d942b20cc76c413b8c5e) )

	ROM_REGION32_BE( 0x400000, "slave", 0 ) /* SH2 Code */
	ROM_LOAD( "32x_s_bios.bin", 0x000000,  0x000400, CRC(bfda1fe5) SHA1(4103668c1bbd66c5e24558e73d4f3f92061a109a) )
ROM_END

ROM_START( 32x_mcd )
	ROM_REGION16_BE( 0x400000, "maincpu", ROMREGION_ERASE00 )

	ROM_REGION16_BE( 0x400000, "gamecart", 0 ) /* 68000 Code */
	ROM_LOAD( "megacd_model1_bios_1_00_e.bin", 0x000000,  0x020000, CRC(529ac15a) SHA1(f891e0ea651e2232af0c5c4cb46a0cae2ee8f356) )

	ROM_REGION32_BE( 0x400000, "gamecart_sh2", 0 ) /* Copy for the SH2 */
	ROM_COPY( "gamecart", 0x000000, 0x0, 0x400000)

	ROM_REGION16_BE( 0x400000, "32x_68k_bios", 0 ) /* 68000 Code */
	ROM_LOAD( "32x_g_bios.bin", 0x000000,  0x000100, CRC(5c12eae8) SHA1(dbebd76a448447cb6e524ac3cb0fd19fc065d944) )

	ROM_REGION32_BE( 0x400000, "master", 0 ) /* SH2 Code */
	ROM_LOAD( "32x_m_bios.bin", 0x000000,  0x000800, CRC(dd9c46b8) SHA1(1e5b0b2441a4979b6966d942b20cc76c413b8c5e) )

	ROM_REGION32_BE( 0x400000, "slave", 0 ) /* SH2 Code */
	ROM_LOAD( "32x_s_bios.bin", 0x000000,  0x000400, CRC(bfda1fe5) SHA1(4103668c1bbd66c5e24558e73d4f3f92061a109a) )
ROM_END

ROM_START( 32x_mcdj )
	ROM_REGION16_BE( 0x400000, "maincpu", ROMREGION_ERASE00 )

	ROM_REGION16_BE( 0x400000, "gamecart", 0 ) /* 68000 Code */
	ROM_DEFAULT_BIOS("v100g")   // this seems the only revision where the cursor in CD menu works, allowing to boot games
	/* Confirmed by ElBarto */
	ROM_SYSTEM_BIOS(0, "v100s", "v1.00S")
	ROMX_LOAD( "mpr-14088h.bin", 0x000000,  0x020000, CRC(3773d5aa) SHA1(bbf729a1aaa1667b783749299e1ad932aaf5f253), ROM_BIOS(0) | ROM_GROUPWORD | ROM_REVERSE)
	/* Confirmed by ElBarto */
	ROM_SYSTEM_BIOS(1, "v100g", "v1.00G")
	ROMX_LOAD( "epr-14088b.bin", 0x000000,  0x020000, CRC(69ed6ccd) SHA1(27d11c3836506f01ee81cd142c0cd8b51abebbd2), ROM_BIOS(1) | ROM_GROUPWORD | ROM_REVERSE)
	/* Confirmed by ElBarto */
	ROM_SYSTEM_BIOS(2, "v100l", "v1.00L")
	ROMX_LOAD( "mpr-14088c.bin", 0x000000,  0x020000, CRC(03134289) SHA1(d60cb5a53f26d6b13e354bc149217587f2301718), ROM_BIOS(2) | ROM_GROUPWORD | ROM_REVERSE)
	/* Confirmed by ElBarto */
	ROM_SYSTEM_BIOS(3, "v100o", "v1.00O")
	ROMX_LOAD( "epr-14088d.bin", 0x000000,  0x020000, CRC(dfa95ee9) SHA1(e13666c76fa0a2e94e2f651b26b0fd625bf55f07), ROM_BIOS(3) | ROM_GROUPWORD | ROM_REVERSE)
	ROM_SYSTEM_BIOS(4, "v100p", "v1.00P")   // CRC: e2e70bc8 when byteswapped
	ROMX_LOAD( "epr-14088e.bin", 0x000000,  0x020000, CRC(9d2da8f2) SHA1(4846f448160059a7da0215a5df12ca160f26dd69), ROM_BIOS(4) )

	ROM_REGION32_BE( 0x400000, "gamecart_sh2", 0 ) /* Copy for the SH2 */
	ROM_COPY( "gamecart", 0x000000, 0x0, 0x400000)

	ROM_REGION16_BE( 0x400000, "32x_68k_bios", 0 ) /* 68000 Code */
	ROM_LOAD( "32x_g_bios.bin", 0x000000,  0x000100, CRC(5c12eae8) SHA1(dbebd76a448447cb6e524ac3cb0fd19fc065d944) )

	ROM_REGION32_BE( 0x400000, "master", 0 ) /* SH2 Code */
	ROM_LOAD( "32x_m_bios.bin", 0x000000,  0x000800, CRC(dd9c46b8) SHA1(1e5b0b2441a4979b6966d942b20cc76c413b8c5e) )

	ROM_REGION32_BE( 0x400000, "slave", 0 ) /* SH2 Code */
	ROM_LOAD( "32x_s_bios.bin", 0x000000,  0x000400, CRC(bfda1fe5) SHA1(4103668c1bbd66c5e24558e73d4f3f92061a109a) )
ROM_END



/***************************************************************************

  Game driver(s)

***************************************************************************/

/*    YEAR  NAME          PARENT    COMPAT  MACHINE          INPUT     CLASS          INIT          COMPANY   FULLNAME */
CONS( 1989, genesis,      0,        0,      ms_megadriv,     md,       md_cons_slot_state, init_genesis, "Sega",   "Genesis (USA, NTSC)",  MACHINE_SUPPORTS_SAVE )
CONS( 1990, megadriv,     genesis,  0,      ms_megadpal,     md,       md_cons_slot_state, init_md_eur,  "Sega",   "Mega Drive (Europe, PAL)", MACHINE_SUPPORTS_SAVE )
CONS( 1988, megadrij,     genesis,  0,      ms_megadriv,     md,       md_cons_slot_state, init_md_jpn,  "Sega",   "Mega Drive (Japan, NTSC)", MACHINE_SUPPORTS_SAVE )

// 1990+ models had the TMSS security chip, leave this as a clone, it reduces compatibility and nothing more.
CONS( 1990, genesis_tmss, genesis,  0,      genesis_tmss,    md,       md_cons_slot_state, init_genesis, "Sega",   "Genesis (USA, NTSC, with TMSS chip)",  MACHINE_SUPPORTS_SAVE )

// the 32X plugged in the cart slot, games plugged into the 32x.  Maybe it should be handled as an expansion device?
CONS( 1994, 32x,          0,        0,      genesis_32x,     md,       md_cons_state, init_genesis, "Sega",   "Genesis with 32X (USA, NTSC)", MACHINE_NOT_WORKING )
CONS( 1994, 32xe,         32x,      0,      md_32x,          md,       md_cons_state, init_md_eur,  "Sega",   "Mega Drive with 32X (Europe, PAL)", MACHINE_NOT_WORKING )
CONS( 1994, 32xj,         32x,      0,      mdj_32x,         md,       md_cons_state, init_md_jpn,  "Sega",   "Mega Drive with 32X (Japan, NTSC)", MACHINE_NOT_WORKING )

// the SegaCD plugged into the expansion port..
CONS( 1992, segacd,       0,        0,      genesis_scd,     md,       md_cons_cd_state, init_genesis, "Sega",   "Sega CD (USA, NTSC)", MACHINE_NOT_WORKING )
CONS( 1993, megacd,       segacd,   0,      md_scd,          md,       md_cons_cd_state, init_md_eur,  "Sega",   "Mega-CD (Europe, PAL)", MACHINE_NOT_WORKING )
CONS( 1991, megacdj,      segacd,   0,      mdj_scd,         md,       md_cons_cd_state, init_md_jpn,  "Sega",   "Mega-CD (Japan, NTSC)", MACHINE_NOT_WORKING ) // this bios doesn't work with our ram interleave needed by a few games?!
CONS( 1991, megacda,      segacd,   0,      md_scd,          md,       md_cons_cd_state, init_md_eur,  "Sega",   "Mega-CD (Asia, PAL)", MACHINE_NOT_WORKING )
CONS( 1993, segacd2,      0,        0,      genesis_scd,     md,       md_cons_cd_state, init_genesis, "Sega",   "Sega CD 2 (USA, NTSC)", MACHINE_NOT_WORKING )
CONS( 1993, megacd2,      segacd2,  0,      md_scd,          md,       md_cons_cd_state, init_md_eur,  "Sega",   "Mega-CD 2 (Europe, PAL)", MACHINE_NOT_WORKING )
CONS( 1993, megacd2j,     segacd2,  0,      mdj_scd,         md,       md_cons_cd_state, init_md_jpn,  "Sega",   "Mega-CD 2 (Japan, NTSC)", MACHINE_NOT_WORKING )
CONS( 1994, aiwamcd,      segacd2,  0,      mdj_scd,         md,       md_cons_cd_state, init_md_jpn,  "AIWA",   "Mega-CD CSD-G1M (Japan, NTSC)", MACHINE_NOT_WORKING )
CONS( 1993, xeye,         0,        0,      genesis2_scd,    md,       md_cons_cd_state, init_genesis, "JVC",    "X'eye (USA, NTSC)", MACHINE_NOT_WORKING )
CONS( 1992, wmega,        xeye,     0,      mdj_scd,         md,       md_cons_cd_state, init_md_jpn,  "Sega",   "Wondermega (Japan, NTSC)", MACHINE_NOT_WORKING )
CONS( 1993, wmegam2,      xeye,     0,      md2j_scd,        md,       md_cons_cd_state, init_md_jpn,  "Victor", "Wondermega M2 (Japan, NTSC)", MACHINE_NOT_WORKING )
CONS( 1994, cdx,          0,        0,      genesis2_scd,    md,       md_cons_cd_state, init_genesis, "Sega",   "CDX (USA, NTSC)", MACHINE_NOT_WORKING )
CONS( 1994, multmega,     cdx,      0,      md2_scd,         md,       md_cons_cd_state, init_md_eur,  "Sega",   "Multi-Mega (Europe, PAL)", MACHINE_NOT_WORKING )

//32X plugged in the cart slot + SegaCD plugged into the expansion port..
CONS( 1994, 32x_scd,      0,        0,      genesis_32x_scd, md,       md_cons_cd_state, init_genesis, "Sega",   "Sega CD with 32X (USA, NTSC)", MACHINE_NOT_WORKING )
CONS( 1995, 32x_mcd,      32x_scd,  0,      md_32x_scd,      md,       md_cons_cd_state, init_md_eur,  "Sega",   "Mega-CD with 32X (Europe, PAL)", MACHINE_NOT_WORKING )
CONS( 1994, 32x_mcdj,     32x_scd,  0,      mdj_32x_scd,     md,       md_cons_cd_state, init_md_jpn,  "Sega",   "Mega-CD with 32X (Japan, NTSC)", MACHINE_NOT_WORKING )

// handheld hardware
CONS( 1995, gen_nomd,     0,        0,      ms_megadriv2,    gen_nomd, md_cons_slot_state, init_genesis, "Sega",   "Genesis Nomad (USA Genesis handheld)",  MACHINE_SUPPORTS_SAVE )

// handheld without LCD
CONS( 1993, megajet,      gen_nomd, 0,      ms_megadriv2,    megajet,  md_cons_slot_state, init_md_jpn,  "Sega",   "Mega Jet (Japan Mega Drive handheld)",  MACHINE_SUPPORTS_SAVE )

// LaserActive (Laserdisc Player(ex: CLD-A100) with 'Control Pack' Addon slot)
// Mega Drive Pack(PAC-S1)/Genesis Pack(PAC-S10) plugged into the Control Pack slot, for plays Mega Drive/Genesis Cartridge, Mega-CD/Sega CD, Mega-LD stuffs
CONS( 1993, laseract,     0,        0,      genesis_scd,     md,       md_cons_cd_state, init_genesis, "Pioneer / Sega","LaserActive with Genesis Pack PAC-S10 (USA, NTSC)", MACHINE_NOT_WORKING )
CONS( 1993, laseractj,    laseract, 0,      mdj_scd,         md,       md_cons_cd_state, init_md_jpn,  "Pioneer / Sega","LaserActive with Mega Drive Pack PAC-S1 (Japan, NTSC)", MACHINE_NOT_WORKING )
//TODO: it has also PC Engine Pack(PAC-N1)/TG16 Pack(PAC-N10) for plays PC Engine/TG16 Cartridge, (Super/Arcade) CD-ROM2/TurboGrafx-CD, LD-ROM2 stuffs, but not emulated.

/* clone hardware - not sure if this hardware is running some kind of emulator, or enhanced MD clone, or just custom banking */
CONS( 200?, dcat16,       0,        0,      dcat16_megadriv, md,       md_cons_slot_state, init_genesis, "Firecore",   "D-CAT16 (Mega Drive handheld)",  MACHINE_NOT_WORKING )
