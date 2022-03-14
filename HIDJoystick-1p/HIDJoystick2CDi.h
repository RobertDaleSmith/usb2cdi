/*
 * Copyright (C) 2021 Jeffrey Janssen - cdi@nmotion.nl
 * 
 * This software may be distributed and modified under the terms of the GNU
 * General Public License version 2 (GPL2) as published by the Free Software
 * Foundation and appearing in the file GPL2.TXT included in the packaging of
 * this file. Please note that GPL2 Section 2[b] requires that all works based
 * on this software must also be made publicly available under the terms of
 * the GPL2 ("Copyleft").
 */

#define JOY_DZ_AVG 0
#define JOY_DS_RNG 7500
#define JOY_DZ_MAX (JOY_DZ_AVG + JOY_DS_RNG)
#define JOY_DZ_MIN (JOY_DZ_AVG - JOY_DS_RNG)

#ifndef __HIDJoystick2CDi_h__
#define __HIDJoystick2CDi_h__

#include "hidjoystickrptparser.h"

class HIDJoystick2CDi {
	const int dSpeed[4] = {4, 8, 16, 32};
	const int aSpeed[4] = {8, 16, 32, 64};

  HIDUniversal Hid;
  JoystickEvents JoyEvents;
  JoystickReportParser Joy;
	CdiController cdi;
	int speed = 0;
	bool allowSpeed = true;

	void setSpeed(int delta) {
		if (allowSpeed) {
			speed = min(3, max(0, speed + delta));
			allowSpeed = false;
		}
	}

public:
	HIDJoystick2CDi(USB *usb, byte pin_rts, byte pin_rxd, byte player): Hid(usb), Joy(&JoyEvents), cdi(pin_rts, pin_rxd, MANEUVER, player) {}
	
	void Init() {
		cdi.Init();

    delay(200);

    if (!Hid.SetReportParser(0, &Joy))
      ErrorMessage<uint8_t > (PSTR("SetReportParser"), 1);
	}

	bool Task() {
		cdi.Task();
	
		//Alter playerSpeed
		if      (JoyEvents.button_run) setSpeed(+1);
		else if (JoyEvents.button_sel) setSpeed(-1);
		else allowSpeed = true;  
		
//		//Analog Stick
//		int lhx = 0;
//		int lhy = 0;
//
//    if      (JoyEvents.button_up)    lhy = -32767;
//    else if (JoyEvents.button_down)  lhy =  32767;
//
//    if      (JoyEvents.button_left)  lhy = -32767;
//    else if (JoyEvents.button_right) lhy =  32767;
//		
//		int x = map(lhx, -32767, 32767, -aSpeed[speed], +aSpeed[speed]);
//		int y = map(lhy, -32767, 32767, -aSpeed[speed], +aSpeed[speed]);
//		
//		if (lhx > JOY_DZ_MIN && lhx < JOY_DZ_MAX) x = 0;
//		if (lhy > JOY_DZ_MIN && lhy < JOY_DZ_MAX) y = 0;

    int x = 0;
    int y = 0;
		
		//Directional input . Overrides stick
		if (JoyEvents.button_left)  x = -dSpeed[speed];
		if (JoyEvents.button_right) x = +dSpeed[speed];
		if (JoyEvents.button_up)    y = -dSpeed[speed];
		if (JoyEvents.button_down)  y = +dSpeed[speed];
		
		//Buttons
		bool bt_1 = (JoyEvents.button_1) != 0;
		bool bt_2 = (JoyEvents.button_2) != 0;
		
		return cdi.JoyInput((byte)x, (byte)y, bt_1, bt_2);
	}
};

#endif
