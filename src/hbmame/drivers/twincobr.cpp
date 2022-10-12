// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/twincobr.cpp"

ROM_START( twincobrs01 ) // twincbu2
	ROM_REGION( 0x30000, "maincpu", 0 )
	ROM_LOAD16_BYTE( "b30_01.7j", 0x00000, 0x10000, CRC(07f64d13) SHA1(864ce0f9369c40c3ae792fc4ab2444a168214749) )
	ROM_LOAD16_BYTE( "b30_03.7h", 0x00001, 0x10000, CRC(41be6978) SHA1(4784804b738a332c7f24a43bcbb7a1e607365735) )
	ROM_LOAD16_BYTE( "s01.8j",    0x20000, 0x08000, CRC(bdd00ba4) SHA1(b76b22f03eb4b821a8c555edd9fcee814f2e66a7) )
	ROM_LOAD16_BYTE( "s01.8h",    0x20001, 0x08000, CRC(ed600907) SHA1(e5964db9eab2c334940795d71cb90f6679490227) )

	ROM_REGION( 0x10000, "audiocpu", 0 )
	ROM_LOAD( "b30-05", 0x00000, 0x08000, CRC(1a8f1e10) SHA1(0c37a7a50b2523506ad77ac03ae752eb94092ff6) )

	ROM_REGION( 0x2000, "dsp", 0 )
	ROM_LOAD16_BYTE( "dsp_22.bin", 0x0001, 0x0800, CRC(79389a71) SHA1(14ec4c1c9b06702319e89a7a250d0038393437f4) )
	ROM_LOAD16_BYTE( "dsp_21.bin", 0x0000, 0x0800, CRC(2d135376) SHA1(67a2cc774d272ee1cd6e6bc1c5fc33fc6968837e) )

	ROM_REGION( 0x0c000, "gfx1", 0 )
	ROM_LOAD( "b30_08.8c", 0x00000, 0x04000, CRC(0a254133) SHA1(17e9cc5e36fb4696012d0f9229fa172034cd843a) )
	ROM_LOAD( "b30_07.128", 0x04000, 0x04000, CRC(3160894d) SHA1(234951e2aa4002e23781f8ed3be64b71b0400e9f) )
	ROM_LOAD( "b30_06.8b", 0x08000, 0x04000, CRC(a599d845) SHA1(732001f2d378d890f148e6b616c287d71fae832a) )

	ROM_REGION( 0x40000, "gfx2", 0 )
	ROM_LOAD( "b30_15.512", 0x00000, 0x10000, CRC(88795b18) SHA1(73bd28ef12576cf54630304c3c0f8a08141a321f) )
	ROM_LOAD( "b30_16.512", 0x10000, 0x10000, CRC(e6f421d2) SHA1(82da99d017780d0e52988dc8ebfcd16b54d34b6b) )
	ROM_LOAD( "b30_13.18c", 0x20000, 0x10000, CRC(13daeac8) SHA1(1cb103f434e2ecf193fa936ca7ea9194064c5b39) )
	ROM_LOAD( "b30_14.20c", 0x30000, 0x10000, CRC(8cc79357) SHA1(31064df2b796ca85ad3caccf626b684dff1104a1) )

	ROM_REGION( 0x20000, "gfx3", 0 )
	ROM_LOAD( "b30_12.16c", 0x00000, 0x08000, CRC(b5d48389) SHA1(a00c5b9c231d3d580fa20c7ad3f8b6fd990e6594) )
	ROM_LOAD( "b30_11.14c", 0x08000, 0x08000, CRC(97f20fdc) SHA1(7cb3cd0637b0db889a3d552fd7c1a916eee5ca27) )
	ROM_LOAD( "b30_10.12c", 0x10000, 0x08000, CRC(170c01db) SHA1(f4c5a1600f6cbb48abbace66c6f7514f79138e8b) )
	ROM_LOAD( "b30_09.10c", 0x18000, 0x08000, CRC(44f5accd) SHA1(2f9bdebe71c8be195332356df68992fd38d86994) )

	ROM_REGION( 0x40000, "scu", 0 )
	ROM_LOAD( "b30_20.12d", 0x00000, 0x10000, CRC(cb4092b8) SHA1(35b1d1e04af760fa106124bd5a94174d63ff9705) )
	ROM_LOAD( "b30_19.512", 0x10000, 0x10000, CRC(4043ad5a) SHA1(bf14829ec800dab9ac6e385bddcab303301df530) )
	ROM_LOAD( "b30_18.512", 0x20000, 0x10000, CRC(cdc16155) SHA1(9f41695409dbf72c2a66911aaa94b8fe5f6acc01) )
	ROM_LOAD( "b30_17.16d", 0x30000, 0x10000, CRC(4264bff8) SHA1(3271b8b23f51346d1928ae01f8b547fed49181e6) )
ROM_END

ROM_START( twincobrs02 ) // twintwst
	ROM_REGION( 0x30000, "maincpu", 0 )
	ROM_LOAD16_BYTE( "b30_01.7j", 0x00000, 0x10000, CRC(07f64d13) SHA1(864ce0f9369c40c3ae792fc4ab2444a168214749) )
	ROM_LOAD16_BYTE( "b30_03.7h", 0x00001, 0x10000, CRC(41be6978) SHA1(4784804b738a332c7f24a43bcbb7a1e607365735) )
	ROM_LOAD16_BYTE( "s02.8j",    0x20000, 0x08000, CRC(3a646618) SHA1(fc1ed8f3c491f5cf16a17e5ce08c5d8f3ce03683) )
	ROM_LOAD16_BYTE( "s02.8h",    0x20001, 0x08000, CRC(d7d1e317) SHA1(57b8433b1677a390a7c7e00a1464bb8ed9cbfc73) )

	ROM_REGION( 0x10000, "audiocpu", 0 )
	ROM_LOAD( "b30_05_ii.4f", 0x00000, 0x08000, CRC(e37b3c44) SHA1(5fed10b29c14e27aee0cd92ecde5c5cb422273b1) )

	ROM_REGION( 0x2000, "dsp", 0 )
	ROM_LOAD16_BYTE( "dsp_22.bin", 0x0001, 0x0800, CRC(79389a71) SHA1(14ec4c1c9b06702319e89a7a250d0038393437f4) )
	ROM_LOAD16_BYTE( "dsp_21.bin", 0x0000, 0x0800, CRC(2d135376) SHA1(67a2cc774d272ee1cd6e6bc1c5fc33fc6968837e) )

	ROM_REGION( 0x0c000, "gfx1", 0 )
	ROM_LOAD( "b30_08.8c", 0x00000, 0x04000, CRC(0a254133) SHA1(17e9cc5e36fb4696012d0f9229fa172034cd843a) )
	ROM_LOAD( "b30_07.10b", 0x04000, 0x04000, CRC(e9e2d4b1) SHA1(e0a19dd46a9ba85d95bba7fbf81d8dc36dbfeabd) )
	ROM_LOAD( "b30_06.8b", 0x08000, 0x04000, CRC(a599d845) SHA1(732001f2d378d890f148e6b616c287d71fae832a) )

	ROM_REGION( 0x40000, "gfx2", 0 )
	ROM_LOAD( "b30_16.20b", 0x00000, 0x10000, CRC(15b3991d) SHA1(f5e7ed7a7721ed7e6dfd440634160390b7a294e4) )
	ROM_LOAD( "b30_15.18b", 0x10000, 0x10000, CRC(d9e2e55d) SHA1(0409e6df836d1d5198b64b21b42192631aa6d096) )
	ROM_LOAD( "b30_13.18c", 0x20000, 0x10000, CRC(13daeac8) SHA1(1cb103f434e2ecf193fa936ca7ea9194064c5b39) )
	ROM_LOAD( "b30_14.20c", 0x30000, 0x10000, CRC(8cc79357) SHA1(31064df2b796ca85ad3caccf626b684dff1104a1) )

	ROM_REGION( 0x20000, "gfx3", 0 )
	ROM_LOAD( "b30_12.16c", 0x00000, 0x08000, CRC(b5d48389) SHA1(a00c5b9c231d3d580fa20c7ad3f8b6fd990e6594) )
	ROM_LOAD( "b30_11.14c", 0x08000, 0x08000, CRC(97f20fdc) SHA1(7cb3cd0637b0db889a3d552fd7c1a916eee5ca27) )
	ROM_LOAD( "b30_10.12c", 0x10000, 0x08000, CRC(170c01db) SHA1(f4c5a1600f6cbb48abbace66c6f7514f79138e8b) )
	ROM_LOAD( "b30_09.10c", 0x18000, 0x08000, CRC(44f5accd) SHA1(2f9bdebe71c8be195332356df68992fd38d86994) )

	ROM_REGION( 0x40000, "scu", 0 )
	ROM_LOAD( "tcw20", 0x00000, 0x10000, CRC(41c8f4ea) SHA1(9da6911b89b5cf49b64a8bcb12ab191cc98316f2) )
	ROM_LOAD( "tcw19", 0x10000, 0x10000, CRC(8d5db99e) SHA1(33a1a51396fd289ee23ea1c645bc85bfed37848d) )
	ROM_LOAD( "tcw18", 0x20000, 0x10000, CRC(eade1ca3) SHA1(af62d4b3a4072d6b0970a47ab6ef68987bac9c93) )
	ROM_LOAD( "tcw17", 0x30000, 0x10000, CRC(6a559622) SHA1(8a07f792efea9d521a5fd0169a5d6c15fe615d5d) )
ROM_END

ROM_START( ktigers01 )
	ROM_REGION( 0x30000, "maincpu", 0 )
	ROM_LOAD16_BYTE( "b30_01.7j", 0x00000, 0x10000, CRC(07f64d13) SHA1(864ce0f9369c40c3ae792fc4ab2444a168214749) )
	ROM_LOAD16_BYTE( "b30_03.7h", 0x00001, 0x10000, CRC(41be6978) SHA1(4784804b738a332c7f24a43bcbb7a1e607365735) )
	ROM_LOAD16_BYTE( "b30_02_a.8j", 0x20000, 0x08000, CRC(321e2be6) SHA1(03b2c530326d1859b66829b49555d862be235643) )
	ROM_LOAD16_BYTE( "b30_04_a.8h", 0x20001, 0x08000, CRC(c3f960ff) SHA1(cbebf576d677cc02f4d0f22dcc226e898d4832c6) )

	ROM_REGION( 0x8000, "audiocpu", 0 )
	ROM_LOAD( "b30_05.4f", 0x0000, 0x8000, CRC(1a8f1e10) SHA1(0c37a7a50b2523506ad77ac03ae752eb94092ff6) )

	ROM_REGION( 0x2000, "dsp", 0 )
	ROM_LOAD( "d70015u_gxc-03_mcu_74002", 0x0000, 0x0c00, CRC(265b6f32) SHA1(1b548edeada4144baf732aba7e7013281c8e9608) )

	ROM_REGION( 0x0c000, "gfx1", 0 )
	ROM_LOAD( "b30_08.8c", 0x00000, 0x04000, CRC(0a254133) SHA1(17e9cc5e36fb4696012d0f9229fa172034cd843a) )
	ROM_LOAD( "b30_07.10b", 0x04000, 0x04000, CRC(e9e2d4b1) SHA1(e0a19dd46a9ba85d95bba7fbf81d8dc36dbfeabd) )
	ROM_LOAD( "b30_06.8b", 0x08000, 0x04000, CRC(a599d845) SHA1(732001f2d378d890f148e6b616c287d71fae832a) )

	ROM_REGION( 0x40000, "gfx2", 0 )
	ROM_LOAD( "b30_16.20b", 0x00000, 0x10000, CRC(15b3991d) SHA1(f5e7ed7a7721ed7e6dfd440634160390b7a294e4) )
	ROM_LOAD( "b30_15.18b", 0x10000, 0x10000, CRC(d9e2e55d) SHA1(0409e6df836d1d5198b64b21b42192631aa6d096) )
	ROM_LOAD( "b30_13.18c", 0x20000, 0x10000, CRC(13daeac8) SHA1(1cb103f434e2ecf193fa936ca7ea9194064c5b39) )
	ROM_LOAD( "b30_14.20c", 0x30000, 0x10000, CRC(8cc79357) SHA1(31064df2b796ca85ad3caccf626b684dff1104a1) )

	ROM_REGION( 0x20000, "gfx3", 0 )
	ROM_LOAD( "b30_12.16c", 0x00000, 0x08000, CRC(b5d48389) SHA1(a00c5b9c231d3d580fa20c7ad3f8b6fd990e6594) )
	ROM_LOAD( "b30_11.14c", 0x08000, 0x08000, CRC(97f20fdc) SHA1(7cb3cd0637b0db889a3d552fd7c1a916eee5ca27) )
	ROM_LOAD( "b30_10.12c", 0x10000, 0x08000, CRC(170c01db) SHA1(f4c5a1600f6cbb48abbace66c6f7514f79138e8b) )
	ROM_LOAD( "b30_09.10c", 0x18000, 0x08000, CRC(44f5accd) SHA1(2f9bdebe71c8be195332356df68992fd38d86994) )

	ROM_REGION( 0x40000, "scu", 0 )
	ROM_LOAD( "b30_20.12d", 0x00000, 0x10000, CRC(cb4092b8) SHA1(35b1d1e04af760fa106124bd5a94174d63ff9705) )
	ROM_LOAD( "b30_19.14d", 0x10000, 0x10000, CRC(9cb8675e) SHA1(559c21d505c60401f7368d4ab2b686b15075c5c5) )
	ROM_LOAD( "b30_18.15d", 0x20000, 0x10000, CRC(806fb374) SHA1(3eebefadcbdf713bf2a65b438092746b07edd3f0) )
	ROM_LOAD( "b30_17.16d", 0x30000, 0x10000, CRC(4264bff8) SHA1(3271b8b23f51346d1928ae01f8b547fed49181e6) )
ROM_END

GAME( 2002, twincobrs01, twincobr, twincobr, twincobru, twincobr_state, init_twincobr, ROT270, "hack", "Twin Cobra (Hack)", 0 )
GAME( 2002, twincobrs02, twincobr, twincobr, twincobr,  twincobr_state, init_twincobr, ROT270, "Twisty", "Twin Twist (Twin Cobra Hack)", 0 )
GAME( 1989, ktigers01,   twincobr, twincobr, ktiger,    twincobr_state, init_twincobr, ROT270, "Toaplan / Taito Corporation", "Kyukyoku Tiger (Japan, 1989)", 0 )

