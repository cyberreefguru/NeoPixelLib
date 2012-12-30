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

#include "LocoNet.h"
#include "LnSwUart.h"
#include "LnConfig.h"

LocoNet::LocoNet()
{
}

void LocoNet::init()
{
  initLnBuf(&LnBuffer) ;
  initLocoNetHardware(&LnBuffer);
}

lnMsg* LocoNet::receive()
{
  return recvLnMsg(&LnBuffer);
}

LN_STATUS LocoNet::send(lnMsg *pPacket)
{
  unsigned char ucTry;
  unsigned char ucPrioDelay = LN_BACKOFF_INITIAL;
  LN_STATUS enReturn;
  unsigned char ucWaitForEnterBackoff;

  for (ucTry = 0; ucTry < LN_TX_RETRIES_MAX; ucTry++)
  {

    // wait previous traffic and than prio delay and than try tx
    ucWaitForEnterBackoff = 1;  // don't want to abort do/while loop before
    do                          // we did not see the backoff state once
    {
      enReturn = sendLocoNetPacketTry(pPacket, ucPrioDelay);

      if (enReturn == LN_DONE)  // success?
        return LN_DONE;

      if (enReturn == LN_PRIO_BACKOFF)
        ucWaitForEnterBackoff = 0; // now entered backoff -> next state != LN_BACKOFF is worth incrementing the try counter
    }
    while ((enReturn == LN_CD_BACKOFF) ||                             // waiting CD backoff
    (enReturn == LN_PRIO_BACKOFF) ||                           // waiting master+prio backoff
    ((enReturn == LN_NETWORK_BUSY) && ucWaitForEnterBackoff)); // or within any traffic unfinished
    // failed -> next try going to higher prio = smaller prio delay
    if (ucPrioDelay > LN_BACKOFF_MIN)
      ucPrioDelay--;
  }
  LnBuffer.stats.txErrors++ ;
  return LN_RETRY_ERROR;
}

LN_STATUS LocoNet::send(lnMsg *pPacket, uint8_t ucPrioDelay)
{
  return sendLocoNetPacketTry(pPacket, ucPrioDelay);
}

LN_STATUS LocoNet::send( uint8_t opCode, uint8_t data1, uint8_t data2 )
{
  lnMsg sendPacket ;

  sendPacket.data[ 0 ] = opCode ;
  sendPacket.data[ 1 ] = data1 ;
  sendPacket.data[ 2 ] = data2 ;

  return send( &sendPacket ) ;
}

LN_STATUS LocoNet::send( uint8_t opCode, uint8_t data1, uint8_t data2, uint8_t prioDelay )
{
  lnMsg sendPacket ;

  sendPacket.data[ 0 ] = opCode ;
  sendPacket.data[ 1 ] = data1 ;
  sendPacket.data[ 2 ] = data2 ;

  return sendLocoNetPacketTry( &sendPacket, prioDelay ) ;
}

uint8_t LocoNet::processSwitchSensorMessage( lnMsg *LnPacket )
{
  uint16_t Address ;
  uint8_t  Direction ;
  uint8_t  Output ;
  uint8_t  ConsumedFlag = 1 ;

  Address = (LnPacket->srq.sw1 | ( ( LnPacket->srq.sw2 & 0x0F ) << 7 )) ;
  if( LnPacket->sr.command != OPC_INPUT_REP )
    Address++;

  switch( LnPacket->sr.command )
  {
  case OPC_INPUT_REP:
    Address <<= 1 ;
    Address += ( LnPacket->ir.in2 & OPC_INPUT_REP_SW ) ? 2 : 1 ;

    if(notifySensor)
      notifySensor( Address, LnPacket->ir.in2 & OPC_INPUT_REP_HI ) ;
    break ;

  case OPC_SW_REQ:
    if(notifySwitchRequest)
      notifySwitchRequest( Address, LnPacket->srq.sw2 & OPC_SW_REQ_OUT, LnPacket->srq.sw2 & OPC_SW_REQ_DIR ) ;
    break ;

  case OPC_SW_REP:
    if(notifySwitchReport)
      notifySwitchReport( Address, LnPacket->srp.sn2 & OPC_SW_REP_HI, LnPacket->srp.sn2 & OPC_SW_REP_SW ) ;
    break ;

  case OPC_SW_STATE:
    Direction = LnPacket->srq.sw2 & OPC_SW_REQ_DIR ;
    Output = LnPacket->srq.sw2 & OPC_SW_REQ_OUT ;

    if(notifySwitchState)
      notifySwitchState( Address, Output, Direction ) ;
    break;

  case OPC_SW_ACK:
    break ;

  case OPC_LONG_ACK:
    if( LnPacket->lack.opcode == (OPC_SW_STATE & 0x7F ) )
    {
      Direction = LnPacket->lack.ack1 & 0x01 ;
    }
    else
      ConsumedFlag = 0 ;
    break;

  default:
    ConsumedFlag = 0 ;
  }

  return ConsumedFlag ;
}

void LocoNet::requestSwitch( uint16_t address, uint8_t output, uint8_t direction )
{
  uint8_t AddrH = (--address >> 7) & 0x0F ;
  uint8_t AddrL = address & 0x7F ;

  if( output )
    AddrH |= OPC_SW_REQ_OUT ;

  if( direction )
    AddrH |= OPC_SW_REQ_DIR ;

  send( OPC_SW_REQ, AddrL, AddrH ) ;
}

void LocoNet::reportSwitch( uint16_t address )
{
  address -= 1;
  send( OPC_SW_STATE, (address & 0x7F), ((address >> 7) & 0x0F) ) ;
}

LocoNet LOCONET = LocoNet();
