// license:BSD-3-Clause
// copyright-holders:Fabio Priuli
/**********************************************************************

    SNK Neo Geo Dial controller emulation

**********************************************************************/

#pragma once

#ifndef __NEOGEO_IRRMAZE__
#define __NEOGEO_IRRMAZE__


#include "emu.h"
#include "ctrl.h"

//**************************************************************************
//  TYPE DEFINITIONS
//**************************************************************************

// ======================> neogeo_dial_device

class neogeo_irrmaze_device : public device_t,
						public device_neogeo_ctrl_edge_interface
{
public:
	// construction/destruction
	neogeo_irrmaze_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);

	// optional information overrides
	virtual ioport_constructor device_input_ports() const override;

protected:
	// device-level overrides
	virtual void device_start() override;
	virtual void device_reset() override;

	// device_neogeo_control_port_interface overrides
	virtual u8 in0_r() override;
	virtual u8 in1_r() override;
	virtual void write_ctrlsel(uint8_t data) override;

private:
	required_ioport m_tx;
	required_ioport m_ty;
	required_ioport m_buttons;
	uint8_t m_ctrl_sel = 0U;
};



// device type definition
DECLARE_DEVICE_TYPE(NEOGEO_IRRMAZE, neogeo_irrmaze_device)


#endif
