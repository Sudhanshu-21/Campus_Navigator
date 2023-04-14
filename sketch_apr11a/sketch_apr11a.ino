#include <TinyGPS++.h>
#include <SoftwareSerial.h> // include the SoftwareSerial library for serial communication with the GPS module
#include <LiquidCrystal.h> // include the LiquidCrystal library for the LCD screen

TinyGPSPlus gps;

SoftwareSerial gpsSerial(0, 1); // set up a software serial port for the GPS module (RX=1, TX=0)
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // set up the LCD screen (RS=12, E=11, D4=5, D5=4, D6=3, D7=2)

// Define a struct to hold location data
struct Location {
  float lattitude;
  float longitude;
  String nameOfPlace;
};

// Define a lookup table of locations (replace with your own)
Location locations[] = {
 {21.125381, 79.046249, "OAT"},
 {21.125862, 79.045783, "HB-4"},
 {21.124568, 79.046182, "HB-3"},
 {21.125290, 79.047375, "Chandu"},
 {21.125610, 79.047584, "3rd Year Mess"},
 {21.125474, 79.048072, "MAC"},
 {21.126209, 79.047920, "HB-8"},
 {21.124299, 79.049358, "Canteen"},
 {21.123490, 79.049869, "New Admin Building"},
 {21.123842, 79.050811, "MME"},
 {21.124544, 79.051345, "Audi"},
 {21.125144, 79.049936, "CME"},
 {21.125186, 79.050722, "CRC"},
 {21.124883, 79.052228, "CSE Dept"},
 {21.124216, 79.052841, "ECE Dept"},
 {21.125277, 79.051362, "Library"},
 {21.123241, 79.051565, "VNIT \n Main Building"},
 {21.122942, 79.051522, "Visvesvaraya \n Statue"},
 {21.122265, 79.052502, "EE Dept"},
 {21.123193, 79.052276, "Civ Engg"},
 {21.123100, 79.052889, "Env Engg"},
//  {21,123667, 79.052581, "Architecure"},
//  {21.128371, 79.052107, "Bhoot Galli"},
//  {21.127358, 79.054153, "Y-Point"},
//  {21.127813, 79.056421, "Girls Hostel"},
//  {21.128437, 79.055189, "Sports Complex"},
};


// Define the number of locations in the lookup table
const int numLocations = 21;


void setup() {
  Serial.begin(9600); // initialize the serial port for debugging
  gpsSerial.begin(9600); // initialize the software serial port for the GPS module
  lcd.begin(16, 2); // initialize the LCD screen with 16 columns and 2 rows
}

void loop()
{
  while (gpsSerial.available() > 0){
    if (gps.encode(gpsSerial.read())){
      lcd.clear();
      displayInfo();
    }
  }
  if (millis() > 5000 && gps.charsProcessed() < 15)
  {
    Serial.println(F("No GPS detected: check wiring."));
    delay(500);
  }
  delay(100);
}

void displayInfo() {
  // Read data from the GPS module
    float lattitude = gps.location.lat();
    float longitude = gps.location.lng();
    
    String locationName = "Location not found";
    float dist = 1000;
    for (int i = 0; i < numLocations; i++) {
      if (abs(locations[i].lattitude - lattitude) < 0.001 && abs(locations[i].longitude - longitude) < 0.001) { // allow for some rounding error
        float temp_dist = sqrt(pow((locations[i].lattitude - lattitude),2)+pow((locations[i].longitude - longitude),2));
        dist = min(temp_dist, dist);
        if(dist == temp_dist){
          locationName = locations[i].nameOfPlace;
        }       
      }
    }
    
    // Print the location name to the LCD screen
      lcd.clear();
      //lcd.print("Lat: ");
      //delay(500);
      //lcd.setCursor(0, 0);
      lcd.print("Lat: ");
      lcd.print(lattitude);
      //delay(1000);
      //lcd.clear();
      //lcd.print(", Lon: ");
      //delay(500);
      lcd.setCursor(0, 1);
      lcd.print("Lon: ");
      //delay(1000);
      lcd.print(longitude);
      delay(1000);
      lcd.clear();
      lcd.print("   ");
      lcd.print(locationName);
      //delay(100);
      if(locationName.length() > 15){
      lcd.autoscroll();
      for(int i=0; i<10; i++){
        lcd.print(" ");
        delay(500);
      }
      lcd.noAutoscroll();
      }
      else{
        delay(1000);
      }
  
    // output
    Serial.print("Lat: ");
    Serial.print(gps.location.lat(), 10);
    Serial.print("Lon: ");
    Serial.print(gps.location.lng(), 10);
    Serial.print(", Location: ");
    Serial.println(locationName);
  
}
