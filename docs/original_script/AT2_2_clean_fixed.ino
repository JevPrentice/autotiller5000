#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <EEPROM.h>
#include <Servo.h>

// --- Forward declarations to satisfy the compiler ---
void LEDOFF();
bool saveCalibToEEPROM();
void handleCalibrationFromEEPROM(uint16_t samplerate_delay_ms);


bool calibSaved = false;  // auto-saved once per boot

  Servo myservo;  // create servo object to control a servo

#define LED1 10
#define LED2 11
#define LED3 12


/* ======================================Needs to be done==========================================

 

/* This driver uses the Adafruit unified sensor library (Adafruit_Sensor),
   which provides a common 'type' for sensor data and some helper functions.

   To use this driver you will also need to download the Adafruit_Sensor
   library and include it in your libraries folder.

   You should also assign a unique ID to this sensor for use with
   the Adafruit Sensor API so that you can identify this particular
   sensor in any data logs, etc.  To assign a unique ID, simply
   provide an appropriate value in the constructor below (12345
   is used by default in this example).

   ------------------------------------Connections-------------------------------------------
   ===========
   Connect SCL to analog 5
   Connect SDA to analog 4
   Connect VDD to 3-5V DC
   Connect GROUND to common ground
   
   mainButtonpin 2
   Port pin 4
   Starboard pin 5

   Buzzer pin - 8
   LED pin - 10

   Servo Pin - 9 (buzzer interferes with pin 3 and 11!)

   LED pin1 - 10 remember to use resistors and short leg to ground.
   LED pin2 - 11
   LED pin3 - 12







   -----------------------------------Libraries used:---------------------------------------

   Adafruit Unified Sensor by Adafruit
   Servo by Michael Margolis, Arduino
   Adafruit BNO055 by Adafruit
    Perhaps also:
    Adafruit Bus IO



   */

   //===============================VARIABLES=====================================
   int mainButtonpin = 2;
   int servopin = 9;  // Servo cconnected to pin9 Red to positive; brown negative; Yellow to pin9 (digital)
   //Buzzer interferes with pin 3 and 11!! 
   int portbuttonpin = 4;  //  decrease course by amountcourseadj degrees - turn to port
   int starbuttonpin = 5;  // Increase course by amountcourseadj degrees - turn to starboard
   int buzpin = 8;  // Output pin for Buzzer
   float course;
   float heading;
   int waittime = 300;
   float headadj;
   float servoadjust;
   float servoposition = 90;
   int delaypush = 300;
   int portpush;
   int starpush;
   long pastpress;
   int portrelease;
   long newpress;
   int pressedtime;
   int starrelease;
   int amountcourseadj = 2;
   int counter;// To be used as a counter in for-loops
   int oldcourse;
   int release;
   int elapsedtime;
   bool pushed;
   int eepromaddr = 250;
  
   //======================================VARIABLES THAT CAN BE CHANGED============================================
   int tackangle = 90; // ANGLE THAT THE YACHT NEEDS TO TACK.
   int sensitivity = EEPROM.read(eepromaddr+1); // change between 1 to 100 --- 100 being very sensitive
   int8_t reverse = (int8_t)EEPROM.read(eepromaddr);// make it -1 to reverse servo action
   int shortpress = 3000;


   

/* Set the delay between fresh samples */
#define BNO055_SAMPLERATE_DELAY_MS (100)

// Check I2C device address and correct line below (by default address is 0x29 or 0x28)
//                                   id, address
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);

/**************************************************************************/
/*
    Displays some basic information on this sensor from the unified
    sensor API sensor_t type (see Adafruit_Sensor for more information)
    */
/**************************************************************************/
void displaySensorDetails(void)
{
    sensor_t sensor;
    bno.getSensor(&sensor);
    Serial.println("------------------------------------");
    Serial.print("Sensor:       "); Serial.println(sensor.name);
    Serial.print("Driver Ver:   "); Serial.println(sensor.version);
    Serial.print("Unique ID:    "); Serial.println(sensor.sensor_id);
    Serial.print("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" xxx");
    Serial.print("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" xxx");
    Serial.print("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" xxx");
    Serial.println("------------------------------------");
    Serial.println("");
    delay(500);
}

/**************************************************************************/
/*
    Display some basic info about the sensor status
    */
/**************************************************************************/
void displaySensorStatus(void)
{
    /* Get the system status values (mostly for debugging purposes) */
    uint8_t system_status, self_test_results, system_error;
    system_status = self_test_results = system_error = 0;
    bno.getSystemStatus(&system_status, &self_test_results, &system_error);

    /* Display the results in the Serial Monitor */
    Serial.println("");
    Serial.print("System Status: 0x");
    Serial.println(system_status, HEX);
    Serial.print("Self Test:     0x");
    Serial.println(self_test_results, HEX);
    Serial.print("System Error:  0x");
    Serial.println(system_error, HEX);
    Serial.println("");
    digitalWrite (LED3,HIGH);
    delay(5000);
    LEDOFF();
}

/**************************************************************************/
/*
    Display sensor calibration status
    */
/**************************************************************************/
void displayCalStatus(void)
{
    /* Get the four calibration values (0..3) */
    /* Any sensor data reporting 0 should be ignored, */
    /* 3 means 'fully calibrated" */
    uint8_t system, gyro, accel, mag;
    system = gyro = accel = mag = 0;
    bno.getCalibration(&system, &gyro, &accel, &mag);

    /* The data should be ignored until the system calibration is > 0 */
    Serial.print("\t");
    if (!system)
    {
        Serial.print("! ");
    }

    /* Display the individual values */
    //LEDOFF();
    Serial.print("Sys:");
    Serial.print(system, DEC);
    if (system>=1){
      digitalWrite(LED1,HIGH);
      delay(10);
    }
    if (system >=2){
      digitalWrite(LED2,HIGH);
      delay(10);
    }
    if (system >=3){
      digitalWrite(LED3,HIGH);
      delay(10);
    }
    Serial.print(" G:");
    Serial.print(gyro, DEC);
    Serial.print(" A:");
    Serial.print(accel, DEC);
    Serial.print(" M:");
    Serial.print(mag, DEC);
    //LEDOFF();
}

/**************************************************************************/
/*
    Display the raw calibration offset and radius data
    */
/**************************************************************************/
void displaySensorOffsets(const adafruit_bno055_offsets_t &calibData)
{
    Serial.print("Accelerometer: ");
    Serial.print(calibData.accel_offset_x); Serial.print(" ");
    Serial.print(calibData.accel_offset_y); Serial.print(" ");
    Serial.print(calibData.accel_offset_z); Serial.print(" ");

    Serial.print("\nGyro: ");
    Serial.print(calibData.gyro_offset_x); Serial.print(" ");
    Serial.print(calibData.gyro_offset_y); Serial.print(" ");
    Serial.print(calibData.gyro_offset_z); Serial.print(" ");

    Serial.print("\nMag: ");
    Serial.print(calibData.mag_offset_x); Serial.print(" ");
    Serial.print(calibData.mag_offset_y); Serial.print(" ");
    Serial.print(calibData.mag_offset_z); Serial.print(" ");

    Serial.print("\nAccel Radius: ");
    Serial.print(calibData.accel_radius);

    Serial.print("\nMag Radius: ");
    Serial.print(calibData.mag_radius);
}


/**************************************************************************/
/*
    Arduino setup function (automatically called at startup)
    */
/**************************************************************************/
void setup(void)
{
    Serial.begin(115200);
    delay(1000);
    Serial.println("Orientation Sensor Test"); Serial.println("");
    pinMode(mainButtonpin, INPUT_PULLUP);
    pinMode (portbuttonpin, INPUT_PULLUP);
    pinMode (starbuttonpin, INPUT_PULLUP);
    pinMode (buzpin,OUTPUT);
    myservo.attach(servopin); 
    pinMode(LED1,OUTPUT);
    pinMode(LED2,OUTPUT);
    pinMode(LED3,OUTPUT);
    LEDOFF();


    /* ==================================Initialise the sensor===================================== */
    if (!bno.begin())
    {
        /* There was a problem detecting the BNO055 ... check your connections */
        Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
        while (1);
    }

    int eeAddress = 0;
    long bnoID;
    bool foundCalib = false;

    EEPROM.get(eeAddress, bnoID);

    adafruit_bno055_offsets_t calibrationData;
    sensor_t sensor;

    /*
    *  Look for the sensor's unique ID at the beginning oF EEPROM.
    *  This isn't foolproof, but it's better than nothing.
    */
    bno.getSensor(&sensor);
    if (bnoID != sensor.sensor_id)
    {
        Serial.println("\nNo Calibration Data for this sensor exists in EEPROM");
      //  delay(500);
    }
    else
    {
        Serial.println("\nFound Calibration for this sensor in EEPROM.");
        eeAddress += sizeof(long);
        EEPROM.get(eeAddress, calibrationData);

        displaySensorOffsets(calibrationData);

        Serial.println("\n\nRestoring Calibration data to the BNO055...");
        bno.setSensorOffsets(calibrationData);

        Serial.println("\n\nCalibration data loaded into BNO055");
        foundCalib = true;
    }

    //delay(1000);

    /* Display some basic information on this sensor */
    displaySensorDetails();

    /* Optional: Display current status */
    displaySensorStatus();

   /* Crystal must be configured AFTER loading calibration data into BNO055. */
    bno.setExtCrystalUse(true);

    sensors_event_t event;
    bno.getEvent(&event);
    /* always recal the mag as It goes out of calibration very often */
    handleCalibrationFromEEPROM(BNO055_SAMPLERATE_DELAY_MS);
    }


//=========================================Calculate the amount of heading adjustment needed======================
void calheadingadjust(){
  headadj = course - heading;
  if (headadj <-180){
    headadj=headadj+360;
  }
  if (headadj > 180){
    headadj = headadj-360;
  }

}


void loop() {
    /* Get a new sensor event */
    sensors_event_t event;
    bno.getEvent(&event);
  // --- Auto-save calibration once when fully calibrated ---
  if (bno.isFullyCalibrated() && !calibSaved) {
    if (saveCalibToEEPROM()) {
      Serial.println(F("Calibration saved to EEPROM."));
      calibSaved = true;
    }
  }

    digitalWrite(LED1,HIGH);

    //=========================Check if mainButton has been pressed================================
     pushed = digitalRead(mainButtonpin);
      if(pushed == LOW){
        LEDOFF();
        digitalWrite(LED2,HIGH);
        servoposition = 90;
        delay(waittime);
        course = event.orientation.x;
         while (pushed == LOW) {
          noTone(buzpin);
          pushed = LOW;                    // Loop is kept perpetual by forcing pushed as LOW (pressed)
          sensors_event_t event;
          bno.getEvent(&event);
          heading = event.orientation.x;
          calheadingadjust();  //jump to subroutine to get variable headadj -- the amount of degrees that the heading needs to adjust with (negative to port)
          altercourse();
          checkbuttonpressed();
          Serial.print(heading,0);
          Serial.print("       ");
          Serial.print(course,0);
          Serial.print("        ");
          Serial.print(headadj,0);
          Serial.print("         ");
          Serial.print(servoposition);
          Serial.print("         ");
          Serial.println( sensitivity/10* sin(0.01746032*headadj));
        

          int release = digitalRead(mainButtonpin);
          if (release == LOW){             // if main button is pressed - it exits the loop.
            checktime();  // Checktime at 535  - In order to exit course steering/ Adjust sensitivity / reverse steering
           
          }
         }
         
      }
    /* Display the floating point data */
    Serial.print("X: ");
    Serial.print(event.orientation.x, 0);
    Serial.print("\tY: ");
    Serial.print(event.orientation.y, 0);
    Serial.print("\tZ: ");
    Serial.print(event.orientation.z, 0);

    myservo.write(90);

    /* Optional: Display calibration status */
    displayCalStatus();

    /* Optional: Display sensor status (debug only) */
    //displaySensorStatus();

    /* New line for the next sample */
    Serial.println("");

    /* Wait the specified delay before requesting new data */
    delay(BNO055_SAMPLERATE_DELAY_MS);
}


//---------------------------------------------SERVO ADJUSTMENT------------------------------------------------


void altercourse(){
servoposition = servoposition + sensitivity/10* sin(0.01746032*headadj)*reverse;
delay(100);
if (servoposition <0){
  servoposition = 0;
}
if (servoposition >180){
  servoposition = 180;
}
myservo.write(servoposition);
}
//-------------------------------------------------CHECK IF PORT OR STARBOARD BUTTON HAS BEEN PRESSED--------------------------------------
void checkbuttonpressed(){
portpush = digitalRead (portbuttonpin);
starpush = digitalRead(starbuttonpin);
if(portpush == LOW){
 tone(buzpin,200);
 portadj(); 
 
}
if(starpush == LOW){
  tone(buzpin,400);
  staradj();
  
}
}

//-----------------------------------------------//PORT ADJUST - BUTTON PRESSED TO ADJUST OR TACK TO PORT------------------------------------
void portadj(){
  
  pastpress = millis();
  while (portpush == LOW){
    portrelease = digitalRead (portbuttonpin);
    if (portrelease == HIGH){
      noTone(buzpin);
      newpress = millis();
      portpush = HIGH;
      
    }
  }
  pressedtime = newpress - pastpress;
  if (pressedtime <shortpress){
    course = course - amountcourseadj;
    delay(waittime);
  }
  if (pressedtime > shortpress){
    oldcourse = course;
  course = course -tackangle;
  buzz();
 // delay(waittime);
  }
  coursenormalize();
}
//----------------------------------------------BUTTON PRESSED FOR STARBOARD TACK / ADJUST--------------------------------
void staradj(){
  pastpress = millis();
  while (starpush == LOW){
    starrelease = digitalRead (starbuttonpin);
    if (starrelease == HIGH){
      newpress = millis();
      noTone(buzpin);
      starpush = HIGH;
      
    }
  }
  pressedtime = newpress - pastpress;
  if (pressedtime <shortpress){
    course = course + amountcourseadj;
    delay(waittime);
  }
  if (pressedtime > shortpress){
    oldcourse = course;
  course = course +tackangle;
  buzz();
 // delay(waittime);
 // noTone(buzpin);
  }
  coursenormalize();
}
//-------------------------------------------COURSE NORMALIZATION (SET IT BETWEEN 0 AND 360 DEGREES)
void coursenormalize(){
  if (course <0){
    course = course + 360;
  }
  if (course >360){
    course = course - 360;
  }
}

//-------------------------------------------BUZZER TO WARN OF IMMINENT TACK--------------------------------------

void buzz(){
  for(counter = 1; counter <=10; counter++){
  tone(buzpin,600);
  delay(50);
//------------prevent tack-------------------
  int pushed = digitalRead(mainButtonpin);  // if main button is pressed - it will cancell the new course (course = oldcourse)  (it won't tack!)
      if(pushed == LOW){
        delay(waittime);
         course = oldcourse;
         counter = 10;
         noTone(buzpin);
      }

  //noTone(buzpin);
  delay (waittime);
  }
}

//---------------------------------------------Toggle in Main Menu-----------------------------------------
// between exiting Course / Adjust sensitivity to steering / reverse steering
void checktime(){
  LEDOFF();
  delay(1000);
  pushed = LOW;
  pastpress=millis();
    while (pushed == LOW){
      Serial.println(pastpress);
      if (millis()-pastpress<2000){
        digitalWrite(LED1,HIGH);
        delay(10);
        }
        if (millis()-pastpress>2000 and millis()-pastpress<4000){
          digitalWrite(LED1,HIGH);
          digitalWrite(LED2,HIGH);
          delay(10);
        }
        if (millis()-pastpress>4000){
          digitalWrite(LED1,HIGH);
          digitalWrite(LED2,HIGH);
          digitalWrite(LED3,HIGH);
          delay(10);
        }
      pushed = digitalRead(mainButtonpin);
      LEDOFF();
    }
  newpress = millis();
  elapsedtime = newpress - pastpress;
  if (elapsedtime <= 2000){
    //noTone (buzpin);
    pushed = HIGH;
    LEDOFF();
  }else if(elapsedtime>2000 and elapsedtime<4000){
    //noTone (buzpin);
    setsensitive();
    release = HIGH;
    delay(30);
    LEDOFF();
  }else if (elapsedtime >= 4000){
    //noTone (buzpin);
    reverse = reverse *-1;
    Serial.print("reverse");
    Serial.print(reverse);
    EEPROM.write(eepromaddr,reverse);
    delay(30);
    release = HIGH;
    LEDOFF();
  }

LEDOFF();
}

void setsensitive(){
  LEDOFF();
  pushed = HIGH;
while (pushed==HIGH){
  Serial.println("set sensitive level");
 // Serial.println (sensitivity);
  portpush = digitalRead (portbuttonpin);
  starpush = digitalRead(starbuttonpin);
    if(portpush == LOW){
    tone(buzpin,sensitivity);
    sensitivity = sensitivity - 10;
    if (sensitivity <30){
      sensitivity = 30;
    }
    //delay(30);
    }

  if(starpush == LOW){
    tone(buzpin,sensitivity);
    sensitivity = sensitivity +10;
    if (sensitivity > 1000){
      sensitivity = 1000;
    }
    //delay(30);
  }
Serial.println(sensitivity);
delay(30);
EEPROM.write(eepromaddr+1,sensitivity);
pushed = digitalRead(mainButtonpin);
  
}
}
void LEDOFF(){
 digitalWrite(LED1,LOW);
 digitalWrite(LED2,LOW);
 digitalWrite(LED3,LOW);
}

// === BNO055 EEPROM Calibration Helpers ===
/*
 * Stores and restores Adafruit BNO055 calibration offsets to/from EEPROM on Arduino Uno.
 * Assumes there is a global 'Adafruit_BNO055 bno;' and 'sensors_event_t event;' in this sketch.
 * No EEPROM.begin()/commit() needed on AVR (Uno).
 */
#define CALIB_MAGIC 0xB055

struct CalibBlob {
  uint16_t magic;
  adafruit_bno055_offsets_t offsets;
};

static const int CALIB_EEPROM_ADDR = 0; // start of EEPROM

bool loadCalibFromEEPROM(adafruit_bno055_offsets_t &out) {
  CalibBlob blob;
  EEPROM.get(CALIB_EEPROM_ADDR, blob);
  if (blob.magic != CALIB_MAGIC) return false;
  out = blob.offsets;
  return true;
}

bool saveCalibToEEPROM() {
  if (!bno.isFullyCalibrated()) return false;
  adafruit_bno055_offsets_t ofs;
  bno.getSensorOffsets(ofs);
  CalibBlob blob;
  blob.magic = CALIB_MAGIC;
  blob.offsets = ofs;
  EEPROM.put(CALIB_EEPROM_ADDR, blob);
  return true;
}

void applyCalibToBNO(const adafruit_bno055_offsets_t &ofs) {
  bno.setSensorOffsets(ofs);
  delay(10);
}

void handleCalibrationFromEEPROM(uint16_t samplerate_delay_ms) {
  adafruit_bno055_offsets_t ofs;
  if (loadCalibFromEEPROM(ofs)) {
    Serial.println(F("Loaded BNO055 calibration from EEPROM."));
    applyCalibToBNO(ofs);
    unsigned long t0 = millis();
    const unsigned long MAX_WAIT_MS = 3000;
    while (!bno.isFullyCalibrated() && (millis() - t0 < MAX_WAIT_MS)) {
      sensors_event_t tempEvent; bno.getEvent(&tempEvent);
      delay(samplerate_delay_ms);
    }
    if (bno.isFullyCalibrated()) {
      Serial.println(F("Sensor reports fully calibrated."));
    } else {
      Serial.println(F("Calibration applied; sensor will refine with gentle motion."));
    }
  } else {
    Serial.println(F("Please Calibrate Sensor: "));
  }
}
/* === End helpers === */