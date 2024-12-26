#include <dht.h>                     // Load the DHT module library
#include <LiquidCrystal_I2C.h>       // Load the LCD with I2C library
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Initialize LCD on address 2x27 with a 16 x 2 display

#include <Stepper.h>                 // Load the Arduino Stepper Library
const int stepsPerRevolution = 2038; // Define the number of steps per rotation
Stepper myStepper = Stepper(stepsPerRevolution, 8, 10, 9, 11); // Create an instance of stepper class

#define dht_pin A0                   // Analog Pin A0 of Arduino is connected to DHT11 out pin
dht DHT;                             // Set alias
byte myTemp;                         // Variable used to store current temperature in Celsius
byte myHumid;                        // Variable used to store current humidity
byte minHumid;                       // Variable used to store minimum humidity
byte maxHumid;                       // Variable used to store maximum humidity
byte minTemp;                        // Variable used to store minimum temperature
byte maxTemp;                        // Variable used to store maximum temperature
byte converted;                      // Variable used to store converted temperature from Celsius to Fahrenheit
byte CminTemp;                       // Variable used to store converted minimum temperature
byte CmaxTemp;                       // Variable used to store converted maximum temperature
int ledStatus = 13;                  // Initiate pin 13 as ledStatus

#define relay_pin 7                  // Pin 7 of Arduino is connected to relay out pin

void setup() {
  pinMode(ledStatus, OUTPUT);     // Set pin mode for status LED
  digitalWrite(ledStatus, HIGH);  // Turn on status LED
  lcd.init();                     // Initialize I2C LCD display
  lcd.backlight();                // Turn on LCD back light
  lcd.clear();                    // Clear the LCD screen

  /* The following section is optional
  ====================================================================================*/
  lcd.setCursor(0, 0);               // Set LCD cursor to 1st position on first line
  lcd.print(F("TEMP & HUMIDITY "));  // Send text to LCD
  lcd.setCursor(0, 1);               // Set LCD cursor to 1st position on 2nd line
  lcd.print(F("Initializing    "));  // Send text to LCD
  delay(500);                        // Set delay
  lcd.setCursor(0, 1);               // Set LCD cursor to 1st position on 2nd line
  lcd.print(F("Initializing.   "));  // Send text to LCD
  delay(500);                        // Set delay
  lcd.setCursor(0, 1);               // Set LCD cursor to 1st position on 2nd line
  lcd.print(F("Initializing..  "));  // Send text to LCD
  delay(500);                        // Set delay
  lcd.setCursor(0, 1);               // Set LCD cursor to 1st position on 2nd line
  lcd.print(F("Initializing... "));  // Send text to LCD
  delay(500);                        // Set delay
  lcd.setCursor(0, 1);               // Set LCD cursor to 1st position on 2nd line
  lcd.print(F("Initializing...."));  // Send text to LCD
  /*====================================================================================
  End of optional section */

  DHT.read11(dht_pin);              // Read data from dht_pin (A0)
  delay(500);                       // Set delay
  minHumid = (DHT.humidity);        // Set variable to initial minimum humidity reading
  maxHumid = (DHT.humidity);        // Set variable to initial maximum humidity reading
  minTemp = (DHT.temperature);      // Set variable to initial minimum temperature reading
  maxTemp = (DHT.temperature);      // Set variable to initial minimum temperature reading
  digitalWrite(ledStatus, LOW);     // Turn off status LED
  lcd.clear();                      // Clear the LCD screen

  pinMode(relay_pin, OUTPUT);
  digitalWrite(relay_pin, LOW);     // Turn off the motor
}

void loop() {
  DHT.read11(dht_pin);              // Take new reading from module
  myHumid = (DHT.humidity);         // Set variable to current humidity reading
  myTemp = (DHT.temperature);       // Set variable to current temperature reading

  if (myHumid < minHumid) { minHumid = (DHT.humidity); }  // If current humidity is < minimum, set minimum to current
  if (myHumid > maxHumid) { maxHumid = (DHT.humidity); }  // If current humidity is > maximum, set maximum to current
  if (myTemp < minTemp) { minTemp = myTemp; }     // If current temperature is < minimum, set maximum to current
  if (myTemp > maxTemp) { maxTemp = myTemp; }     // If current temperature is > maximum, set maximum to current

  HumidStat();  // Run HumidStat function
  delay(5000);  // wait for delay
  TempStat();   // Run TempStat function
  delay(5000);  // wait for delay

  // Control the motor according to temperature
  if (myTemp > 30) {
      digitalWrite(relay_pin, HIGH); // Turn on the motor
     // Rotate CCW at 10 RPM
	    myStepper.setSpeed(10);
	    myStepper.step(-stepsPerRevolution);
  } else {
      digitalWrite(relay_pin, LOW);  // Turn off the motor
  }
}

void TempStat() {
  digitalWrite(ledStatus, HIGH);  // Turn on status LED
  lcd.setCursor(0, 0);            // Set LCD cursor position
  lcd.print(F("TEMP   min: "));   // Display message on LCD
  lcd.setCursor(12, 0);           // Set LCD cursor position
  lcd.print(minTemp);            // Display minimum temperature on LCD
  lcd.print(F(" C"));             // Display message on LCD
  lcd.setCursor(0, 1);            // Set LCD cursor position
  lcd.print(myTemp);           // Display current temperature on LCD
  lcd.print(F(" C   "));          // Display message on LCD
  lcd.setCursor(7, 1);            // Set LCD cursor position
  lcd.print(F("max: "));          // Display message on LCD
  lcd.setCursor(12, 1);           // Set LCD cursor position
  lcd.print(maxTemp);            // Display maximum temperature on LCD
  lcd.print(F(" C"));             // Display message on LCD
  digitalWrite(ledStatus, LOW);   // Turn off status LED
}

void HumidStat() {
  digitalWrite(ledStatus, HIGH);  // Turn on status LED
  lcd.setCursor(0, 0);            // Set LCD cursor position
  lcd.print(F("HUMID  min: "));   // Display message on LCD
  lcd.setCursor(12, 0);           // Set LCD cursor position
  lcd.print(minHumid);            // Display minimum humidity on LCD
  lcd.print(F(" %"));             // Display message on LCD
  lcd.setCursor(0, 1);            // Set LCD cursor position
  lcd.print(myHumid);             // Display current humidity on LCD
  lcd.print(F(" %  "));           // Display message on LCD
  lcd.setCursor(7, 1);            // Set LCD cursor position
  lcd.print(F("max: "));          // Display message on LCD
  lcd.setCursor(12, 1);           // Set LCD cursor position
  lcd.print(maxHumid);            // Display maximum humidity on LCD
  lcd.print(F(" %"));             // Display message on LCD
  digitalWrite(ledStatus, LOW);   // Turn off status LED
}
