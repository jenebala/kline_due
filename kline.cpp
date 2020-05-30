#include "kline.h"


k_line_prop::k_line_prop(HardwareSerial* serial,long baudRate, uint8_t txpin, uint16_t timeout)
{   
    _txpin=txpin;
    _baud=baudRate;
    _timeout=timeout;
    
    kline_rxtx = serial;
}

uint8_t k_line_prop::kds_kline_ini()
{
    uint8_t ini_response[] = {0x80, 0xF1, 0x11, 0xC1, 0xEA, 0x8F, 0xBF};
     // SEND : 0x81, 0x11, 0xF1, 0x81, 0x4

    //kline_rxtx->end();

    pinMode(_txpin,OUTPUT);
   
    // Kline Fast INI 
    digitalWrite(_txpin,HIGH);
    delay(1000);
    digitalWrite(_txpin,LOW);
    delay(25);
    digitalWrite(_txpin,HIGH);
    delay(25);

    kline_rxtx->begin(_baud);
    kline_rxtx->setTimeout(_timeout);

    //KDS INI
    kline_rxtx->write(0x81);
    kline_rxtx->write(0x11);
    kline_rxtx->write(0xF1);
    kline_rxtx->write(0x81);
    kline_rxtx->write(0x4);
    
    uint8_t readbyte=13;

    #ifdef DEBUG
      time_0=millis();
    #endif
  
    kline_rxtx->readBytes(buffer,readbyte);
    
    

    #ifdef DEBUG
        time_1=millis();
        Serial.print("K-line INI: ");
        printHex(buffer,readbyte);
        Serial.println();
        Serial.print("Time: ");
        Serial.print(time_1-time_0);
        Serial.println(" ms");
        

    #endif
   
    
    for (uint8_t i = 0; i < 1; i++)
    {
      if ( buffer[i+5] != ini_response[i] ) return 0;
    }
    return 1;
    
}



uint16_t k_line_prop::kds_read_tps()
{   //KDS Read_TPS
    uint16_t max_tps=405;
    uint16_t min_tps=201;

    kline_rxtx->write(0x80);
    kline_rxtx->write(0x11);
    kline_rxtx->write(0xF1);
    kline_rxtx->write(0x2);
    kline_rxtx->write(0x21);
    kline_rxtx->write(0x4);
    kline_rxtx->write(0xA9);

    uint8_t readbyte=16;
    
    #ifdef DEBUG
      time_0=millis();
    #endif


    kline_rxtx->readBytes(buffer,readbyte);
    
    uint16_t tps = 0;
    
    tps=tps+(buffer[13]<<8);
    tps=tps+buffer[14];

    

    

    #ifdef DEBUG
      time_1=millis();
      Serial.print("K-line TPS: ");
      printHex(buffer,readbyte);
      Serial.println();
      Serial.print("Time: ");
      Serial.print(time_1-time_0);
      Serial.println(" ms");
      Serial.print("TPS RAW:");
      Serial.println(tps);
    #endif

  return ((tps-min_tps)*100)/(max_tps-min_tps);
}



uint16_t k_line_prop::kds_read_rpm()
{   //KDS Read_RPM
    kline_rxtx->write(0x80);
    kline_rxtx->write(0x11);
    kline_rxtx->write(0xF1);
    kline_rxtx->write(0x2);
    kline_rxtx->write(0x21);
    kline_rxtx->write(0x9);
    kline_rxtx->write(0xAE);

    uint8_t readbyte=16;
    
    #ifdef DEBUG
      time_0=millis();
    #endif


    kline_rxtx->readBytes(buffer,readbyte);
    uint16_t rpm_a = 0;
    uint8_t rpm_b = 0;
    
    rpm_a= (buffer[13]);
    rpm_a=rpm_a*100;
    rpm_b=buffer[14];
    
    #ifdef DEBUG
      time_1=millis();
      Serial.print("K-line RPM: ");
      printHex(buffer,readbyte);
      Serial.println();
      Serial.print("Time: ");
      Serial.print(time_1-time_0);
      Serial.println(" ms");
      Serial.print("rpm_a: ");
      Serial.println(rpm_a);
      Serial.print("rpm_b: ");
      Serial.println(rpm_b);
    
    #endif

   

  return rpm_a+rpm_b;
}

int16_t k_line_prop::kds_read_engine_temp()
{   // Engine temp read
    kline_rxtx->write(0x80);
    kline_rxtx->write(0x11);
    kline_rxtx->write(0xF1);
    kline_rxtx->write(0x2);
    kline_rxtx->write(0x21);
    kline_rxtx->write(0x06);
    kline_rxtx->write(0xAB);

    uint8_t readbyte=15;
    
    #ifdef DEBUG
      time_0=millis();
    #endif


    kline_rxtx->readBytes(buffer,readbyte);
    
    int16_t engine_temp = 0;
    
    engine_temp = (buffer[13]-48)/1,6;


    #ifdef DEBUG
      time_1=millis();
      Serial.print("K-line Engine temp: ");
      printHex(buffer,readbyte);
      Serial.println();
      Serial.print("Time: ");
      Serial.print(time_1-time_0);
      Serial.println(" ms");
      Serial.print("Engine temp RAW: ");
      Serial.println(engine_temp);
    #endif

   

  return engine_temp;


}


uint8_t k_line_prop::kds_read_gear_position()
{
    // Gear position
    kline_rxtx->write(0x80);
    kline_rxtx->write(0x11);
    kline_rxtx->write(0xF1);
    kline_rxtx->write(0x2);
    kline_rxtx->write(0x21);
    kline_rxtx->write(0x0B);
    kline_rxtx->write(0xB0);

    uint8_t readbyte=15;
    
    #ifdef DEBUG
      time_0=millis();
    #endif


    kline_rxtx->readBytes(buffer,readbyte);
    
    int16_t gear_position = 0;
    
    gear_position = buffer[13];


    #ifdef DEBUG
      time_1=millis();
      Serial.print("K-line Gear position: ");
      printHex(buffer,readbyte);
      Serial.println();
      Serial.print("Time: ");
      Serial.print(time_1-time_0);
      Serial.println(" ms");
      Serial.print("Gear position RAW: ");
      Serial.println(gear_position);
    #endif

   

  return gear_position;
}




void k_line_prop::printHex(uint8_t *data, uint8_t length)
{  
 
  for (int i=0; i<length; i++)
  {
      Serial.print("x");
      Serial.print(data[i],HEX);
      Serial.print(" ");
  }

}