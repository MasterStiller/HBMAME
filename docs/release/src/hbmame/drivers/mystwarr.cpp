// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/mystwarr.cpp"

 /*************
 Dragoon Might
****************/

ROM_START( mtlchamps01 )
	ROM_REGION( 0x400000, "maincpu", 0 )
	ROM_LOAD16_BYTE( "234eab01_ps01.20f", 0x000000, 0x40000, CRC(7c3bb381) SHA1(a543dfac8180946cd1f6e24548185324665996ea) )
	ROM_LOAD16_BYTE( "234eab02_ps01.20g", 0x000001, 0x40000, CRC(1b53e493) SHA1(a88f4c257e9b2be1d997052ee6cc191e4d66cda0) )
	ROM_LOAD16_BYTE( "234_d03.19f",  0x300000, 0x80000, CRC(abb577c6) SHA1(493f11a10a4d5b62d755ff8274e77d898544944f) )
	ROM_LOAD16_BYTE( "234_d04.19g",  0x300001, 0x80000, CRC(030a1925) SHA1(03783488950c9f27af5948e7b9f6a609c2df6e0b) )

	ROM_REGION( 0x40000, "soundcpu", 0 )
	ROM_LOAD("234_d05.6b", 0x00000, 0x20000, CRC(efb6bcaa) SHA1(4fb24b89a50b341871945547859278a6e2f5e002) )
	ROM_RELOAD(            0x20000, 0x20000 )

	ROM_REGION( 0x600000, "k056832", ROMREGION_ERASE00 )
	ROM_LOADTILE_WORD( "234a08.1h", 0x000000, 1*1024*1024, CRC(27e94288) SHA1(a92b03adf7beea6a1ceb74f659c87c628a7ab8e4) )
	ROM_LOADTILE_WORD( "234a09.1k", 0x000002, 1*1024*1024, CRC(03aad28f) SHA1(e7d9d788822ac9666e089b58288e3fcdba1b89da) )
	ROM_LOADTILE_BYTE( "234a10.3h", 0x000004, 512*1024, CRC(51f50fe2) SHA1(164fc975feff442d93f1917727c159051dcd3a55) )

	ROM_REGION( 0xa00000, "k055673", ROMREGION_ERASE00 )
	ROM_LOAD64_WORD( "234a16.22k", 0x000000, 2*1024*1024, CRC(14d909a5) SHA1(15da356852fc0c63ecd924ac37ebe24bf3ba0760) )
	ROM_LOAD64_WORD( "234a15.20k", 0x000002, 2*1024*1024, CRC(a5028418) SHA1(ec6fc7b38fb1d27490a5a9310ecac2d1049e197c) )
	ROM_LOAD64_WORD( "234a14.19k", 0x000004, 2*1024*1024, CRC(d7921f47) SHA1(3fc97b308ad2ca25a376373ddfe08c8a375c424e) )
	ROM_LOAD64_WORD( "234a13.17k", 0x000006, 2*1024*1024, CRC(5974392e) SHA1(7c380419244439804797a9510846d273ebe99d02) )
	ROM_LOAD16_BYTE( "234a12.12k", 0x800000, 1024*1024, CRC(c7f2b099) SHA1(b72b80feb52560a5a42a1db39b059ac8bca27c10) )
	ROM_LOAD16_BYTE( "234a11.10k", 0x800001, 1024*1024, CRC(82923713) SHA1(a36cd3b2c9d36e93a3c25ba1d4e162f3d92e06ae) )

	ROM_REGION( 0x400000, "k054539", 0 )
	ROM_LOAD( "234a06.2d", 0x000000, 2*1024*1024, CRC(12d32384) SHA1(ecd6cd752b0e20339e17a7652ed843fbb43f7595) )
	ROM_LOAD( "234a07.1d", 0x200000, 2*1024*1024, CRC(05ee239f) SHA1(f4e6e7568dc73666a2b5e0c3fe743432e0436464) )

	ROM_REGION( 0x80, "eeprom", 0 )
	ROM_LOAD( "mtlchamp.nv", 0x0000, 0x080, CRC(cd47858e) SHA1(8effdcd631516d537f956509111cb3d4d18040db) )
ROM_END

 /*************
 Monster Maulers
****************/

ROM_START( mmaulerss01 )
	ROM_REGION( 0x400000, "maincpu", 0 )
	ROM_LOAD16_BYTE( "170eaa07_ps01.24m", 0x000000, 0x80000, CRC(0a14a7de) SHA1(5d74ca1b306901201d0fbf9b65dadc5c26082cd4) )
	ROM_LOAD16_BYTE( "170eaa09_ps01.19l", 0x000001, 0x80000, CRC(45219214) SHA1(5cef21c1556b943d3fa122e882d641e0627e946e) )
	ROM_LOAD16_BYTE( "170a08.21m",   0x100000, 0x40000, CRC(03c59ba2) SHA1(041473fe5f9004bfb7ca767c2004154c27f726ff) )
	ROM_LOAD16_BYTE( "170a10.17l",   0x100001, 0x40000, CRC(8a340909) SHA1(3e2ef2642e792cdc38b3442df67377ed9e70d3ab) )

	ROM_REGION( 0x40000, "soundcpu", 0 )
	ROM_LOAD("170a13.9c", 0x00000, 0x40000, CRC(2ebf4d1c) SHA1(33a3f4153dfdc46cc223d216a17ef9428c09129d) )

	ROM_REGION( 0x600000, "k056832", ROMREGION_ERASE00 )
	ROM_LOADTILE_WORD( "170a16.2t", 0x000000, 1*1024*1024, CRC(41fee912) SHA1(73cf167ac9fc42cb8048a87b6c6d1c3c0ae3c2e2) )
	ROM_LOADTILE_WORD( "170a17.2x", 0x000002, 1*1024*1024, CRC(96957c91) SHA1(b12d356f8a015ec0984bdb86da9c569eb0c67880) )
	ROM_LOADTILE_BYTE( "170a24.5r", 0x000004, 512*1024, CRC(562ad4bd) SHA1(f55b29142ea39f090244f0945a56760bab25c7a7) )

	ROM_REGION( 0xa00000, "k055673", ROMREGION_ERASE00 )
	ROM_LOAD64_WORD( "170a19.34u", 0x000000, 2*1024*1024, CRC(be835141) SHA1(b76e1da45bf602dd9eb30fb8b7181cea2e820c3d) )
	ROM_LOAD64_WORD( "170a21.34y", 0x000002, 2*1024*1024, CRC(bcb68136) SHA1(1d453f59d832b8ea99cf0a60a917edce5c1c90a0) )
	ROM_LOAD64_WORD( "170a18.36u", 0x000004, 2*1024*1024, CRC(e1e3c8d2) SHA1(2c94fcedd1dcef3d3332af358ae8a67dea507216) )
	ROM_LOAD64_WORD( "170a20.36y", 0x000006, 2*1024*1024, CRC(ccb4d88c) SHA1(064b4dab0ca6e5a1fa2fc2e9bbb19c7499830ee1) )
	ROM_LOAD16_BYTE( "170a23.29y", 0x800000, 1024*1024, CRC(6b5390e4) SHA1(0c5066bc86e782db4b64c2a604aed89ae99af005) )
	ROM_LOAD16_BYTE( "170a22.32y", 0x800001, 1024*1024, CRC(21628106) SHA1(1e025ff53caa5cbbf7695f8a77736d59f8a8af1b) )

	ROM_REGION( 0x180000, "gfx3", 0 )
	ROM_LOAD( "170a04.33n", 0x000000, 0x80000, CRC(64b9a73b) SHA1(8b984bfd8bdf6d93ad223fca46a4f958a0edb2be) )
	ROM_LOAD( "170a05.30n", 0x080000, 0x80000, CRC(f2c101d0) SHA1(d80045c9a02db08ea6c851bdc12826862e11c381) )
	ROM_LOAD( "170a06.27n", 0x100000, 0x80000, CRC(b032e59b) SHA1(482300c683db20c2b2fc6e007b8f7e35373e3c00) )

	ROM_REGION( 0x80000, "gfx4", 0 )
	ROM_LOAD( "170a02.34j", 0x000000, 0x40000, CRC(b040cebf) SHA1(4d1ba4ee60fd7caf678837ec6f4d68fcbce1ccf2) )
	ROM_LOAD( "170a03.36m", 0x040000, 0x40000, CRC(7fb412b2) SHA1(f603a8f0becf88e345f4b7a68cf018962a255a1e) )

	ROM_REGION( 0x400000, "k054539", 0 )
	ROM_LOAD("170a14.2g", 0x000000, 2*1024*1024, CRC(83317cda) SHA1(c5398c5959ef3ea73835e13db69660dd28c31486) )
	ROM_LOAD("170a15.2m", 0x200000, 2*1024*1024, CRC(d4113ae9) SHA1(e234d06f462e3db64455c384c2f42174f9ef9c6a) )

	ROM_REGION( 0x80, "eeprom", 0 )
	ROM_LOAD( "mmaulers.nv", 0x0000, 0x080, CRC(8324f517) SHA1(4697d091a1924e1a5d6c3ffc64a40fd36eebe557) )
ROM_END

 /****************
 Metamorphic Force
*******************/

ROM_START( metamrphs01 ) //metamrph
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_BYTE( "224eaa01s01.15h", 0x000001, 0x40000, CRC(8fc188bb) SHA1(c5a2b5c50505e82e89e280c92e5a08bf4d8e76f1) )
	ROM_LOAD16_BYTE( "224eaa02s01.15f", 0x000000, 0x40000, CRC(ec7b34a0) SHA1(cf2d9c45cff3d05079b51628dad07f4b879ffaaf) )
	ROM_LOAD16_BYTE( "224a03",       0x100001, 0x80000, CRC(a5bedb01) SHA1(5e7a0b93af654ba6a87be8d449c7080a0f0e2a43) )
	ROM_LOAD16_BYTE( "224a04",       0x100000, 0x80000, CRC(ada53ba4) SHA1(f77bf854dff1f8f718579fe6d3730066708396e2) )

	ROM_REGION( 0x40000, "soundcpu", 0 )
	ROM_LOAD("224a05", 0x000000, 0x40000, CRC(4b4c985c) SHA1(c83cce05355023be9cd55b4aa595c61f8236269c) )

	ROM_REGION( 0x500000, "k056832", ROMREGION_ERASE00 )
	ROM_LOADTILE_WORD( "224a09", 0x000000, 1*1024*1024, CRC(1931afce) SHA1(78838c0fd2a9c80f130db1fcf6c88b14f7363639) )
	ROM_LOADTILE_WORD( "224a08", 0x000002, 1*1024*1024, CRC(dc94d53a) SHA1(91e16371a335f078a81c06a1045759653080aba0) )

	ROM_REGION( 0x800000, "k055673", ROMREGION_ERASE00 )
	ROM_LOAD64_WORD( "224a10", 0x000000, 2*1024*1024, CRC(161287f0) SHA1(a13b197a98fa1cebb11fb87b54e277c72852c4ee) )
	ROM_LOAD64_WORD( "224a11", 0x000002, 2*1024*1024, CRC(df5960e1) SHA1(ee7794dd119f5f2c52e7ba589d78067a89ff3cab) )
	ROM_LOAD64_WORD( "224a12", 0x000004, 2*1024*1024, CRC(ca72a4b3) SHA1(a09deb6d7cb8be4edaeb78e0e676ea2d6055e9e0) )
	ROM_LOAD64_WORD( "224a13", 0x000006, 2*1024*1024, CRC(86b58feb) SHA1(5a43746e2cd3c7aca21496c092aef83e64b3ab2c) )

	ROM_REGION( 0x40000, "k053250_1", 0 )
	ROM_LOAD( "224a14", 0x000000, 0x40000, CRC(3c79b404) SHA1(7c6bb4cbf050f314ea0cd3e8bc6e1947d0573084) )

	ROM_REGION( 0x400000, "k054539", 0 )
	ROM_LOAD( "224a06", 0x000000, 2*1024*1024, CRC(972f6abe) SHA1(30907495fc49fe3424c092b074c1dc137aa14306) )
	ROM_LOAD( "224a07", 0x200000, 1*1024*1024, CRC(61b2f97a) SHA1(34bf835d6361c7809d40fa20fd238c9e2a84b101) )

	ROM_REGION( 0x80, "eeprom", 0 )
	ROM_LOAD( "metamrph.nv", 0x0000, 0x080, CRC(2c51229a) SHA1(7f056792cc44ec3d4aacc33c825ab796a913488e) )
ROM_END

 /*************
 Violent Storm
****************/

ROM_START( viostorms01 ) //viostormab
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_BYTE( "168aab01_ps01.15h", 0x000001, 0x80000, CRC(a6e55e6e) SHA1(9068f7da102d200374106968c8939dfa874f7d08) )
	ROM_LOAD16_BYTE( "168aab02_ps01.15f", 0x000000, 0x80000, CRC(f014f55b) SHA1(fae0a0974c46af316e49832ef70251010df025ac) )

	ROM_REGION( 0x40000, "soundcpu", 0 )
	ROM_LOAD("168a05.7c", 0x00000, 0x20000, CRC(507fb3eb) SHA1(a4f676e3caaafe86918c76ded08d0c202969adf6) )
	ROM_RELOAD(           0x20000, 0x20000 )

	ROM_REGION( 0x600000, "k056832", ROMREGION_ERASE00 )
	ROM_LOADTILE_WORD( "168a09.1h", 0x000000, 2*1024*1024, CRC(1b34a881) SHA1(5de20f7ee7f90d4f6dea349ca5000bfcf74253b1) )
	ROM_LOADTILE_WORD( "168a08.1k", 0x000002, 2*1024*1024, CRC(db0ce743) SHA1(dfe24a1e3e72da188a92668928e79afd6c5d22ee) )

	ROM_REGION( 0x800000, "k055673", ROMREGION_ERASE00 )
	ROM_LOAD64_WORD( "168a10.22k", 0x000000, 2*1024*1024, CRC(bd2bbdea) SHA1(54faf2ded16e66d675bbbec4ebd42b4708edfaef) )
	ROM_LOAD64_WORD( "168a11.19k", 0x000002, 2*1024*1024, CRC(7a57c9e7) SHA1(8763c310f7b515aef52d4e007bc949e8803690f4) )
	ROM_LOAD64_WORD( "168a12.20k", 0x000004, 2*1024*1024, CRC(b6b1c4ef) SHA1(064ab4db884c8f98ab9e631b7034996d4b92ab7b) )
	ROM_LOAD64_WORD( "168a13.17k", 0x000006, 2*1024*1024, CRC(cdec3650) SHA1(949bc06bb38a2d5315ee4f6db19e043655b90e6e) )

	ROM_REGION( 0x40000, "gfx3", ROMREGION_ERASE00 )

	ROM_REGION( 0x400000, "k054539", 0 )
	ROM_LOAD( "168a06.1c", 0x000000, 2*1024*1024, CRC(25404fd7) SHA1(282cf523728b38d0bf14d765dd7257aa1fb2af39) )
	ROM_LOAD( "168a07.1e", 0x200000, 2*1024*1024, CRC(fdbbf8cc) SHA1(a8adf72a25fe2b9c4c338350d02c92deb5f8c8e9) )

	ROM_REGION( 0x80, "eeprom", 0 )
	ROM_LOAD( "viostormab.nv", 0x0000, 0x080, CRC(38ffce43) SHA1(f0666198562ae7c07c8f805088e882a10c79bcf3) )
ROM_END

ROM_START( viostorms02 ) //viostormeb
	ROM_REGION( 0x200000, "maincpu", 0)
	ROM_LOAD16_BYTE( "168eab01_ps02.15h", 0x000001, 0x80000, CRC(49a5f912) SHA1(55929e5fd7bb7aa55ccc829f91bb29c6d6eacf54) )
	ROM_LOAD16_BYTE( "168eab02_ps02.15f", 0x000000, 0x80000, CRC(69f3c524) SHA1(23fd0d34a35cbad2f4020c8fa0fe75f498c386ed) )

	ROM_REGION( 0x40000, "soundcpu", 0 )
	ROM_LOAD("168a05.7c", 0x00000, 0x20000, CRC(507fb3eb) SHA1(a4f676e3caaafe86918c76ded08d0c202969adf6) )
	ROM_RELOAD(           0x20000, 0x20000 )

	ROM_REGION( 0x600000, "k056832", ROMREGION_ERASE00)
	ROM_LOADTILE_WORD( "168a09.1h", 0x000000, 2*1024*1024, CRC(1b34a881) SHA1(5de20f7ee7f90d4f6dea349ca5000bfcf74253b1) )
	ROM_LOADTILE_WORD( "168a08.1k", 0x000002, 2*1024*1024, CRC(db0ce743) SHA1(dfe24a1e3e72da188a92668928e79afd6c5d22ee) )

	ROM_REGION( 0x800000, "k055673", ROMREGION_ERASE00)
	ROM_LOAD64_WORD( "168a10.22k", 0x000000, 2*1024*1024, CRC(bd2bbdea) SHA1(54faf2ded16e66d675bbbec4ebd42b4708edfaef) )
	ROM_LOAD64_WORD( "168a11.19k", 0x000002, 2*1024*1024, CRC(7a57c9e7) SHA1(8763c310f7b515aef52d4e007bc949e8803690f4) )
	ROM_LOAD64_WORD( "168a12.20k", 0x000004, 2*1024*1024, CRC(b6b1c4ef) SHA1(064ab4db884c8f98ab9e631b7034996d4b92ab7b) )
	ROM_LOAD64_WORD( "168a13.17k", 0x000006, 2*1024*1024, CRC(cdec3650) SHA1(949bc06bb38a2d5315ee4f6db19e043655b90e6e) )

	ROM_REGION( 0x40000, "gfx3", ROMREGION_ERASE00)

	ROM_REGION( 0x400000, "k054539", 0)
	ROM_LOAD( "168a06.1c", 0x000000, 2*1024*1024, CRC(25404fd7) SHA1(282cf523728b38d0bf14d765dd7257aa1fb2af39) )
	ROM_LOAD( "168a07.1e", 0x200000, 2*1024*1024, CRC(fdbbf8cc) SHA1(a8adf72a25fe2b9c4c338350d02c92deb5f8c8e9) )

	ROM_REGION( 0x80, "eeprom", 0 )
	ROM_LOAD( "viostormeb.nv", 0x0000, 0x080, CRC(28b5fe49) SHA1(0ef51ae4b012a7d680543747fd4b6dd9dfb5f560) )
ROM_END

ROM_START( viostorms03 ) //viostormub
	ROM_REGION( 0x200000, "maincpu", 0)
	ROM_LOAD16_BYTE( "168uab01_ps03.15h", 0x000001, 0x80000, CRC(45126fb4) SHA1(f1a1b769d2789d54b66de08cc70f926205559409) )
	ROM_LOAD16_BYTE( "168uab02_ps03.15f", 0x000000, 0x80000, CRC(d4a738f0) SHA1(0e4bcc5d977710f894fa6f6fbb2ad2b2c58d8985) )

	ROM_REGION( 0x40000, "soundcpu", 0 )
	ROM_LOAD("168a05.7c", 0x00000, 0x20000, CRC(507fb3eb) SHA1(a4f676e3caaafe86918c76ded08d0c202969adf6) )
	ROM_RELOAD(           0x20000, 0x20000 )

	ROM_REGION( 0x600000, "k056832", ROMREGION_ERASE00)
	ROM_LOADTILE_WORD( "168a09.1h", 0x000000, 2*1024*1024, CRC(1b34a881) SHA1(5de20f7ee7f90d4f6dea349ca5000bfcf74253b1) )
	ROM_LOADTILE_WORD( "168a08.1k", 0x000002, 2*1024*1024, CRC(db0ce743) SHA1(dfe24a1e3e72da188a92668928e79afd6c5d22ee) )

	ROM_REGION( 0x800000, "k055673", ROMREGION_ERASE00)
	ROM_LOAD64_WORD( "168a10.22k", 0x000000, 2*1024*1024, CRC(bd2bbdea) SHA1(54faf2ded16e66d675bbbec4ebd42b4708edfaef) )
	ROM_LOAD64_WORD( "168a11.19k", 0x000002, 2*1024*1024, CRC(7a57c9e7) SHA1(8763c310f7b515aef52d4e007bc949e8803690f4) )
	ROM_LOAD64_WORD( "168a12.20k", 0x000004, 2*1024*1024, CRC(b6b1c4ef) SHA1(064ab4db884c8f98ab9e631b7034996d4b92ab7b) )
	ROM_LOAD64_WORD( "168a13.17k", 0x000006, 2*1024*1024, CRC(cdec3650) SHA1(949bc06bb38a2d5315ee4f6db19e043655b90e6e) )

	ROM_REGION( 0x40000, "gfx3", ROMREGION_ERASE00)

	ROM_REGION( 0x400000, "k054539", 0)
	ROM_LOAD( "168a06.1c", 0x000000, 2*1024*1024, CRC(25404fd7) SHA1(282cf523728b38d0bf14d765dd7257aa1fb2af39) )
	ROM_LOAD( "168a07.1e", 0x200000, 2*1024*1024, CRC(fdbbf8cc) SHA1(a8adf72a25fe2b9c4c338350d02c92deb5f8c8e9) )

	ROM_REGION( 0x80, "eeprom", 0 )
	ROM_LOAD( "viostormub.nv", 0x0000, 0x080, CRC(b6937413) SHA1(eabc2ea661201f5ed42ab541aee765480bbdd5bc) )
ROM_END

ROM_START( viostorms04 ) //viostorma
	ROM_REGION( 0x200000, "maincpu", 0)
	ROM_LOAD16_BYTE( "168uab01_ps04.15h", 0x000001, 0x80000, CRC(3a870170) SHA1(f6fd2e63105235574adb9ec7adfa02f9f5826cd7) )
	ROM_LOAD16_BYTE( "168uab02_ps04.15f", 0x000000, 0x80000, CRC(d00bf154) SHA1(e95acc9ef04a31da22c685202a18cc637305fe29) )

	ROM_REGION( 0x40000, "soundcpu", 0 )
	ROM_LOAD("168a05.7c", 0x00000, 0x20000, CRC(507fb3eb) SHA1(a4f676e3caaafe86918c76ded08d0c202969adf6) )
	ROM_RELOAD(           0x20000, 0x20000 )

	ROM_REGION( 0x600000, "k056832", ROMREGION_ERASE00)
	ROM_LOADTILE_WORD( "168a09.1h", 0x000000, 2*1024*1024, CRC(1b34a881) SHA1(5de20f7ee7f90d4f6dea349ca5000bfcf74253b1) )
	ROM_LOADTILE_WORD( "168a08.1k", 0x000002, 2*1024*1024, CRC(db0ce743) SHA1(dfe24a1e3e72da188a92668928e79afd6c5d22ee) )

	ROM_REGION( 0x800000, "k055673", ROMREGION_ERASE00)
	ROM_LOAD64_WORD( "168a10.22k", 0x000000, 2*1024*1024, CRC(bd2bbdea) SHA1(54faf2ded16e66d675bbbec4ebd42b4708edfaef) )
	ROM_LOAD64_WORD( "168a11.19k", 0x000002, 2*1024*1024, CRC(7a57c9e7) SHA1(8763c310f7b515aef52d4e007bc949e8803690f4) )
	ROM_LOAD64_WORD( "168a12.20k", 0x000004, 2*1024*1024, CRC(b6b1c4ef) SHA1(064ab4db884c8f98ab9e631b7034996d4b92ab7b) )
	ROM_LOAD64_WORD( "168a13.17k", 0x000006, 2*1024*1024, CRC(cdec3650) SHA1(949bc06bb38a2d5315ee4f6db19e043655b90e6e) )

	ROM_REGION( 0x40000, "gfx3", ROMREGION_ERASE00)

	ROM_REGION( 0x400000, "k054539", 0)
	ROM_LOAD( "168a06.1c", 0x000000, 2*1024*1024, CRC(25404fd7) SHA1(282cf523728b38d0bf14d765dd7257aa1fb2af39) )
	ROM_LOAD( "168a07.1e", 0x200000, 2*1024*1024, CRC(fdbbf8cc) SHA1(a8adf72a25fe2b9c4c338350d02c92deb5f8c8e9) )

	ROM_REGION( 0x80, "eeprom", 0 )
	ROM_LOAD( "viostorm.nv", 0x0000, 0x080, CRC(3cb1c96c) SHA1(56ed6633e7108925a13ad6bc32381bfde592f70b) )
ROM_END

ROM_START( viostorms05 ) //viostorma
	ROM_REGION( 0x200000, "maincpu", 0)
	ROM_LOAD16_BYTE( "168uab01_ps05.15h", 0x000001, 0x80000, CRC(f547608b) SHA1(0509b646e3b1c72dc3ba00cb8526b777b5167124) )
	ROM_LOAD16_BYTE( "168uab02_ps05.15f", 0x000000, 0x80000, CRC(c980ffe8) SHA1(27b724454db59ddea635c30ffc8fc8d98fd8305a) )

	ROM_REGION( 0x40000, "soundcpu", 0 )
	ROM_LOAD("168a05.7c", 0x00000, 0x20000, CRC(507fb3eb) SHA1(a4f676e3caaafe86918c76ded08d0c202969adf6) )
	ROM_RELOAD(           0x20000, 0x20000 )

	ROM_REGION( 0x600000, "k056832", ROMREGION_ERASE00)
	ROM_LOADTILE_WORD( "168a09.1h", 0x000000, 2*1024*1024, CRC(1b34a881) SHA1(5de20f7ee7f90d4f6dea349ca5000bfcf74253b1) )
	ROM_LOADTILE_WORD( "168a08.1k", 0x000002, 2*1024*1024, CRC(db0ce743) SHA1(dfe24a1e3e72da188a92668928e79afd6c5d22ee) )

	ROM_REGION( 0x800000, "k055673", ROMREGION_ERASE00)
	ROM_LOAD64_WORD( "168a10.22k", 0x000000, 2*1024*1024, CRC(bd2bbdea) SHA1(54faf2ded16e66d675bbbec4ebd42b4708edfaef) )
	ROM_LOAD64_WORD( "168a11.19k", 0x000002, 2*1024*1024, CRC(7a57c9e7) SHA1(8763c310f7b515aef52d4e007bc949e8803690f4) )
	ROM_LOAD64_WORD( "168a12.20k", 0x000004, 2*1024*1024, CRC(b6b1c4ef) SHA1(064ab4db884c8f98ab9e631b7034996d4b92ab7b) )
	ROM_LOAD64_WORD( "168a13.17k", 0x000006, 2*1024*1024, CRC(cdec3650) SHA1(949bc06bb38a2d5315ee4f6db19e043655b90e6e) )

	ROM_REGION( 0x40000, "gfx3", ROMREGION_ERASE00)

	ROM_REGION( 0x400000, "k054539", 0)
	ROM_LOAD( "168a06.1c", 0x000000, 2*1024*1024, CRC(25404fd7) SHA1(282cf523728b38d0bf14d765dd7257aa1fb2af39) )
	ROM_LOAD( "168a07.1e", 0x200000, 2*1024*1024, CRC(fdbbf8cc) SHA1(a8adf72a25fe2b9c4c338350d02c92deb5f8c8e9) )

	ROM_REGION( 0x80, "eeprom", 0 )
	ROM_LOAD( "viostorm.nv", 0x0000, 0x080, CRC(3cb1c96c) SHA1(56ed6633e7108925a13ad6bc32381bfde592f70b) )
ROM_END

/*    YEAR  NAME            PARENT    MACHINE        INPUT       INIT             MONITOR COMPANY                 FULLNAME FLAGS */
// Martial Champion
GAME( 1993, mtlchamps01,   mtlchamp,        martchmp, martchmp, mystwarr_state, empty_init, ROT0,  "DDJ", "Martial Champion (Easy Move)", MACHINE_IMPERFECT_GRAPHICS )
// Monster Maulers
GAME( 1993, mmaulerss01,   mmaulers,        dadandrn, dadandrn, mystwarr_state, empty_init, ROT0,  "DDJ", "Monster Maulers (Easy Move)", MACHINE_IMPERFECT_GRAPHICS )
// Metamorphic Force
GAME( 2018, metamrphs01,   metamrph,        metamrph, metamrph, mystwarr_state, empty_init, ROT0,  "hack", "Metamorphic Force (Version Enhanced 2017-07-03)", MACHINE_IMPERFECT_GRAPHICS )
// Violent Storm
GAME( 2018, viostorms01,   viostorm,        viostorm, viostorm, mystwarr_state, empty_init, ROT0,  "hack", "Violent Storm (Enhanced Edition 2018-08-14)", MACHINE_IMPERFECT_GRAPHICS )
GAME( 2018, viostorms02,   viostorm,        viostorm, viostorm, mystwarr_state, empty_init, ROT0,  "hack", "Violent Storm (Enhanced Edition 2018-08-13)", MACHINE_IMPERFECT_GRAPHICS )
GAME( 2018, viostorms03,   viostorm,        viostorm, viostorm, mystwarr_state, empty_init, ROT0,  "hack", "Violent Storm (Enhanced Edition 2018-05-07)", MACHINE_IMPERFECT_GRAPHICS )
GAME( 2020, viostorms04,   viostorm,        viostorm, viostorm, mystwarr_state, empty_init, ROT0,  "hack", "Violent Storm (Enhanced Edition 2020-03-24)", MACHINE_IMPERFECT_GRAPHICS )
GAME( 2020, viostorms05,   viostorm,        viostorm, viostorm, mystwarr_state, empty_init, ROT0,  "hack", "Violent Storm (Enhanced Edition 2020-03-25)", MACHINE_IMPERFECT_GRAPHICS )

