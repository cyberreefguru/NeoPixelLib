#ifndef LOCONET_H_
#define LOCONET_H_

/****************************************************************************
 * 	Copyright (C) 2009 Alex Shepherd
 * 
 * 	Portions Copyright (C) Digitrax Inc.
 * 
 * 	This library is free software; you can redistribute it and/or
 * 	modify it under the terms of the GNU Lesser General Public
 * 	License as published by the Free Software Foundation; either
 * 	version 2.1 of the License, or (at your option) any later version.
 * 
 * 	This library is distributed in the hope that it will be useful,
 * 	but WITHOUT ANY WARRANTY; without even the implied warranty of
 * 	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * 	Lesser General Public License for more details.
 * 
 * 	You should have received a copy of the GNU Lesser General Public
 * 	License along with this library; if not, write to the Free Software
 * 	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 * 
 *****************************************************************************
 * 
 * 	IMPORTANT:
 * 
 * 	Some of the message formats used in this code are Copyright Digitrax, Inc.
 * 	and are used with permission as part of the EmbeddedLocoNet project. That
 * 	permission does not extend to uses in other software products. If you wish
 * 	to use this code, algorithm or these message formats outside of
 * 	EmbeddedLocoNet, please contact Digitrax Inc, for specific permission.
 * 
 * 	Note: The sale any LocoNet device hardware (including bare PCB's) that
 * 	uses this or any other LocoNet software, requires testing and certification
 * 	by Digitrax Inc. and will be subject to a licensing agreement.
 * 
 * 	Please contact Digitrax Inc. for details.
 * 
 *****************************************************************************
 * 	DESCRIPTION
 * 	This module provides functions that manage the sending and receiving of LocoNet packets.
 * 	
 * 	As bytes are received from the LocoNet, they are stored in a circular
 * 	buffer and after a valid packet has been received it can be read out.
 * 	
 * 	When packets are sent successfully, they are also appended to the Receive
 * 	circular buffer so they can be handled like they had been received from
 * 	another device.
 * 
 * 	Statistics are maintained for both the send and receiving of packets.
 * 
 * 	Any invalid packets that are received are discarded and the stats are
 * 	updated appropriately.
 * 
 *****************************************************************************/

#include "WProgram.h"

#include "utility/LnBuff.h"


class LocoNet
{
  private:
    LnBuf   LnBuffer ;

  public:
    LocoNet();
    void        init(void);
    lnMsg*      receive(void);
    LN_STATUS   send(lnMsg *TxPacket);
    LN_STATUS   send(lnMsg *TxPacket, uint8_t PrioDelay);
    LN_STATUS   send( uint8_t OpCode, uint8_t Data1, uint8_t Data2 );
    LN_STATUS   send( uint8_t OpCode, uint8_t Data1, uint8_t Data2, uint8_t PrioDelay );
    
    uint8_t processSwitchSensorMessage( lnMsg *LnPacket ) ;

    void requestSwitch( uint16_t Address, uint8_t Output, uint8_t Direction ) ;
    void reportSwitch( uint16_t Address ) ;
};

extern LocoNet LOCONET;

#if defined (__cplusplus)
	extern "C" {
#endif

extern void notifySensor( uint16_t Address, uint8_t State ) __attribute__ ((weak));
extern void notifySwitchRequest( uint16_t Address, uint8_t Output, uint8_t Direction ) __attribute__ ((weak));
extern void notifySwitchReport( uint16_t Address, uint8_t Output, uint8_t Direction ) __attribute__ ((weak));
extern void notifySwitchState( uint16_t Address, uint8_t Output, uint8_t Direction ) __attribute__ ((weak));

#if defined (__cplusplus)
}
#endif

#endif
