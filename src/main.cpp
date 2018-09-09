#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>
#include <DHT.h> //include the temp sensor library
#include <Adafruit_Sensor.h>
#define DHTPIN 7     // what pin we're connected to
#define DHTTYPE DHT11   // DHT 11 temp&humid sensor
#include <Wire.h>
#include <RTClib.h>

char daysOfTheWeek[7][12] = {"Sunday ", "Monday ", "Tuesday ", "Wednesday ", "Thursday ", "Friday ", "Saturday "};
char monthName[12][5]= {"Jan ","Feb ","Mar ","Apr ","May ","Jun ","Jul ","Aug ","Sep ","Oct ","Nov ","Dec "};
int pinCS = 10; // Attach CS to this pin, DIN to MOSI and CLK to SCK
int numberOfHorizontalDisplays = 1;
int numberOfVerticalDisplays = 4;
int lightPin = 0;
int wait = 20; // In milliseconds
int spacer = 1;
int width = 5 + spacer; // The font width is 5 pixels
int counter = 1;
int reading;

String H = "H:";
String T = "T:";
String cels = "C";
String cent = "%";
String divider;
String slash = "/";
String space;
String tape;
String hrmin;

RTC_DS3231 rtc;
Max72xxPanel matrix = Max72xxPanel(pinCS, numberOfHorizontalDisplays, numberOfVerticalDisplays);
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  matrix.setIntensity(0.1); // Use a value between 0 and 15 for brightness
  dht.begin();
  rtc.begin();
  Serial.begin(9600);
  matrix.setRotation(1);    // The same hold for the last display
  //delay(5000);
  //if (rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
//  }
}

void displaydata(String);
void scrolldata(String);
//int rtcclocktime();

void loop() {


   reading  = analogRead(lightPin);
   int temp = dht.readTemperature();
   int hum = dht.readHumidity();
                //TEMP
        String tape = String(T) + String(temp) + String(cels);
        Serial.println(tape); // debug
        Serial.println(counter);
        Serial.println(tape.length());
        displaydata(tape);
                //HUMID
        tape = String(H) + String(hum) + String(cent);
        Serial.println(tape); // debug
        Serial.println(counter);
        Serial.println(tape.length());
        displaydata(tape);
                //TIME
        DateTime now = rtc.now();
                if (now.minute() < 10) {
                  divider = ":0";
                } else divider = ":";
                
                if (now.minute() > 60 or now.minute() < 0 or now.hour() < 0 or now.hour() > 24  ) {
                 String tape = "Time sync error!!";
                 scrolldata(tape);
                } 
                else
                {
                  tape = String(now.hour() + String(divider)) + now.minute();
                  displaydata(tape);
                }
                
        Serial.println(tape); // debug
        Serial.println(counter);
        Serial.println(tape.length());
              
              if (now.day() == 1 or now.day() == 21 or now.day() == 31) {
                 space = "st ";
              } else 
              if (now.day() == 2 or now.day() == 22) {
                 space = "nd ";
              } else 
              if (now.day() == 3 or now.day() == 23) {
                 space = "rd ";
              } else 
               space = "th ";
              
        tape = String(daysOfTheWeek[now.dayOfTheWeek()]) + String(now.day()) + String(space) + String(monthName[now.month()-1]);
        Serial.println(tape); // debug
        Serial.println(counter);
        Serial.println(tape.length());
        scrolldata(tape);               
}

void displaydata(String tape){
  //counter++;
  for ( uint8_t i = 0 ; i < width * tape.length() + matrix.width() - 1 - spacer; i++ ) {
    matrix.fillScreen(LOW);
    matrix.setIntensity(reading/30);
    uint8_t letter = tape.length();//i / width;
    int x = (matrix.width() - 1);//- i % width;
    int y = (matrix.height() - 8) / 2; // center the text vertically
    while ( x + width - spacer >= 0 && letter >= 0 ) {
      if ( letter < tape.length() ) {
        matrix.drawChar(x, y, tape[letter], HIGH, LOW, 1);
      }
      letter--;
      x -= width;
    }
    matrix.write(); // Send bitmap to display
   delay(wait);
  }
}

void scrolldata(String tape)  {

  for ( uint8_t i = 0 ; i < width * tape.length() + matrix.width() - 1 - spacer; i++ ) {

    matrix.fillScreen(LOW);
    matrix.setIntensity(reading/30);
    uint8_t letter = i / width;
    int x = (matrix.width() - 1) - i % width;
    int y = (matrix.height() - 8) / 2; // center the text vertically

    while ( x + width - spacer >= 0 && letter >= 0 ) {
      if ( letter < tape.length() ) {
        matrix.drawChar(x, y, tape[letter], HIGH, LOW, 1);
      }

      letter--;
      x -= width;
    }

    matrix.write(); // Send bitmap to display

  delay(wait);
  }
}