/*
 * Sketch for the CD-i Controller Library using a MagicNS adapter with a Switch Pro/Switch SNES controller - 2 Players
 * By Jeffrey Janssen - cdi@nmotion.nl
 */
 
#include <usbhid.h>
#include <hiduniversal.h>
#include <usbhub.h>
#include <CdiController.h>
#include <SPI.h>
#include "HIDJoystick2CDi.h"

USB Usb;
USBHub UsbHub(&Usb);

HIDJoystick2CDi cdiPlayer(&Usb, 2, 3, 0); //Controller 1 -> RTS = 2, RXD = 3

void setup() {
  Serial.begin(115200);
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  Serial.println("Start");

	//Set pin 4 and 5 to support the level converter
	pinMode(4, OUTPUT);
	pinMode(5, OUTPUT);
	digitalWrite(4, HIGH); //LV
	digitalWrite(5, LOW);  //GND

	if (Usb.Init() == -1) Serial.println("OSC did not start.");

	cdiPlayer.Init();
}

void loop() { 
	Usb.Task();
	cdiPlayer.Task();
}
