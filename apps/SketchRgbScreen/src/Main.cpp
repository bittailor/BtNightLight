//*************************************************************************************************
//
//  BITTAILOR.CH - ArduinoScreenMaster
//
//-------------------------------------------------------------------------------------------------
//
//  Main
//  
//*************************************************************************************************

#include <Bt/Util/Delay.hpp>
#include <Bt/Util/Vector.hpp>
#include <Bt/Com/TwoWireClient.hpp>
#include <Bt/Com/Twi.hpp>
#include <Bt/Ui/Colorduino.hpp>
#include <Bt/Ui/RgbScreenProxy.hpp>
#include <Bt/Ui/RgbScreenServer.hpp>
#include <Bt/Ui/CompoundRgbScreen.hpp>
#include <Bt/Io/CapacitiveSensorInput.hpp>
#include <Arduino.h>
#include <new.h>
#include <Bt/Io/DigitalInput.hpp>
#include <Bt/Io/DigitalInputListener.hpp>
#include <Bt/Workcycle/MainWorkcycle.hpp>

#include "Settings.hpp"
#include "Image.hpp"
#include "ImageRepository.hpp"

//-------------------------------------------------------------------------------------------------

static const Bt::Ui::Color BLACK     (  0,  0,  0);
static const Bt::Ui::Color RED       (255,  0,  0);

//-------------------------------------------------------------------------------------------------

class ImageScroller {

   public:

      ImageScroller(Bt::Ui::I_RgbScreen& iScreen)
      : mCurrentIndex(0), mScreen(&iScreen) {
      }




      void next(bool iValue) {
         if (!iValue) {
            return;
         }

         uint8_t previousIndex = mCurrentIndex;

         mCurrentIndex++ ;
         if (mCurrentIndex >= ImageRepository::size()) {
            mCurrentIndex = 0;
         }
         drawNext(previousIndex);
      }

      void previous(bool iValue) {
         if (!iValue) {
            return;
         }

         uint8_t previousIndex = mCurrentIndex;

         if (mCurrentIndex <= 0) {
            mCurrentIndex = ImageRepository::size();
         }
         mCurrentIndex-- ;
         drawPrevious(previousIndex);
      }

      void draw() {
         ImageRepository::getImage(mCurrentIndex).draw(*mScreen);
         mScreen->repaint();
      }

      void drawNext(uint8_t iPreviousIndex) {
         for(uint8_t x = 0; x < Image::SIZE ; x++) {
            ImageRepository::getImage(iPreviousIndex).draw(*mScreen, 0, x, Image::SIZE-x);
            ImageRepository::getImage(mCurrentIndex).draw(*mScreen, Image::SIZE-x, 0, x);
            mScreen->repaint();
         }
         draw();
      }

      void drawPrevious(uint8_t iPreviousIndex) {
         for(uint8_t x = 0; x < Image::SIZE ; x++) {
            ImageRepository::getImage(iPreviousIndex).draw(*mScreen, x, 0, Image::SIZE-x);
            ImageRepository::getImage(mCurrentIndex).draw(*mScreen, 0, Image::SIZE-x, x);
            mScreen->repaint();
         }
         draw();
      }



   private:
      uint8_t mCurrentIndex;
      Bt::Ui::I_RgbScreen* mScreen;
};

//-------------------------------------------------------------------------------------------------

void setup() {
  Serial.begin(9600);
   Serial.println("setup");
}

void loop() {
   uint8_t dimmFactor = 32;

   Serial.println("s0");

   // I2C slaves
   Bt::Com::Twi twi;
   Bt::Com::TwoWireClient<Bt::Com::Twi> server1(twi,1);
   Bt::Com::TwoWireClient<Bt::Com::Twi> server2(twi,2);
   

   // I2C led matrix proxies
   Bt::Ui::RgbScreenProxy proxy1(server1);
   Bt::Ui::RgbScreenProxy proxy2(server2);
   
   // Combine led matrixes to compund screen
   Bt::Util::Vector<Bt::Ui::CompoundRgbScreen,1> screen;
   {
      Bt::Util::StaticMatrix<Bt::Ui::I_RgbScreen*,1,2> screens;
      screens(0,0) = &proxy1;
      screens(0,1) = &proxy2;
      screen.pushBack<Bt::Util::I_Matrix<Bt::Ui::I_RgbScreen*>&>(screens);
   }
   Serial.println("s1");

   Bt::Ui::I_RgbScreen& iScreen = screen[0];

   //while (true)
   {
      for (int y = 0 ; y < iScreen.height(); y++) {
         for (int x = 0 ; x < iScreen.width(); x++) {
            iScreen.fill(BLACK);
            iScreen.setPixel(x,y,RED.dimm(dimmFactor));   
            iScreen.repaint();
            delay(10);
         }
      }

      delay(200);
   }

   Serial.println("s2");

   ImageRepository::getImage(0).draw(iScreen);
   iScreen.repaint();

   Serial.println("s3");

   while(true){
   }

}

