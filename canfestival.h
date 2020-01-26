/*
 This file is part of CanFestival, a library implementing CanOpen Stack.

 Copyright (C): Edouard TISSERANT and Francis DUPIN
 AVR Port: Andreas GLAUSER and Peter CHRISTEN

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

#ifndef __CAN_CANFESTIVAL__
#define __CAN_CANFESTIVAL__

#include "applicfg.h"
#include "data.h"
#include "CO_timer.h"
#include "objacces.h"
#include "digitalWriteFast.h"
#include "CO_ErrorState.h"


int blinkState = 0;
int error =0;

// ---------  to be called by user app ---------

template<const int redLEDPin, const int greenLEDPin> class CO {
public:
    CO() :
            m(Message_Initializer) {
    }
    ;
    void CO_Cycle();
    void CO_Init();
    static void testBlink();

private:
    static void errorStateBlink(uint8_t dummy);

    Message m;
};

template<int redLEDPin, int greenLEDPin> void CO<redLEDPin, greenLEDPin>::CO_Init() {
#ifdef EV_LED    
    if (redLEDPin>0) { pinModeFast(redLEDPin, OUTPUT) };
    if (greenLEDPin>0) { pinModeFast(greenLEDPin, OUTPUT) };
    if (redLEDPin>0) { digitalWriteFast(redLEDPin, 1) };
    if (greenLEDPin>0) { digitalWriteFast(greenLEDPin, 1) };
#endif
    initCAN();

    UNS8 st = setState(Initialisation);
    Serial.print(F("State: "));
    Serial.println(st);
    
    UNS8 dataType; /* Unused */
    UNS32 ByteSize = 4;
    UNS8 tmp[] = { 0, 0, 0, 0, 0, 0, 0, 0 }; /* temporary space to hold bits */
    
    if (getODentry (0x1018, 4, tmp, &ByteSize, &dataType, 0) != OD_SUCCESSFUL) {
            MSG_ERR(0x1013, " Couldn't find mapped variable at index-subindex-size : ", (UNS32) (*pMappingParameter));
            Serial.print(F("Couldn\'t find mapped variable at index-subindex-size : "));
            // Serial.println((UNS32) (*pMappingParameter));
    } else {
        // UNS8 stmp[8] = { 0x10, 0x18, 0x04, 0x00, tmp[0], tmp[1], tmp[2], tmp[3] };
        
        // CAN.sendMsgBuf(0x180, 0, 8, stmp);
    }
    // SetAlarm(0, &errorStateBlink, 62, 62);


#ifdef EV_LED   
    if (redLEDPin>0) { digitalWriteFast(redLEDPin, 0) };
    if (greenLEDPin>0) { digitalWriteFast(greenLEDPin, 0) };
#endif
    // st = setState(Operational);
    // Serial.print(F("State: "));
    // Serial.println(st);
}

template<int redLEDPin, int greenLEDPin> void CO<redLEDPin, greenLEDPin>::CO_Cycle() {

    TimeDispatch();

    if ((CAN_MSGAVAIL == CAN.checkReceive()) && !CAN.checkError()) {
        CAN.readMsgBuf(&(m.len), m.data);
        m.cob_id = CAN.getCanId();
        // m.rtr = 0; // m_nRtr;
        m.rtr = CAN.isRemoteRequest();
        // Serial.println(m.cob_id, HEX);
        // Serial.println(m.rtr);
        // Serial.println(CAN.isExtendedFrame());
        if (isRxNoError()) {
            flashGreen();
        }
        canDispatch(&m);         // process it
    }

    // if (CAN.checkError()) {
    //     if (error == 0) {
    //         error = 1;
    //         byte e = CAN.getError();
    //         Serial.print(F("I'm in an error state: "));
    //         Serial.println(e);
    //         setState(e_nodeState::Stopped);
    //     }
    // }

    uint8_t ef = CAN.getError();
    if (ef & MCP_EFLG_TXWAR)
        setTxErrorState(tx_warning);

    if (ef & MCP_EFLG_TXBO) {  // MCP_CAN::EFlg_TxBusOff) {
        setTxErrorState(tx_bus_off);
    } else if (ef & MCP_EFLG_TXEP)
        setTxErrorState(tx_passive);
    else if (!isTxNoError()) {
        // if (CAN.checkTransmit())
            resetTxErrorState();
    }
    updateTxErrorState();

    if (ef && MCP_EFLG_RXWAR) //MCP_CAN::EFlg_RxWar)
        setRxErrorState(rx_warning);

    if (ef & (MCP_EFLG_RX1OVR | MCP_EFLG_RX0OVR))// (MCP_CAN::EFlg_Rx1Ovr | MCP_CAN::EFlg_Rx0Ovr))
        setRxErrorState(rx_overflow);
    else if (ef && MCP_EFLG_RXEP) // MCP_CAN::EFlg_RxEP)
        setRxErrorState(rx_passive);
    else if (greePatternStarted())
        resetRxErrorState();

    updateRxErrorState();
}

template<int redLEDPin, int greenLEDPin> void CO<redLEDPin, greenLEDPin>::errorStateBlink(uint8_t dummy) {
    if(nextRedBlinkState()) {
        if(redLEDPin>0) digitalWriteFast(redLEDPin, 1);
    } else {
        if(redLEDPin>0) digitalWriteFast(redLEDPin, 0);
    }

    if(nextGreenBlinkState()) {
        if(greenLEDPin>0) digitalWriteFast(greenLEDPin, 1);
    } else {
        if(greenLEDPin>0) digitalWriteFast(greenLEDPin, 0);
    }
}

template<int redLEDPin, int greenLEDPin> void CO<redLEDPin, greenLEDPin>::testBlink() {
    if(redLEDPin>0) digitalWriteFast(redLEDPin, !blinkState);
    if(greenLEDPin>0) digitalWriteFast(greenLEDPin, blinkState);
    blinkState = !blinkState;
}

#endif
