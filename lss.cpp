/*
This file is part of CanFestival, a library implementing CanOpen Stack. 


Copyright (C): Jorge Berzosa


See COPYING file for copyrights details.


This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.


This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.


You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/*!
** @file   lss.c
** @author Jorge Berzosa
** @date   Mon Oct  22 05:44:32 2007
**
** @brief
**
**
*/
// #include <stdlib.h>

#include "data.h"
#include "lss.h"
#include "sysdep.h"

#ifdef CO_ENABLE_LSS

#define LSS_TIMEOUT_MS (TIMEVAL)1000 /* ms */
//#define LSS_FS_TIMEOUT_MS	(TIMEVAL)100  /* ms */

/* Returns the LSS ident field from a Message struct */
#define getLSSIdent(msg) (((UNS32)msg->data[4] << 24) | ((UNS32)msg->data[3] << 16) | (msg->data[2] << 8) | (msg->data[1]));

/* Returns the LSS switch delay field from a Message struct */
// #define getLSSDelay(msg) ((msg->data[2] << 8) | (msg->data[1]))

// void LssAlarmMSG(UNS32 id);
// void LssAlarmSDELAY(UNS32 id);

// #define StopLSS_MSG_TIMER()                                                                \
// 	{                                                                                      \
// 		MSG_WAR(0x3D01, "StopLSS_MSG_TIMER", 0);                                           \
// 		ObjDict_Data.lss_transfer.timerMSG = DelAlarm(ObjDict_Data.lss_transfer.timerMSG); \
// 	}

// #define StartLSS_MSG_TIMER()                                                                              \
// 	{                                                                                                     \
// 		MSG_WAR(0x3D02, "StartLSS_MSG_TIMER", 0);                                                         \
// 		ObjDict_Data.lss_transfer.timerMSG = SetAlarm(0, &LssAlarmMSG, MS_TO_TIMEVAL(LSS_TIMEOUT_MS), 0); \
// 	}

// #define StopLSS_SDELAY_TIMER()                                                                   \
// 	{                                                                                            \
// 		MSG_WAR(0x3D03, "StopLSS_SDELAY_TIMER", 0);                                              \
// 		ObjDict_Data.lss_transfer.timerSDELAY = DelAlarm(ObjDict_Data.lss_transfer.timerSDELAY); \
// 	}

// #define StartLSS_SDELAY_TIMER()                                                                                                                                                           \
// 	{                                                                                                                                                                                     \
// 		MSG_WAR(0x3D04, "StartLSS_SDELAY_TIMER", 0);                                                                                                                                      \
// 		ObjDict_Data.lss_transfer.timerSDELAY = SetAlarm(0, &LssAlarmSDELAY, MS_TO_TIMEVAL(ObjDict_Data.lss_transfer.switchDelay), MS_TO_TIMEVAL(ObjDict_Data.lss_transfer.switchDelay)); \
// 	}

// void LssAlarmMSG(UNS32 id)
// {
// 	(void)id;
// 	StopLSS_MSG_TIMER();

// 	if (ObjDict_Data.lss_transfer.command == LSS_IDENT_REMOTE_NON_CONF)
// 	{
// 		MSG_WAR(0x2D08, "LSS timeout. There are not no-configured slaves in the net", 0);
// 		ObjDict_Data.lss_transfer.state = LSS_FINISHED;
// 		ObjDict_Data.lss_transfer.dat1 = 1;
// 	}
// 	else
// 	{
// 		MSG_ERR(0x1D09, "LSS timeout. LSS response not received.", 0);
// 		MSG_WAR(0x2D0A, "LSS timeout command specifier : ", ObjDict_Data.lss_transfer.command);
// 		/* Set aborted state */
// 		ObjDict_Data.lss_transfer.state = LSS_ABORTED_INTERNAL;
// 	}

// 	/* Call the user function to inform of the problem.*/
// 	if (ObjDict_Data.lss_transfer.Callback)
// 	{
// 		/*If there is a callback, it is responsible of the error*/
// 		(*ObjDict_Data.lss_transfer.Callback)(ObjDict_Data.lss_transfer.command);
// 	}
// }

/*!                                                                                                
**                                                                                                 
**                                                                                                 
** @param d                                                                                        
** @param id                                                                                       
**/
// void LssAlarmSDELAY(UNS32 id)
// {
// 	(void)id;
// 	/* The first switch_delay period expired. Store the node state, change it 
//  	 * so no CAN messages will be sent or received, call the ChangeBaudRate function*/
// 	if (ObjDict_Data.lss_transfer.switchDelayState == SDELAY_FIRST)
// 	{
// 		MSG_WAR(0x3D0B, "LSS switch delay first period expired", 0);
// 		ObjDict_Data.lss_transfer.switchDelayState = SDELAY_SECOND;
// 		//(*d->lss_ChangeBaudRate)(d,d->lss_transfer.baudRate);
// 		// canChangeBaudRate(d->lss_transfer.canHandle_t, d->lss_transfer.baudRate); // BG
// 	}
// 	else
// 	{ /* d->lss_transfer.switchDelayState==SDELAY_SECOND */
// 		MSG_WAR(0x3D0C, "LSS switch delay second period expired", 0);
// 		ObjDict_Data.lss_transfer.switchDelayState = SDELAY_OFF;
// 		StopLSS_SDELAY_TIMER();
// 	}
// }

void startLSS()
{
	/*MSG_WAR(0x3D09, "LSS services started",0);*/
}

void stopLSS()
{
	/*MSG_WAR(0x3D09, "LSS services stopped",0);*/
}

#ifdef EV
UNS32 BytesToSerialNum(UNS8 *buf, UNS8 start) {
  UNS32 Id = 0x00000000;
  UNS8 j = 3;
  for (UNS8 i = start; i > 0 && i> start -4; i--) {
    UNS32 num = (UNS32)buf[i] << (j * 8);
    Id |= num;
    j--;
  }
  return Id;
}
#endif

UNS8 sendSlaveLSSMessage(UNS8 command, void *dat1, void *dat2)
{
	Message m;
	UNS8 i;

	if (!ObjDict_Data.CurrentCommunicationState.csLSS)
	{
		// MSG_WAR(0x2D17, "unable to send the LSS message, not in the proper state =>", ObjDict_Data.nodeState);
		return 0xFF;
	}

	for (i = 1; i < 8; i++)
		m.data[i] = 0;
	m.len = 8;
	m.rtr = NOT_A_REQUEST;
	m.data[0] = command;
	m.cob_id = UNS16_LE(SLSS_ADRESS);

	/* Tha data sent with the msg depends on the command */
	switch (command)
	{
	case LSS_INQ_NODE_ID: /* Inquire Node-ID */
		m.data[1] = *(UNS8 *)dat1;
		break;
	case LSS_CONF_NODE_ID:	/* Configure Node-ID */
	case LSS_CONF_BIT_TIMING: /* Configure Bit Timing Parameters */
	case LSS_CONF_STORE:	  /* Store Configured Parameters */
		m.data[1] = *(UNS8 *)dat1;
		m.data[2] = *(UNS8 *)dat2;
		break;
	case LSS_INQ_VENDOR_ID:		/* Inquire Identity Vendor-ID */
	case LSS_INQ_PRODUCT_CODE:  /* Inquire Identity Product-Code */
	case LSS_INQ_REV_NUMBER:	/* Inquire Identity Revision-Number */
	case LSS_INQ_SERIAL_NUMBER: /* Inquire Identity Serial-Number */
		m.data[1] = (UNS8)(*(UNS32 *)dat1 & 0xFF);
		m.data[2] = (UNS8)(*(UNS32 *)dat1 >> 8 & 0xFF);
		m.data[3] = (UNS8)(*(UNS32 *)dat1 >> 16 & 0xFF);
		m.data[4] = (UNS8)(*(UNS32 *)dat1 >> 24 & 0xFF);
		break;
	case LSS_SM_SELECTIVE_RESP:	/* Switch Mode Selective response*/
	case LSS_IDENT_SLAVE:		   /* LSS Identify Slave */
	case LSS_IDENT_NON_CONF_SLAVE: /* LSS identify non-configured remote slave */
		break;
	default:
		MSG_ERR(0x1D18, "send Slave LSS command not implemented", command);
		return 0xFF;
	}

	return canSend(&m);
}


UNS8 sendLSS(UNS8 command, void *dat1, void *dat2)
{
	UNS8 res = 1;
	res = sendSlaveLSSMessage(command, dat1, dat2);
	return res;
}

UNS8 proceedLSS_Slave(Message *m)
{

	#ifdef EV
	const subindex *si;
    UNS8 si_size;
    ODCallback_t *callbacks;

	UNS32 msgSerialNo = BytesToSerialNum(m->data, 7);

	if (si = ObjDict_scanIndexOD(0x1018, &si_size, &callbacks)) {
		UNS32* deviceSerialNo = (UNS32*) si[4].pObject;
		if (msgSerialNo !=  *deviceSerialNo) {
			// Serial.println(F("This LSS is not for me"));
			// Serial.print(F("msgSerialNo:"));
			// Serial.println(msgSerialNo, HEX);
			// Serial.print(F("deviceSerialNo:"));
			// Serial.println(*deviceSerialNo, HEX);
			return 0;
		}
	} else {
		// Serial.println(F("No SN on LSS"));
		return 0xFF;
	}
            
	#endif
	
	UNS8 msg_cs;
	// Serial.print(F("SlaveLSS proceedLSS; command: "));
	// Serial.println(m->data[0]);

	MSG_WAR(0x3D21, "SlaveLSS proceedLSS; command ", m->data[0]);

	switch (msg_cs = m->data[0])
	{
	case LSS_SM_GLOBAL: /* Switch Mode Global */
		/* if there is not a mode change break*/
		if (m->data[1] == ObjDict_Data.lss_transfer.mode)
		{
			// Serial.print(F("SlaveLSS already in the mode"));
			MSG_WAR(0x3D22, "SlaveLSS already in the mode ", m->data[1]);
			break;
		}

		if (m->data[1] == LSS_CONFIGURATION_MODE)
		{
			MSG_WAR(0x3D23, "SlaveLSS switching to configuration mode ", 0);
			/* Store the NodeId in case it will be changed */
			ObjDict_Data.lss_transfer.nodeID= getNodeId();
			ObjDict_Data.lss_transfer.mode = LSS_CONFIGURATION_MODE;
		}
		else if (m->data[1] == LSS_WAITING_MODE)
		{
			MSG_WAR(0x3D24, "SlaveLSS switching to operational mode ", 0);

			/* If the nodeID has changed update it and put the node state to Initialisation. */
			if (ObjDict_Data.lss_transfer.nodeID != getNodeId())
			{
				if (getNodeId() == 0xFF)
				{ /* The nodeID was 0xFF; initialize the application*/
					MSG_WAR(0x3D25, "The node Id has changed. Reseting to Initialisation state", 0);
					// Serial.print("The node Id has changed. Reseting to Initialisation state");
					setNodeId(ObjDict_Data.lss_transfer.nodeID);
					setState(Initialisation);
					Serial.print(F("New NodeId: "));
					Serial.println(ObjDict_bDeviceNodeId);
				}
				else
				{ /* The nodeID will be changed on NMT_Reset_Comunication Request*/
				}
			}
			ObjDict_Data.lss_transfer.mode = LSS_WAITING_MODE;
		}
		break;
	case LSS_CONF_NODE_ID: /* Configure Node-ID */
	{
		UNS8 error_code = 0;
		UNS8 spec_error = 0;

		if (ObjDict_Data.lss_transfer.mode == LSS_CONFIGURATION_MODE)
		{
			if (m->data[1] > 127 && m->data[1] != 0xFF)
			{
				MSG_ERR(0x1D26, "NodeID out of range", 0);
				error_code = 1; /* NodeID out of range */
			}
			else
			{
				Serial.print(F("SET NodeId"));
				Serial.println(m->data[1]);
				ObjDict_Data.lss_transfer.nodeID = m->data[1];
			}
		}
		else
		{
			MSG_WAR(0x3D27, "SlaveLSS not in configuration mode", 0);
			//error_code=0xFF;
			break;
		}
		sendSlaveLSSMessage(msg_cs, &error_code, &spec_error);
	}
	break;
	// case LSS_CONF_STORE: /* Store Configured Parameters */
	// {
	// 	UNS8 error_code=0;
	// 	UNS8 spec_error=0;

	// 	if(ObjDict_Data.lss_transfer.mode==LSS_CONFIGURATION_MODE){
	// 		if(ObjDict_Data.lss_StoreConfiguration){
	// 			 /* call lss_StoreConfiguration with NodeId */
	//   			(*ObjDict_Data.lss_StoreConfiguration)(&error_code,&spec_error);
	// 		}
	// 		else{
	// 			MSG_ERR(0x1D2E, "Store configuration not supported",0);
	// 			error_code=1; /* store configuration is not supported */
	// 		}
	// 	}
	// 	else{
	// 		MSG_WAR(0x3D2F, "SlaveLSS not in configuration mode",0);
	// 		//error_code=0xFF;
	// 		break;
	// 	}
	// 	sendSlaveLSSMessage(msg_cs,&error_code,&spec_error);
	// }
	// break;
	case LSS_SM_SELECTIVE_VENDOR: /* Switch Mode Selective */
	case LSS_SM_SELECTIVE_PRODUCT:
	case LSS_SM_SELECTIVE_REVISION:
	case LSS_SM_SELECTIVE_SERIAL:
	{
		UNS32 _SpecificNodeInfo;
		const subindex *si;
		UNS8 si_size;
		ODCallback_t *callbacks;

		if (ObjDict_Data.lss_transfer.mode == LSS_CONFIGURATION_MODE)
		{
			MSG_ERR(0x1D30, "Switch Mode Selective only supported in operational mode", 0);
			break;
		}

		_SpecificNodeInfo = getLSSIdent(m);

		if (!(si = ObjDict_scanIndexOD(0x1018, &si_size, &callbacks)))
		{
			return 0xFF;
		}

		if (_SpecificNodeInfo == (UNS32)si[msg_cs - (LSS_SM_SELECTIVE_VENDOR - 1)].pObject)
		{

			ObjDict_Data.lss_transfer.addr_sel_match |= (0x01 << (msg_cs - LSS_SM_SELECTIVE_VENDOR));
			/* If all the fields has been set */
			if (ObjDict_Data.lss_transfer.addr_sel_match == 0x0F)
			{

				MSG_WAR(0x3D31, "SlaveLSS switching to configuration mode ", 0);
				ObjDict_Data.lss_transfer.addr_sel_match = 0;
				ObjDict_Data.lss_transfer.nodeID = getNodeId();
				ObjDict_Data.lss_transfer.mode = LSS_CONFIGURATION_MODE;

				sendSlaveLSSMessage(LSS_SM_SELECTIVE_RESP, 0, 0);
			}
		}
		else
		{
			MSG_WAR(0x3D32, "LSS identity field doesn't match ", _SpecificNodeInfo);
			ObjDict_Data.lss_transfer.addr_sel_match = 0;
		}
	}
	break;
	case LSS_IDENT_REMOTE_VENDOR: /* LSS Identify Remote Slaves */
	case LSS_IDENT_REMOTE_PRODUCT:
	case LSS_IDENT_REMOTE_REV_LOW:
	case LSS_IDENT_REMOTE_REV_HIGH:
	case LSS_IDENT_REMOTE_SERIAL_LOW:
	case LSS_IDENT_REMOTE_SERIAL_HIGH:
	{
		UNS32 _SpecificNodeInfo;
		const subindex *si;
		UNS8 si_size;
		ODCallback_t *callbacks;

		_SpecificNodeInfo = getLSSIdent(m);

		if (!(si = ObjDict_scanIndexOD(0x1018, &si_size, &callbacks)))
		{
			return 0xFF;
		}

		/* Check if the data match the identity object. */
		switch (msg_cs)
		{
		case LSS_IDENT_REMOTE_VENDOR:
			ObjDict_Data.lss_transfer.addr_ident_match = (_SpecificNodeInfo == (UNS32)si[1].pObject) ? ObjDict_Data.lss_transfer.addr_ident_match | 0x01 : 0;
			break;
		case LSS_IDENT_REMOTE_PRODUCT:
			ObjDict_Data.lss_transfer.addr_ident_match = (_SpecificNodeInfo == (UNS32)si[2].pObject) ? ObjDict_Data.lss_transfer.addr_ident_match | 0x02 : 0;
			break;
		case LSS_IDENT_REMOTE_REV_LOW:
			ObjDict_Data.lss_transfer.addr_ident_match = (_SpecificNodeInfo <= (UNS32)si[3].pObject) ? ObjDict_Data.lss_transfer.addr_ident_match | 0x04 : 0;
			break;
		case LSS_IDENT_REMOTE_REV_HIGH:
			ObjDict_Data.lss_transfer.addr_ident_match = (_SpecificNodeInfo >= (UNS32)si[3].pObject) ? ObjDict_Data.lss_transfer.addr_ident_match | 0x08 : 0;
			break;
		case LSS_IDENT_REMOTE_SERIAL_LOW:
			ObjDict_Data.lss_transfer.addr_ident_match = (_SpecificNodeInfo <= (UNS32)si[4].pObject) ? ObjDict_Data.lss_transfer.addr_ident_match | 0x10 : 0;
			break;
		case LSS_IDENT_REMOTE_SERIAL_HIGH:
			ObjDict_Data.lss_transfer.addr_ident_match = (_SpecificNodeInfo >= (UNS32)si[4].pObject) ? ObjDict_Data.lss_transfer.addr_ident_match | 0x20 : 0;
			break;
		}
		/* If all the fields has been set.. */
		if (ObjDict_Data.lss_transfer.addr_ident_match == 0x3F)
		{
			MSG_WAR(0x3D33, "SlaveLSS identified ", 0);
			ObjDict_Data.lss_transfer.addr_ident_match = 0;
			sendSlaveLSSMessage(LSS_IDENT_SLAVE, 0, 0);
		}
		else if (ObjDict_Data.lss_transfer.addr_ident_match == 0)
		{
			MSG_WAR(0x3D34, "LSS identify field doesn't match ", _SpecificNodeInfo);
		}
	}
	break;
	case LSS_IDENT_REMOTE_NON_CONF: /* LSS identify non-configured remote slave */
	{
		if (getNodeId() == 0xFF)
		{
			MSG_WAR(0x3D35, "SlaveLSS non-configured ", 0);
			sendSlaveLSSMessage(LSS_IDENT_NON_CONF_SLAVE, 0, 0);
		}
		else
		{
			MSG_WAR(0x3D36, "SlaveLSS already configured ", 0);
		}
	}
	break;
	case LSS_INQ_VENDOR_ID:		/* Inquire Identity Vendor-ID */
	case LSS_INQ_PRODUCT_CODE:  /* Inquire Identity Product-Code */
	case LSS_INQ_REV_NUMBER:	/* Inquire Identity Revision-Number */
	case LSS_INQ_SERIAL_NUMBER: /* Inquire Identity Serial-Number */
		if (ObjDict_Data.lss_transfer.mode == LSS_CONFIGURATION_MODE)
		{
			UNS32 _SpecificNodeInfo;
			const subindex *si;
			UNS8 si_size;
			ODCallback_t *callbacks;

			if (!(si = ObjDict_scanIndexOD(0x1018, &si_size, &callbacks)))
			{
				return 0xFF;
			}
			_SpecificNodeInfo = (UNS32)si[msg_cs - (LSS_INQ_VENDOR_ID - 1)].pObject;

			sendSlaveLSSMessage(msg_cs, &_SpecificNodeInfo, 0);
		}
		break;
	case LSS_INQ_NODE_ID: /* Inquire Node-ID */
		if (ObjDict_Data.lss_transfer.mode == LSS_CONFIGURATION_MODE)
		{
			UNS8 NodeID;

			NodeID = getNodeId();
			MSG_WAR(0x3D38, "SlaveLSS Node ID inquired ", NodeID);
			sendSlaveLSSMessage(msg_cs, &NodeID, 0);
		}
		else
		{
			MSG_WAR(0x3D39, "SlaveLSS not in configuration mode", 0);
		}
		break;
	default:
		MSG_ERR(0x1D40, "SlaveLSS command not implemented", msg_cs);
		return 0xFF;
	}

	return 0;
}

//void _lss_StoreConfiguration(UNS8 *error, UNS8 *spec_error){printf("_lss_StoreConfiguration\n");}

#endif