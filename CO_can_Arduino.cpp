#include "CO_can.h"
#include "CO_ErrorState.h"
#include "applicfg.h"
#include "states.h"
#include "config.h"

// TODO: add rx and tx queue to can driver and make interrupt driven
// TODO: make CS selectable via template
MCP_CAN CAN(10); // TODO add CS to CO_Init call!

UNS8 canSend(Message *m)
{
    if (CAN.sendMsgBuf((uint32_t)m->cob_id, 0, m->rtr, m->len, m->data) == CAN_FAILTX)
    {
        Serial.println(F("Can Send Failed"));
        if (CAN.checkError())
            setTxErrorState(tx_error);
        else
            setTxErrorState (tx_all_busy);
    }
    else
    {
        if (isTxNoError()) {
            flashRed();
        }
    }
    return 0;
}

void initCAN()
{

#ifdef EV_8MHZ    
    while (CAN_OK != CAN.begin(CAN_250KBPS, MCP_8MHz)) 
#else
    while (CAN_OK != CAN.begin(CAN_250KBPS, MCP_16MHz)) // BG changed from MCP_8MHz  // init can bus : baudrate = 10k
#endif    
    {
        Serial.println(F("CAN BUS Module Failed to Initialized"));
        Serial.println(F("Retrying...."));
        delay(2000);
    }
    Serial.println(F("CAN BUS Module Initialized"));

      CAN.init_Mask(0,0, 0x007E); // allow two consecutive ids
      CAN.init_Filt(0,0, getNodeId()); // RxPDO and SDO
      CAN.init_Filt(1,0, 0x07E5); // lss


      CAN.init_Mask(1,0, 0x07FF);
      CAN.init_Filt(2,0, 0x0000); // NMT
      CAN.init_Filt(3,0, 0x0080); // sync
      CAN.init_Filt(4,0, 0x0100); // time stamp
      CAN.init_Filt(5,0, 0x0600); // special query SDO
}
/*
if( (IncomingID & Mask) == (Filter & Mask))
    //Accept the message
 else
    //Discard the message
*/
/*
int Mask1 =   00001111110;
int Filter2 = 00000000000;
int MF  = Mask1 & Filter2; 
//=>          00001111110
       
int canid = 00010000000;
int Mask1 = 00001111110;
int IM = canid & Mask1;
// =>       00011111110

int Mask2 = 01111111111;
//=>        01111111111
            01111111111

            01111110101

*/