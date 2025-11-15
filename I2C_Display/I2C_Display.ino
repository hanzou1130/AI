// Arduino UNO R4 MINIMA - 20x4 I2C LCD Display Demo
// Display a counter on SunFounder 20x4 I2C LCD
// Increment counter every second and show on 4th line

#include <Arduino.h>
#include "I2C_Display.h"

// Constant definitions
static const uint8_t LED_PIN        = LED_BUILTIN;                              // LED output pin
static const uint16_t DELAY_TIME_MS = 50U;                                      // Delay time (milliseconds)
static const uint8_t LCD_I2C_ADDR   = 0x27;                                     // LCD I2C address (commonly 0x27 or 0x3F)
static const uint8_t LCD_COLS       = 20U;                                      // LCD column count
static const uint8_t LCD_ROWS       = 4U;                                       // LCD row count

// Global object
LiquidCrystal_I2C lcd ( LCD_I2C_ADDR, LCD_COLS, LCD_ROWS );                     // LCD object

// Global variable
static uint32_t loopCounter         = 0U;                                       // Loop counter

// Implementation section
void setup ( void )
{
  pinMode       ( LED_PIN, OUTPUT );                                            // Set LED pin to output mode

  // LCD initialization
  lcd.init      ();                                                             // Initialize LCD (UNO R4 compatible)
  lcd.backlight ();                                                             // Turn backlight ON
  lcd.clear     ();                                                             // Clear display

  // Boot messages
  lcd.setCursor ( 0, 0 );                                                       // Line 1
  lcd.print     ( "Arduino UNO R4" );
  lcd.setCursor ( 0, 1 );                                                       // Line 2
  lcd.print     ( "LED Blink 100ms" );
  lcd.setCursor ( 0, 2 );                                                       // Line 3
  lcd.print     ( "20x4 I2C LCD" );
  lcd.setCursor ( 0, 3 );                                                       // Line 4
  lcd.print     ( "Status: Running" );
}

void loop ( void )
{
  // Increment loop counter
  loopCounter++;

  // Update LCD (show counter on 4th line)
  lcd.setCursor ( 0, 3 );                                                       // Line 4
  lcd.print     ( "Count: " );
  lcd.print     ( loopCounter );
  lcd.print     ( "        " );                                                 // Clear leftover characters

  // Wait
  delay         ( 1000 );                                                       // Wait 1 second
}
