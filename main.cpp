#include "Arduino.h"
#include "kline.h"
#include <Wire.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &Wire);

k_line_prop kline (&Serial1, 10400, 14, 600);


void setup()
{
  Serial.begin(115200);
  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  delay(1000);
  display.clearDisplay();
  display.display();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.print("KDS INI: ");
  display.print( kline.kds_kline_ini() );
  display.setCursor(0,0);
  display.display();

  delay(1000);
  display.clearDisplay();
 
  

}



void loop()
{
 
  display.setTextSize(1);
  display.setCursor(0,0);
  display.clearDisplay();

  display.print("TPS: ");
  display.println( kline.kds_read_tps() );
  delay(30);
 
  display.print("RPM: ");
  display.println(kline.kds_read_rpm() );
  delay(30);

  display.print("Engine temp: ");
  display.println( kline.kds_read_engine_temp() );
  delay(30);

  display.print("Gear position: ");
  display.println( kline.kds_read_gear_position() );
  delay(30);

  display.display();
}








