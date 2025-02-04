#include <SPI.h>
#include <TimeLib.h> //include the Arduino Time library
#include <BLEPeripheral.h>
#include "lib_RenderBuffer.h"
#include "lib_StringBuffer.h"

TinyScreen display = TinyScreen(TinyScreenPlus);
RenderBuffer<uint8_t,20> buffer;

#define BLE_REQ 10
#define BLE_RDY 2
#define BLE_RST 9

BLEPeripheral blePeripheral = BLEPeripheral(BLE_REQ, BLE_RDY, BLE_RST);

BLEService service = BLEService("CCC0");
BLECharCharacteristic characteristic = BLECharCharacteristic("CCC1", BLERead | BLENotify);
BLEDescriptor desc = BLEDescriptor("2901", "value");

void setup() {
  BLESetup();
  display.begin();
  display.setFlip(true);
  display.setBrightness(8);
  display.setBitDepth(buffer.is16bit());

  setTime(13,19,55,6,3,2016); //values in the order hr,min,sec,day,month,year
}

void loop(void) {
  blePeripheral.poll();
}

void BLESetup(){
  blePeripheral.setLocalName("tinywatch");

  blePeripheral.setAdvertisedServiceUuid(service.uuid());
  blePeripheral.addAttribute(service);
  blePeripheral.addAttribute(characteristic);
  blePeripheral.addAttribute(desc);
  
  blePeripheral.setEventHandler(BLEConnected, blePeripheralConnectHandler);
  blePeripheral.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);
 
  blePeripheral.begin();
}

void blePeripheralConnectHandler(BLECentral& central) {
  if (central){
    buffer.drawText(stringBuffer.start().put("connected...").get(),15,8,buffer.rgb(255,0,0), &virtualDJ_5ptFontInfo);    
    buffer.flush(display);
    stringBuffer.reset();
  }
}

void blePeripheralDisconnectHandler(BLECentral& central) {
  if (central){
    buffer.drawText(stringBuffer.start().put("disconnected...").get(),15,8,buffer.rgb(255,0,0), &virtualDJ_5ptFontInfo);
    buffer.flush(display);
    stringBuffer.reset();
  }
}

void renderTime(unsigned int n){
  buffer.drawText(stringBuffer.start().putDec(hour()).put(":").putDec(minute()).put(":").putDec(second()).get(),15,8,buffer.rgb(255,0,0), &virtualDJ_5ptFontInfo);
}

void renderDate(unsigned int n){
  buffer.drawText(stringBuffer.start().putDec(month()).put("-").putDec(day()).put("-").putDec(year()).get(),15,16,buffer.rgb(255,0,0), &virtualDJ_5ptFontInfo);
}


//TODO: Method to render background.

//TODO: Method to render orientation.

//TODO: Method to handle button presses.

//TODO: Method to render menu.

//TODO: Method to handle sleep.

//TODO: Method to render notifications.

//TODO: whateva whatev we do what we want...
