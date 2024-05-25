#pragma once
#ifndef __STATES_H__
#define __STATES_H__

#include "Common.h"
#include "Hardware.h"

namespace States
{
  extern uint16_t EE_STATE;

  // Bootup 0
  void Initialization(Common::Payload_States &pay_states); // 1
  void Standby(Common::Payload_States &pay_states); // 2 
  void Deployment(Common::Payload_States &pay_states); // 3
  void Landing(Common::Payload_States &pay_states); // 4

  static void processCommand(bool enableCX, bool enableST, bool enableSIM, bool enableSIMP, bool en) 
  {
    String received;
    if (Hardware::read_ground_radio(received)) 
    {
      if (received.startsWith("CMD," + String(Common::TEAM_ID) + ",")) 
      {
        String cmd = received.substring(9,received.length());
        // CMD,<TEAM_ID>,CX,<ON_OFF>
        if (cmd.startsWith("CX")) {
          Hardware::lastCMD = "CX";
          if (cmd.endsWith("ON")) {
            Hardware::CX = true;
            EE_STATE = 2; // standby mode
            EEPROM.put(Common::ST_ADDR, EE_STATE);
          }
          else {
            Hardware::CX = false;
          }
        }
        else if (cmd.startsWith("ST")) {
          Hardware::lastCMD = "ST"; 
          if (cmd.endsWith("GPS")) {
            // set to GPS time
          }
          else {
            String utc = cmd.substring(2,cmd.length());
            // set to utc time
          }
        }
        else if (cmd.startsWith("SIM")) {
          Hardware::lastCMD = "SIM";
          if (cmd.endsWith("ENABLE")) {
            Hardware::SIM_ENABLE = true;
          }
          else if (cmd.endsWith("ACTIVATE")) {
            if (Hardware::SIM_ENABLE) {
              Hardware::SIM_ACTIVATE = true;
            }
          }
          else if (cmd.endsWith("DISABLE")) {
            Hardware::SIM_ENABLE = false;
            Hardware::SIM_ACTIVATE = false;
          }
        }
        else if (cmd.endsWith("CAL")) {
          Hardware::lastCMD = "CAL";
          Hardware::calibrated = true;
        }
      }
    }
  }
}
#endif
