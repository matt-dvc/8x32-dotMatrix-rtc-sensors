#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>
#include <DHT.h> //include the temp sensor library
#include <Adafruit_Sensor.h>
#define DHTPIN 7     // what pin we're connected to
#define DHTTYPE DHT11   // DHT 11 temp&humid sensor
#include <Wire.h>
#include <RTClib.h>

RTC_DS3231 rtc;
char daysOfTheWeek[7][12] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

int pinCS = 10; // Attach CS to this pin, DIN to MOSI and CLK to SCK
int numberOfHorizontalDisplays = 1;
int numberOfVerticalDisplays = 4;
int wait = 20; // In milliseconds
int spacer = 1;
int width = 5 + spacer; // The font width is 5 pixels
int counter = 1;
Max72xxPanel matrix = Max72xxPanel(pinCS, numberOfHorizontalDisplays, numberOfVerticalDisplays);
DHT dht(DHTPIN, DHTTYPE);
String H = "H:";
String T = "T:";
String cels = "C";
String cent = "%";
String divider = ":";
String slash = "/";
String tape;
String hrmin;

void setup() {
  matrix.setIntensity(0.1); // Use a value between 0 and 15 for brightness
  dht.begin();
  Serial.begin(9600);
  matrix.setRotation(1);    // The same hold for the last display
  //delay(5000);
  //if (rtc.lostPower()) {
  //  Serial.println("RTC lost power, lets set the time!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
//  }
}

void displaydata(String);
int rtcclocktime();



void loop() {



   int temp = dht.readTemperature();
   int hum = dht.readHumidity();
   //String hrmin = hrmin;
   //counter++;

   //if (counter = 1) {
  //   counter = 2;
        String tape = String(T) + String(temp) + String(cels);
        Serial.println(tape); // debug
        Serial.println(counter);
        Serial.println(tape.length());
        displaydata(tape);

  //  } else if (counter = 2){
     tape = String(H) + String(hum) + String(cent);
        Serial.println(tape); // debug
        Serial.println(counter);
        Serial.println(tape.length());
        displaydata(tape);
  //  } else if (counter = 3){
        DateTime now = rtc.now();
     tape = String(now.hour() + String(divider)) + now.minute();
        Serial.println(tape); // debug
        Serial.println(counter);
        Serial.println(tape.length());
        displaydata(tape);

    //    DateTime now = rtc.now();
    tape = String(daysOfTheWeek[now.dayOfTheWeek()]);
        Serial.println(tape); // debug
        Serial.println(counter);
        Serial.println(tape.length());
        displaydata(tape);
  //  } else if (counter > 3 ) counter = 1;
    tape = String(now.day() + slash + now.month());
        Serial.println(tape); // debug
        Serial.println(counter);
        Serial.println(tape.length());
        displaydata(tape);



//rtcclocktime();

//delay(1000);

}

void displaydata(String tape){
  //counter++;
  for ( int i = 0 ; i < width * tape.length() + matrix.width() - 1 - spacer; i++ ) {
    matrix.fillScreen(LOW);
    int letter = tape.length();//i / width;
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

/*rtcclocktime(){
  DateTime now = rtc.now();
//---------------------------
String hrmin =  String(now.hour() + divider + now.minute());
Serial.print(String(hrmin));

//-----------------------------------
*/
