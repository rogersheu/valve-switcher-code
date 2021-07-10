/*
   TimeSerial.pde, from
   https://github.com/thijse/Arduino-Code-and-Libraries/blob/master/Libraries/Time/Examples/TimeSerial/TimeSerial.pde
   and included in the Time library (Michael Margolis), with example code compiled by Paul Stoffregen.

   Modified by Roger Sheu for use with Valve Switcher made by Wolfgang Nadler for Peter DeCarlo. 
   example code illustrating Time library set through serial port messages.

   Messages consist of the letter T followed by ten digit time (as seconds since Jan 1 1970)
   you can send the text on the next line using Serial Monitor to set the clock to noon Jan 1 2013
  T1357041600

   A Processing example sketch to automatically send the messages is included in the download.
   Install Arduino IDE and Processing IDE to use.
   Use in conjunction with SyncArduinoClock.pde. Upload the code here to the Arduino, then run SyncArduinoClock.
   Press "Click to Time Sync" to start. Change print frequency as needed for application.

   To ensure synchronization across multiple computers, use Dimension 4 or similar NTP time source.
*/

#include <TimeLib.h>
#include <elapsedMillis.h> //Check to make sure it works for longer time periods (longer than 65 seconds)

#define TIME_HEADER  'T'   // Header tag for serial time sync message
#define TIME_REQUEST  7    // ASCII bell character requests a time sync message 

elapsedMillis timetoPrint; //Most frequent
elapsedMillis timetoSample; 
elapsedMillis timetoSync; //Least frequent

int nextValve = 0;


void setup()  {
  Serial.begin(9600);
  //while (!Serial) ; // Needed for Leonardo only
  //pinMode(13, OUTPUT);
  setSyncProvider(requestSync);  //set function to call when sync required
  Serial.println("Waiting for sync message");

  
  for (int i = 30; i < 49; i += 1) {
    pinMode(i, INPUT);
  }

  //actuates valves
  pinMode(20, OUTPUT);
  pinMode(19, OUTPUT);
  pinMode(18, OUTPUT);
  pinMode(17, OUTPUT);

}

void loop() {
  int secsPrintFreq = 5; //in seconds
  float sampleMins = 5; //in mins
  int minstoSync = 15; //in mins

  (unsigned long) timetoPrint;
  (unsigned long) timetoSample;
  (unsigned long) timetoSync;

  if (Serial.available()) {
    processSyncMessage();
  }


  if (timetoPrint > secsPrintFreq * 1000){
    timetoPrint -= secsPrintFreq * 1000;
    if (timeStatus() != timeNotSet) {
      digitalClockDisplay();
      Serial.println();
    }

  // Optional if LED connected to 13/GND
//  if (timeStatus() == timeSet) {
//    digitalWrite(13, HIGH); // LED on if synced
//  } else {
//    digitalWrite(13, LOW);  // LED off if needs refresh
//  }
  }

  
  if (timetoSample > sampleMins * 1000 * 60) {
    timetoSample -= sampleMins * 1000 * 60;
    valveSwitch(nextValve + 17);
    nextValve += 1;
    nextValve = nextValve % 4;
  }

  if (timetoSync > minstoSync * 1000 * 60 ) {
    timetoSync -= minstoSync * 1000 * 60;
    requestSync();
  }
}

// Valves are connected via pins 17 through 20, and need six seconds to complete actuation
void valveSwitch(int valveNum) {
  for (int i = 17; i < 20; i += 1) {
    digitalWrite(i, LOW); //I can probably run this and the HIGH in parallel since those are two different valves
  }
  delay(6000); //6 seconds to actuate
  digitalWrite(valveNum, HIGH); //set the next valve to be sampling
//  int prevValveNum = valveNum - 18;
//  if(prevValveNum == -1) {
//    prevValveNum += 4;
//  }
//  digitalClockDisplay();
//  Serial.println(" " + String(prevValveNum));
  delay(6000); //6 seconds to actuate
  digitalClockDisplay();
  Serial.println("," + String(valveNum-17)); //Only print time and valve once the valve has finished setting, just has to be consistent
}

void digitalClockDisplay() {
  // digital clock display of the time

  // now() is in seconds since Jan 1 1970
//  Serial.print(hour());
//  printDigits(minute());
//  printDigits(second());
//  Serial.print(" ");
//  Serial.print(month());
//  Serial.print("/");
//  Serial.print(day());
//  Serial.print("/");
//  Serial.print(year());
  Serial.print(hour() + printDigits(minute()) + printDigits(second()) + " " + month() + "/" + day() + "/" + year());

  //Serial.print(year() + "-" + month() + "-" + day() + " " + hour() + printDigits(minute()) + printDigits(second()));
}

String printDigits(int digits) {
  // utility function for digital clock display: prints preceding colon and leading 0
  String output = ":";

  if (digits == 0) //Account for 00 hour (midnight) or 00 minute
    output += '0';
  
  if (digits < 10)
    output += '0';
  output += String(digits);

  return output; 

}

//
//void printDigits(int digits) {
//  // utility function for digital clock display: prints preceding colon and leading 0
// 
//  Serial.print(":");
//  if (digits < 10)
//    Serial.print('0');
//  Serial.print(digits);
//
//}


void processSyncMessage() {
  unsigned long pctime;
  const unsigned long DEFAULT_TIME = 1357041600; // Jan 1 2013

  if (Serial.find(TIME_HEADER)) {
    pctime = Serial.parseInt();
    if ( pctime >= DEFAULT_TIME) { // check the integer is a valid time (greater than Jan 1 2013)
      setTime(pctime); // Sync Arduino clock to the time received on the serial port
    }
  }
}

time_t requestSync()
{
  Serial.write(TIME_REQUEST);
  return 0; // the time will be sent later in response to serial mesg
}
