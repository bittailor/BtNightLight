#include <Arduino.h>
#include <Wire.h>
#include <Bt/Ui/Colorduino.hpp>
#include <Bt/Ui/RgbScreenServer.hpp>
#include <Bt/Com/Twi.hpp>
#include <Bt/Com/TwoWireServer.hpp>


void drawAddress(uint8_t iAddress, Bt::Ui::I_RgbScreen& iScreen, const Bt::Ui::Color& iColor) {
   for (size_t y = 0 ; y < iScreen.height() ; y++) {
      for (size_t x = 0 ; x < iScreen.width() ; x++) {
         if ((x + y) >= iAddress) {

            return;
         }
         iScreen.setPixel(x,y,iColor);
      }
   }
}

void setup() {
  Serial.begin(9600);
}

void loop() {
   uint8_t address = 3;
   uint8_t dimmFactor = 32;

   Bt::Ui::Color red   (255,  0,  0);
   Bt::Ui::Color orange(238,154,  0);
   Bt::Ui::Color green (  0,255,  0);
   Bt::Ui::Color black (  0,  0,  0);

   Bt::Ui::Colorduino colorduino(1);
   colorduino.fill(black);
   colorduino.repaint();
   Bt::Ui::RgbScreenServer server(colorduino);
   Bt::Com::Twi twi;
   Bt::Com::TwoWireServer<Bt::Com::Twi> twiServer(twi,address,server);

   for(int y = 0 ; y < colorduino.height() ; y++ ) {
      for(int x = 0 ; x < colorduino.width() ; x++ ) {
         colorduino.fill(black);
         colorduino.setPixel(x,y,red.dimm(dimmFactor));
         colorduino.repaint();
         delay(10);   
      }
   }
   
   colorduino.fill(black);
   drawAddress(address,colorduino,green.dimm(dimmFactor));
   colorduino.repaint();

   while(true) {
      delay(1000);
   }
}