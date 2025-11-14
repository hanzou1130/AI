// I2C Scanner for Arduino UNO R4 MINIMA
// I2Cバスに接続されているデバイスのアドレスをスキャン

#include <Wire.h>

void setup ( void )
{
  Wire.begin        ();                                                         // I2C初期化
  Serial.begin      ( 9600 );                                                   // シリアル通信開始
  while ( !Serial );                                                            // シリアル接続待機
  Serial.println    ( "I2C Scanner" );
}

void loop ( void )
{
  byte error, address;
  int nDevices;
  
  Serial.println    ( "Scanning..." );
  
  nDevices          = 0;
  for ( address = 1; address < 127; address++ )
  {
    Wire.beginTransmission ( address );
    error           = Wire.endTransmission ();
    
    if ( error == 0 )
    {
      Serial.print  ( "I2C device found at address 0x" );
      if ( address < 16 )
        Serial.print ( "0" );
      Serial.print  ( address, HEX );
      Serial.println ( " !" );
      nDevices++;
    }
    else if ( error == 4 )
    {
      Serial.print  ( "Unknown error at address 0x" );
      if ( address < 16 )
        Serial.print ( "0" );
      Serial.println ( address, HEX );
    }
  }
  
  if ( nDevices == 0 )
    Serial.println  ( "No I2C devices found\n" );
  else
    Serial.println  ( "done\n" );
  
  delay             ( 5000 );                                                   // 5秒待機
}
