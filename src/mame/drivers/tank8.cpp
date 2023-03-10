// license:BSD-3-Clause
// copyright-holders:Stefan Jokisch
/***************************************************************************

Atari Tank 8 driver

***************************************************************************/

#include "emu.h"
#include "includes/tank8.h"

#include "cpu/m6800/m6800.h"
#include "sound/discrete.h"
#include "speaker.h"



TIMER_CALLBACK_MEMBER(tank8_state::set_collision)
{
	m_maincpu->set_input_line(0, ASSERT_LINE);

	m_collision_index = param;
}


void tank8_state::machine_reset()
{
	m_collision_index = 0;
}


uint8_t tank8_state::collision_r()
{
	return m_collision_index;
}

void tank8_state::lockout_w(offs_t offset, uint8_t data)
{
	machine().bookkeeping().coin_lockout_w(offset, ~data & 1);
}


void tank8_state::int_reset_w(uint8_t data)
{
	m_collision_index &= ~0x3f;

	m_maincpu->set_input_line(0, CLEAR_LINE);
}

void tank8_state::crash_w(uint8_t data)
{
	m_discrete->write(TANK8_CRASH_EN, data);
}

void tank8_state::explosion_w(uint8_t data)
{
	m_discrete->write(TANK8_EXPLOSION_EN, data);
}

void tank8_state::bugle_w(uint8_t data)
{
	m_discrete->write(TANK8_BUGLE_EN, data);
}

void tank8_state::bug_w(uint8_t data)
{
	/* D0 and D1 determine the on/off time off the square wave */
	switch(data & 3) {
		case 0:
			m_discrete->write(TANK8_BUGLE_DATA1,8.0);
			m_discrete->write(TANK8_BUGLE_DATA2,4.0);
			break;
		case 1:
			m_discrete->write(TANK8_BUGLE_DATA1,8.0);
			m_discrete->write(TANK8_BUGLE_DATA2,7.0);
			break;
		case 2:
			m_discrete->write(TANK8_BUGLE_DATA1,8.0);
			m_discrete->write(TANK8_BUGLE_DATA2,2.0);
			break;
		case 3:
			m_discrete->write(TANK8_BUGLE_DATA1,16.0);
			m_discrete->write(TANK8_BUGLE_DATA2,4.0);
			break;
	}

}

void tank8_state::attract_w(uint8_t data)
{
	m_discrete->write(TANK8_ATTRACT_EN, data);
}

void tank8_state::motor_w(offs_t offset, uint8_t data)
{
	m_discrete->write(NODE_RELATIVE(TANK8_MOTOR1_EN, offset), data);
}

void tank8_state::tank8_cpu_map(address_map &map)
{
	map(0x0000, 0x00ff).ram();
	map(0x0400, 0x17ff).rom();
	map(0xf800, 0xffff).rom();

	map(0x1c00, 0x1c00).r(FUNC(tank8_state::collision_r));

	map(0x1c01, 0x1c01).portr("P1");
	map(0x1c02, 0x1c02).portr("P2");
	map(0x1c03, 0x1c03).portr("P3");
	map(0x1c04, 0x1c04).portr("P4");
	map(0x1c05, 0x1c05).portr("P5");
	map(0x1c06, 0x1c06).portr("P6");
	map(0x1c07, 0x1c07).portr("P7");
	map(0x1c08, 0x1c08).portr("P8");
	map(0x1c09, 0x1c09).portr("DSW1");
	map(0x1c0a, 0x1c0a).portr("DSW2");
	map(0x1c0b, 0x1c0b).portr("RC");
	map(0x1c0f, 0x1c0f).portr("VBLANK");

	map(0x1800, 0x1bff).w(FUNC(tank8_state::video_ram_w)).share("video_ram");
	map(0x1c00, 0x1c0f).writeonly().share("pos_h_ram");
	map(0x1c10, 0x1c1f).writeonly().share("pos_v_ram");
	map(0x1c20, 0x1c2f).writeonly().share("pos_d_ram");

	map(0x1c30, 0x1c37).w(FUNC(tank8_state::lockout_w));
	map(0x1d00, 0x1d00).w(FUNC(tank8_state::int_reset_w));
	map(0x1d01, 0x1d01).w(FUNC(tank8_state::crash_w));
	map(0x1d02, 0x1d02).w(FUNC(tank8_state::explosion_w));
	map(0x1d03, 0x1d03).w(FUNC(tank8_state::bugle_w));
	map(0x1d04, 0x1d04).w(FUNC(tank8_state::bug_w));
	map(0x1d05, 0x1d05).writeonly().share("team");
	map(0x1d06, 0x1d06).w(FUNC(tank8_state::attract_w));
	map(0x1e00, 0x1e07).w(FUNC(tank8_state::motor_w));

}


static INPUT_PORTS_START( tank8 )
	PORT_START("P1")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_COIN1 )
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_JOYSTICKLEFT_UP ) PORT_2WAY PORT_PLAYER(1)
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_JOYSTICKLEFT_DOWN ) PORT_2WAY PORT_PLAYER(1)
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_JOYSTICKRIGHT_UP ) PORT_2WAY PORT_PLAYER(1)
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_JOYSTICKRIGHT_DOWN ) PORT_2WAY PORT_PLAYER(1)
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_UNUSED )
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_UNUSED )
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_PLAYER(1)

	PORT_START("P2")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_COIN2 )
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_JOYSTICKLEFT_UP ) PORT_2WAY PORT_PLAYER(2)
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_JOYSTICKLEFT_DOWN ) PORT_2WAY PORT_PLAYER(2)
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_JOYSTICKRIGHT_UP ) PORT_2WAY PORT_PLAYER(2)
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_JOYSTICKRIGHT_DOWN ) PORT_2WAY PORT_PLAYER(2)
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_UNUSED )
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_UNUSED )
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_PLAYER(2)

	PORT_START("P3")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_COIN3 )
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_JOYSTICKLEFT_UP ) PORT_2WAY PORT_PLAYER(3)
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_JOYSTICKLEFT_DOWN ) PORT_2WAY PORT_PLAYER(3)
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_JOYSTICKRIGHT_UP ) PORT_2WAY PORT_PLAYER(3)
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_JOYSTICKRIGHT_DOWN ) PORT_2WAY PORT_PLAYER(3)
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_UNUSED )
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_UNUSED )
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_PLAYER(3)

	PORT_START("P4")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_COIN4 )
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_JOYSTICKLEFT_UP ) PORT_2WAY PORT_PLAYER(4)
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_JOYSTICKLEFT_DOWN ) PORT_2WAY PORT_PLAYER(4)
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_JOYSTICKRIGHT_UP ) PORT_2WAY PORT_PLAYER(4)
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_JOYSTICKRIGHT_DOWN ) PORT_2WAY PORT_PLAYER(4)
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_UNUSED )
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_UNUSED )
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_PLAYER(4)

	PORT_START("P5")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_COIN5 )
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_JOYSTICKLEFT_UP ) PORT_2WAY PORT_PLAYER(5)
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_JOYSTICKLEFT_DOWN ) PORT_2WAY PORT_PLAYER(5)
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_JOYSTICKRIGHT_UP ) PORT_2WAY PORT_PLAYER(5)
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_JOYSTICKRIGHT_DOWN ) PORT_2WAY PORT_PLAYER(5)
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_UNUSED )
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_UNUSED )
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_PLAYER(5)

	PORT_START("P6")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_COIN6 )
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_JOYSTICKLEFT_UP ) PORT_2WAY PORT_PLAYER(6)
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_JOYSTICKLEFT_DOWN ) PORT_2WAY PORT_PLAYER(6)
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_JOYSTICKRIGHT_UP ) PORT_2WAY PORT_PLAYER(6)
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_JOYSTICKRIGHT_DOWN ) PORT_2WAY PORT_PLAYER(6)
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_UNUSED )
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_UNUSED )
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_PLAYER(6)

	PORT_START("P7")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_COIN7 )
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_JOYSTICKLEFT_UP ) PORT_2WAY PORT_PLAYER(7)
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_JOYSTICKLEFT_DOWN ) PORT_2WAY PORT_PLAYER(7)
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_JOYSTICKRIGHT_UP ) PORT_2WAY PORT_PLAYER(7)
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_JOYSTICKRIGHT_DOWN ) PORT_2WAY PORT_PLAYER(7)
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_UNUSED )
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_UNUSED )
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_PLAYER(7)

	PORT_START("P8")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_COIN8 )
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_JOYSTICKLEFT_UP ) PORT_2WAY PORT_PLAYER(8)
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_JOYSTICKLEFT_DOWN ) PORT_2WAY PORT_PLAYER(8)
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_JOYSTICKRIGHT_UP ) PORT_2WAY PORT_PLAYER(8)
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_JOYSTICKRIGHT_DOWN ) PORT_2WAY PORT_PLAYER(8)
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_UNUSED )
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_UNUSED )
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_PLAYER(8)

	/* play time setting according to documents */
	PORT_START("DSW1")
	PORT_DIPNAME( 0x0f, 0x08, "Play Time" )
	PORT_DIPSETTING(    0x0f, "60 seconds" )
	PORT_DIPSETTING(    0x07, "70 seconds" )
	PORT_DIPSETTING(    0x0b, "80 seconds" )
	PORT_DIPSETTING(    0x03, "90 seconds" )
	PORT_DIPSETTING(    0x0d, "100 seconds" )
	PORT_DIPSETTING(    0x05, "110 seconds" )
	PORT_DIPSETTING(    0x09, "120 seconds" )
	PORT_DIPSETTING(    0x01, "130 seconds" )
	PORT_DIPSETTING(    0x0e, "140 seconds" )
	PORT_DIPSETTING(    0x06, "150 seconds" )
	PORT_DIPSETTING(    0x0a, "160 seconds" )
	PORT_DIPSETTING(    0x02, "170 seconds" )
	PORT_DIPSETTING(    0x0c, "180 seconds" )
	PORT_DIPSETTING(    0x04, "190 seconds" )
	PORT_DIPSETTING(    0x08, "200 seconds" )
	PORT_DIPSETTING(    0x00, "210 seconds" )
	PORT_BIT( 0xf0, IP_ACTIVE_LOW, IPT_UNUSED )

	PORT_START("DSW2")
	PORT_DIPNAME( 0x01, 0x01, "Remote" )
	PORT_DIPSETTING(    0x01, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_OTHER ) PORT_NAME( "Team" )
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_OTHER ) PORT_NAME( "Remote Start" )
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_OTHER ) PORT_NAME( "Remote Team" )
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BILL1 )
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_UNUSED )
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_UNUSED )
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_UNUSED )

	PORT_START("RC")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_OTHER ) PORT_NAME( "RC 1" )
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_OTHER ) PORT_NAME( "RC 2" )
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_OTHER ) PORT_NAME( "RC 3" )
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_OTHER ) PORT_NAME( "RC 4" )
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_OTHER ) PORT_NAME( "RC 5" )
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_OTHER ) PORT_NAME( "RC 6" )
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_OTHER ) PORT_NAME( "RC 7" )
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_OTHER ) PORT_NAME( "RC 8" )

	PORT_START("VBLANK")
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_CUSTOM ) PORT_VBLANK("screen")

	PORT_START("CRASH")
	PORT_ADJUSTER( 50, "Crash, Explosion Volume" )
INPUT_PORTS_END


static const gfx_layout tile_layout_1 =
{
	16, 16,
	64,
	1,
	{ 0 },
	{
		7, 7, 6, 6, 5, 5, 4, 4, 3, 3, 2, 2, 1, 1, 0, 0
	},
	{
		0x000, 0x000, 0x200, 0x200, 0x400, 0x400, 0x600, 0x600,
		0x800, 0x800, 0xa00, 0xa00, 0xc00, 0xc00, 0xe00, 0xe00
	},
	8
};


static const gfx_layout tile_layout_2 =
{
	16, 16,
	64,
	1,
	{ 0 },
	{
		0x000, 0x000, 0x200, 0x200, 0x400, 0x400, 0x600, 0x600,
		0x800, 0x800, 0xa00, 0xa00, 0xc00, 0xc00, 0xe00, 0xe00

	},
	{
		0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7
	},
	8
};


static const gfx_layout tank_layout =
{
	16, 16,
	4,
	1,
	{ 0 },
	{
		0x07, 0x06, 0x05, 0x04, 0x0f, 0x0e, 0x0d, 0x0c,
		0x17, 0x16, 0x15, 0x14, 0x1f, 0x1e, 0x1d, 0x1c
	},
	{
		0x000, 0x400, 0x020, 0x420, 0x040, 0x440, 0x060, 0x460,
		0x080, 0x480, 0x0a0, 0x4a0, 0x0c0, 0x4c0, 0x0e0, 0x4e0
	},
	0x100
};


static GFXDECODE_START( gfx_tank8 )
	GFXDECODE_ENTRY( "gfx1", 0, tile_layout_1, 0, 10 )
	GFXDECODE_ENTRY( "gfx1", 0, tile_layout_2, 0, 10 )
	GFXDECODE_ENTRY( "gfx2", 0, tank_layout,   0, 8 )
	GFXDECODE_ENTRY( "gfx3", 0, tank_layout,   0, 8 )
GFXDECODE_END


void tank8_state::tank8(machine_config &config)
{
	/* basic machine hardware */
	M6800(config, m_maincpu, 11055000 / 10); /* ? */
	m_maincpu->set_addrmap(AS_PROGRAM, &tank8_state::tank8_cpu_map);

	/* video hardware */
	SCREEN(config, m_screen, SCREEN_TYPE_RASTER);
	m_screen->set_video_attributes(VIDEO_UPDATE_AFTER_VBLANK);
	m_screen->set_refresh_hz(60);
	m_screen->set_vblank_time(ATTOSECONDS_IN_USEC(30 * 1000000 / 15681));
	m_screen->set_size(512, 524);
	m_screen->set_visarea(16, 495, 0, 463);
	m_screen->set_screen_update(FUNC(tank8_state::screen_update));
	m_screen->screen_vblank().set(FUNC(tank8_state::screen_vblank));
	m_screen->set_palette(m_palette);

	GFXDECODE(config, m_gfxdecode, m_palette, gfx_tank8);
	PALETTE(config, m_palette, FUNC(tank8_state::tank8_palette), 20, 10);

	/* sound hardware */
	SPEAKER(config, "mono").front_center();

	DISCRETE(config, m_discrete, tank8_discrete).add_route(ALL_OUTPUTS, "mono", 0.3);
}


ROM_START( tank8a )
	ROM_REGION( 0x20000, "maincpu", 0 )
	ROM_LOAD( "5071.c1",     0x10400, 0x0800, CRC(8e756e9e) BAD_DUMP SHA1(02ba64cae708967c39070b7e96085f41ed381fd4) )
	ROM_LOAD( "5072.e1",     0x10c00, 0x0800, CRC(88d65e48) BAD_DUMP SHA1(377b3c9f838bd515f8ddb55feda44398e1922521) )
	ROM_RELOAD(              0x1f800, 0x0800 )
	ROM_LOAD( "5073.f1",     0x11400, 0x0200, CRC(deedbe7c) SHA1(686c498d4c87054e00a9cfc0588e49f72470e1f9) )
	ROM_LOAD( "5074.j1",     0x11600, 0x0200, CRC(e49098b4) SHA1(5019ab0d4f3cd22733a60f15a54e4b772f534430) )

	ROM_REGION( 0x0200, "gfx1", 0 ) /* tiles */
	ROM_LOAD( "5075.n6",     0x0000, 0x0200, CRC(2d6519b3) SHA1(3837687893d0fca683ff9b86b335a77d98fd4230) )

	ROM_REGION( 0x0100, "gfx2", 0 ) /* tanks #1 */
	ROM_LOAD( "5079.h5",     0x0000, 0x0100, CRC(5c32d471) SHA1(983c7f15ad3a50ab87157b6894b9c292358de5a1) )

	ROM_REGION( 0x0100, "gfx3", 0 ) /* tanks #2 */
	ROM_LOAD( "5078.j5",     0x0000, 0x0100, CRC(ab083245) SHA1(e084627a4a17dd274d31638c938a04aa5049359b) )

	ROM_REGION( 0x0100, "user1", 0 ) /* decode PROMs */
	ROM_LOAD_NIB_LOW ( "5077.k1", 0x0000, 0x0100, CRC(343fc116) SHA1(ac9e95fc2a5dc115e8158d69482882072bea972b) )
	ROM_LOAD_NIB_HIGH( "5076.l1", 0x0000, 0x0100, CRC(4e1d9dad) SHA1(dc467914cbc8fd1add2d5e3b988e9596037b3a1e) )
ROM_END


ROM_START( tank8b )
	ROM_REGION( 0x20000, "maincpu", 0 )
	ROM_LOAD( "5071.c1",     0x10400, 0x0800, CRC(8e756e9e) BAD_DUMP SHA1(02ba64cae708967c39070b7e96085f41ed381fd4) )
	ROM_LOAD( "5072.e1",     0x10c00, 0x0800, CRC(88d65e48) BAD_DUMP SHA1(377b3c9f838bd515f8ddb55feda44398e1922521) )
	ROM_RELOAD(              0x1f800, 0x0800 )
	ROM_LOAD( "5764.f1",     0x11400, 0x0200, CRC(da4e81e0) SHA1(7e504b7064a8fed9dd7a6fe013f22427edc52384) )
	ROM_LOAD( "5765.j1",     0x11600, 0x0200, CRC(537e0c75) SHA1(b35db8d316cef9a2603b64868023e72e21d03143) )

	ROM_REGION( 0x0200, "gfx1", 0 ) /* tiles */
	ROM_LOAD( "5075.n6",     0x0000, 0x0200, CRC(2d6519b3) SHA1(3837687893d0fca683ff9b86b335a77d98fd4230) )

	ROM_REGION( 0x0100, "gfx2", 0 ) /* tanks #1 */
	ROM_LOAD( "5079.h5",     0x0000, 0x0100, CRC(5c32d471) SHA1(983c7f15ad3a50ab87157b6894b9c292358de5a1) )

	ROM_REGION( 0x0100, "gfx3", 0 ) /* tanks #2 */
	ROM_LOAD( "5078.j5",     0x0000, 0x0100, CRC(ab083245) SHA1(e084627a4a17dd274d31638c938a04aa5049359b) )

	ROM_REGION( 0x0100, "user1", 0 ) /* decode PROMs */
	ROM_LOAD_NIB_LOW ( "5077.k1", 0x0000, 0x0100, CRC(343fc116) SHA1(ac9e95fc2a5dc115e8158d69482882072bea972b) )
	ROM_LOAD_NIB_HIGH( "5076.l1", 0x0000, 0x0100, CRC(4e1d9dad) SHA1(dc467914cbc8fd1add2d5e3b988e9596037b3a1e) )
ROM_END


ROM_START( tank8c )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "a05071.c1",   0x0400, 0x0800, /* wrong revision */ CRC(2211fb2c) SHA1(6d44d1e9c94a7cb364c8809445c015a6510b8c5f) )
	ROM_LOAD( "a05072.e1",   0x0c00, 0x0800, CRC(d907b116) SHA1(290a77e6095d4ffc2365d784e74e115fe90617fb) )
	ROM_RELOAD(              0xf800, 0x0800 )
	ROM_LOAD( "a05473.f1",   0x1400, 0x0200, CRC(109020db) SHA1(c8517a09c1ff36a1b08c4a71acaae55dc48fc45b) )
	ROM_LOAD( "a05474.j1",   0x1600, 0x0200, CRC(64d8a386) SHA1(fdb6669762ddebbd775c66d67d82876428dfe009) )

	ROM_REGION( 0x0200, "gfx1", 0 ) /* tiles */
	ROM_LOAD( "5075.n6",     0x0000, 0x0200, CRC(2d6519b3) SHA1(3837687893d0fca683ff9b86b335a77d98fd4230) )

	ROM_REGION( 0x0100, "gfx2", 0 ) /* tanks #1 */
	ROM_LOAD( "5079.h5",     0x0000, 0x0100, CRC(5c32d471) SHA1(983c7f15ad3a50ab87157b6894b9c292358de5a1) )

	ROM_REGION( 0x0100, "gfx3", 0 ) /* tanks #2 */
	ROM_LOAD( "5078.j5",     0x0000, 0x0100, CRC(ab083245) SHA1(e084627a4a17dd274d31638c938a04aa5049359b) )
ROM_END


ROM_START( tank8d )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "a05071.c1",   0x0400, 0x0800, /* wrong revision */ CRC(2211fb2c) SHA1(6d44d1e9c94a7cb364c8809445c015a6510b8c5f) )
	ROM_LOAD( "a05072.e1",   0x0c00, 0x0800, CRC(d907b116) SHA1(290a77e6095d4ffc2365d784e74e115fe90617fb) )
	ROM_RELOAD(              0xf800, 0x0800 )
	ROM_LOAD( "a05918.f1",   0x1400, 0x0200, CRC(c2fca931) SHA1(a5aea79ef560d4498ba38482260f49a09cfe59d6) )
	ROM_LOAD( "a05919.j1",   0x1600, 0x0200, CRC(47204dc0) SHA1(b602e26b615e5e8bb747e9bd3879e4b95d923dc1) )

	ROM_REGION( 0x0200, "gfx1", 0 ) /* tiles */
	ROM_LOAD( "5075.n6",     0x0000, 0x0200, CRC(2d6519b3) SHA1(3837687893d0fca683ff9b86b335a77d98fd4230) )

	ROM_REGION( 0x0100, "gfx2", 0 ) /* tanks #1 */
	ROM_LOAD( "5079.h5",     0x0000, 0x0100, CRC(5c32d471) SHA1(983c7f15ad3a50ab87157b6894b9c292358de5a1) )

	ROM_REGION( 0x0100, "gfx3", 0 ) /* tanks #2 */
	ROM_LOAD( "5078.j5",     0x0000, 0x0100, CRC(ab083245) SHA1(e084627a4a17dd274d31638c938a04aa5049359b) )
ROM_END


ROM_START( tank8 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "b05475.c1",   0x0400, 0x0800, CRC(62a00e75) SHA1(58d80dc58bc2a4503348807db578348fc76a5349) )
	ROM_LOAD( "a05072.e1",   0x0c00, 0x0800, CRC(d907b116) SHA1(290a77e6095d4ffc2365d784e74e115fe90617fb) )
	ROM_RELOAD(              0xf800, 0x0800 )
	ROM_LOAD( "b05476.f1",   0x1400, 0x0200, CRC(98754edd) SHA1(56eb017bad9c29649573875a6b13189f2ba69b0e) )
	ROM_LOAD( "b05477.j1",   0x1600, 0x0200, CRC(5087223b) SHA1(fea032e6d0b3e0730a1180e57118e1765693f67e) )

	ROM_REGION( 0x0200, "gfx1", 0 ) /* tiles */
	ROM_LOAD( "5075.n6",     0x0000, 0x0200, CRC(2d6519b3) SHA1(3837687893d0fca683ff9b86b335a77d98fd4230) )

	ROM_REGION( 0x0100, "gfx2", 0 ) /* tanks #1 */
	ROM_LOAD( "5079.h5",     0x0000, 0x0100, CRC(5c32d471) SHA1(983c7f15ad3a50ab87157b6894b9c292358de5a1) )

	ROM_REGION( 0x0100, "gfx3", 0 ) /* tanks #2 */
	ROM_LOAD( "5078.j5",     0x0000, 0x0100, CRC(ab083245) SHA1(e084627a4a17dd274d31638c938a04aa5049359b) )
ROM_END


void tank8_state::init_decode()
{
	const uint8_t* DECODE = memregion("user1")->base();
	uint8_t* p1 = memregion("maincpu")->base() + 0x00000;
	uint8_t* p2 = memregion("maincpu")->base() + 0x10000;

	for (int i = 0x0400; i <= 0x17ff; i++)
	{
		p1[i] = DECODE[p2[i]];
	}
	for (int i = 0xf800; i <= 0xffff; i++)
	{
		p1[i] = DECODE[p2[i]];
	}
}


GAME( 1976, tank8,    0,        tank8,    tank8, tank8_state, empty_init,  ROT0, "Atari (Kee Games)", "Tank 8 (set 1)",  MACHINE_SUPPORTS_SAVE)
GAME( 1976, tank8a,   tank8,    tank8,    tank8, tank8_state, init_decode, ROT0, "Atari (Kee Games)", "Tank 8 (set 2)",  MACHINE_NOT_WORKING | MACHINE_SUPPORTS_SAVE )
GAME( 1976, tank8b,   tank8,    tank8,    tank8, tank8_state, init_decode, ROT0, "Atari (Kee Games)", "Tank 8 (set 3)",  MACHINE_NOT_WORKING | MACHINE_SUPPORTS_SAVE )
GAME( 1976, tank8c,   tank8,    tank8,    tank8, tank8_state, empty_init,  ROT0, "Atari (Kee Games)", "Tank 8 (set 4)",  MACHINE_NOT_WORKING | MACHINE_SUPPORTS_SAVE )
GAME( 1976, tank8d,   tank8,    tank8,    tank8, tank8_state, empty_init,  ROT0, "Atari (Kee Games)", "Tank 8 (set 5)",  MACHINE_NOT_WORKING | MACHINE_SUPPORTS_SAVE )
