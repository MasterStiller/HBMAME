// license:BSD-3-Clause
// copyright-holders:Fabio Priuli
/**********************************************************************

    SNK Neo Geo Kizuna 4Players controller emulation

**********************************************************************/

#pragma once

#ifndef __NEOGEO_KIZ4P__
#define __NEOGEO_KIZ4P__


#include "emu.h"
#include "ctrl.h"

//**************************************************************************
//  TYPE DEFINITIONS
//**************************************************************************

// ======================> neogeo_kizuna4p_device

class neogeo_kizuna4p_device : public device_t,
						public device_neogeo_ctrl_edge_interface
{
public:
	// construction/destruction
	neogeo_kizuna4p_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);

	// optional information overrides
	virtual ioport_constructor device_input_ports() const override;

protected:
	// device-level overrides
	virtual void device_start() override;
	virtual void device_reset() override;

	// device_neogeo_control_port_interface overrides
	virtual u8 in0_r() override;
	virtual u8 in1_r() override;
	virtual uint8_t read_start_sel() override;
	virtual void write_ctrlsel(uint8_t data) override;

private:
	required_ioport m_joy1;
	required_ioport m_joy2;
	required_ioport m_joy3;
	required_ioport m_joy4;
	required_ioport m_ss1;
	required_ioport m_ss2;
	uint8_t m_ctrl_sel = 0U;
};



// device type definition
DECLARE_DEVICE_TYPE(NEOGEO_KIZ4P, neogeo_kizuna4p_device)


#endif
