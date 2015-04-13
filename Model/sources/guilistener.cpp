#include "../headers/guilistener.h"
#include "../../Controller/headers/simulator.h"
#include "../../AGSI_Defs.h"

static AGSIFUNCS m_agsi;
static uint16_t m_keyboardValue;

namespace Model {

    bool GuiListener::init(AGSIFUNCS Agsi) {
        m_agsi = Agsi;

        Controller::Simulator::simulator().buttons(buttons);
        Controller::Simulator::simulator().keyboard(keyboard);
        Controller::Simulator::simulator().potentiometers(potentiometers);

        return true;
    }

    uint16_t GuiListener::keyboardVal() {
        return m_keyboardValue;
    }

    void GuiListener::keyboard(uint16_t values) {
        m_keyboardValue = values;
        m_agsi.Message("Keyboard callback launched\n");

        int maskOutYounger = 0x01;
        int maskOutOlder = 0x02;
        uint8_t firstRow = 0 | ((values & 0x8000) | ((values & 0x800)<<3) |
             ((values & 0x80)<<6) | ((values&8)<<9));
        uint8_t secondRow =  0 | (((values & 0x4000)<<1) | ((values & 0x400)<<4) |
                                  ((values & 0x40)<<7) | ((values&4)<<10));
        uint8_t thirdRow =  0 | (((values & 0x2000)<<2) | ((values & 0x200)<<5) |
                                  ((values & 0x20)<<8) | ((values&2)<<11));
        uint8_t fourthRow =  0 | (((values & 0x1000)<<3) | ((values & 0x100)<<6) |
                                  ((values & 0x10)<<9) | ((values&1)<<12));

        DWORD port1;
        DWORD prevVal;
        BYTE result=0;
        m_agsi.ReadSFR(m_port1Addr, &port1, &prevVal, 0xFF);

        DWORD maskedPort = port1 & 0x03;

        switch(maskedPort){
        case 0:
            result = firstRow;
            break;
        case 1:
            result = secondRow;
            break;
        case 2:
            result = thirdRow;
            break;
        case 3:
            result=fourthRow;
            break;
        default:
            m_agsi.Message("Ooops executed default in keyboard model\n");
            break;
        }

        result >>=2;

        m_agsi.WriteSFR(m_port1Addr,result,0x3C);
        m_agsi.UpdateWindows();
    }

    void GuiListener::potentiometers(int which, double newVoltage) {
        m_agsi.Message("potentiometer callback launched\n");
        m_agsi.UpdateWindows();
    }

    void GuiListener::buttons(uint8_t buttons) {
        buttons |= 0xF0;
        m_agsi.WriteMemory(m_buttonsExtAddr, 1, &buttons);
        m_agsi.UpdateWindows();
    }

}
