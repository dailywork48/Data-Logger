#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DHT.h"
#include "RTClib.h"
const int chipSelect = 10; //cs or the save select pin from the sd shield is connected to 10.
RTC_DS1307 RTC;

#define DHTPIN 2    // modify to the pin we connected
#define DHTPIN1 3    // modify to the pin we connected
 
#define DHTTYPE DHT21   // AM2301
#define DHTTYPE1 DHT21   // AM2301

#define OLED_RESET     4
Adafruit_SSD1306 display(OLED_RESET);
 
DHT dht(DHTPIN, DHTTYPE);
DHT dht1(DHTPIN1, DHTTYPE);

File dataFile;
DateTime now;

void setup(void) {
  Serial.begin(9600);
//setup clock
  Wire.begin();
  RTC.begin();
   dht.begin();
  dht1.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

//check or the Real Time Clock is on
  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    // uncomment it & upload to set the time, date and start run the RTC!
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
//setup SD card
   Serial.print("Check SD card");

  // see if the SD card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed");
    // don't do anything more:
    return;
  }
  Serial.println("Ready!");
  
  //write down the date (year / month / day         prints only the start, so if the logger runs for sevenal days you only findt the start back at the begin.
    now = RTC.now();
    dataFile = SD.open("datalog.txt", FILE_WRITE);
    dataFile.print("Start logging on: ");
    dataFile.print(now.year(),DEC);
    dataFile.print('/');
    dataFile.print(now.month(),DEC);
    dataFile.print('/');
    dataFile.print(now.day(),DEC);
    dataFile.println(" ");
    dataFile.println("In Temp , In RH , Out Temp , Out RH , Time");
    dataFile.close();
}

  void displayTempHumid() {
  delay(2000); // Pause for 2 seconds

  float h1 = dht.readHumidity();
  float t1 = dht.readTemperature();
  float h2 = dht1.readHumidity();
  float t2 = dht1.readTemperature();

  // Clear the buffer
  display.clearDisplay();

  //Display style
  display.setTextColor(WHITE); //font color
  display.setTextSize(1); //font size
  
  //Tite 
  display.setCursor(0,0); //coordinates
  display.print("DATA LOGGER"); //text to dispaly

  //set indoor humidity cursor coordinates
  display.setCursor(0,10); //coordinates
  display.print("Indoor Humidity: "); //text to display
  display.print(h1); //value of indoor humidity
  display.print(" %");  //text to display after value

    //set indoor temperature cursor coordinates
  display.setCursor(0,20); //coordinates
  display.print("Indoor Temperature: "); //text to display
  display.print(t1); //value of indoor humidity
  display.print(" C");  //text to display after value

      //set outdoor humidity cursor coordinates
  display.setCursor(0,30); //coordinates
  display.print("Outdoor Humidity: "); //text to display
  display.print(h2); //value of indoor humidity
  display.print(" %");  //text to display after value

      //set outdoor temperature cursor coordinates
  display.setCursor(0,40); //coordinates
  display.print("Outdoor Temperature: "); //text to display
  display.print(t2); //value of indoor humidity
  display.print(" C");  //text to display after value
}

void loop(void) {
  
//read the time
  now = RTC.now();

 float h1 = dht.readHumidity();
 float t1 = dht.readTemperature();
 float h2 = dht1.readHumidity();
 float t2 = dht1.readTemperature();
  
  //open file to log data in.
   dataFile = SD.open("datalog.txt", FILE_WRITE);

  // if the file is available, write to it:
  //time.
  if (dataFile) {

    dataFile.print(t1);
    dataFile.print(",");
    dataFile.print(h1);
    dataFile.print(",");
    dataFile.print(t2);
    dataFile.print(",");
    dataFile.print(h2);
    dataFile.print(",");
    dataFile.print("");
    
    dataFile.print(now.hour(),DEC);
    dataFile.print(":");
    dataFile.print(now.minute(),DEC);
    dataFile.print(":");
    dataFile.println(now.second(),DEC);
   
    dataFile.close();
    // print to the serial port too:
    Serial.println("data stored");
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
  //delay(60000); // this will log the temperature every minute.
  delay(600000);

  displayTempHumid();
  display.display();
}