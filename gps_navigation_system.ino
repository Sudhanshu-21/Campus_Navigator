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
  {40.758701, -73.985682, "Times Square, NYC"},
  {51.507351, -0.127758, "Big Ben, London"},
  {21.1253125, 79.0461769, "OAT"},
  {21.1254051, 79.0455315, "HB-4"},
  {21.1245682, 79.0461863, "HB-3},
  {21.125290, 79.047375, "Chandrama"},
  {21.124299, 79.049358, "Canteen"}
  {21.123490, 79.049869, "New Admin Building"}
  {21.123842, 79.050811, "MME"}
};


// Define the number of locations in the lookup table
const int numLocations = sizeof(locations) / sizeof(Location);


void setup() {
  Serial.begin(9600); // initialize the serial port for debugging
  gpsSerial.begin(9600); // initialize the software serial port for the GPS module
  lcd.begin(16, 2); // initialize the LCD screen with 16 columns and 2 rows
  lcd.print("GPS Lookup Table"); // print a header to the LCD screen
  delay(100);
  lcd.clear();
}

void loop()
{
  while (gpsSerial.available() > 0)
    if (gps.encode(gpsSerial.read()))
      displayInfo();

  if (millis() > 5000 && gps.charsProcessed() < 15)
  {
    Serial.println(F("No GPS detected: check wiring."));
    delay(7000);
  }
}

void displayInfo() {
  // Read data from the GPS module
    float lattitude=0;
    float longitude=0;
   
    if (gpsData.startsWith("$GPGGA")) { // only process GGA messages
      // Parse the latitude and longitude from the GPS data
      lattitude = gps.location.lat();
      longitude = gps.location.lng();
    }
    // Look up the name of the location from the latitude and longitude
    
    String locationName = "Location not found";
    float dist = 0;
    for (int i = 0; i < numLocations; i++) {
      if (abs(locations[i].lattitude - lattitude) < 0.1 && abs(locations[i].longitude - longitude) < 0.1) { // allow for some rounding error
        float temp_dist = sqrt(pow((locations[i].lattitude - lattitude),2)+pow((locations[i].longitude - longitude),2));
        dist = min(temp_dist, dist);
        if(dist == temp_dist){
          locationName = locations[i].nameOfPlace;
        }
      }
    }
    
    // Print the location name to the LCD screen
      lcd.clear();
      lcd.print("Lat: ");
      delay(500);
      lcd.setCursor(0, 1);
      lcd.print(lattitude);
      delay(1000);
      lcd.clear();
      lcd.print("Lon: ");
      delay(500);
      lcd.setCursor(0, 1);
      lcd.print(longitude);
      delay(1000);
      lcd.clear();
      lcd.print(locationName);
      delay(10000);
    
  
    // output
    Serial.print("Lat: ");
    Serial.print(gps.location.lat(), 10);
    Serial.print("Lon: ");
    Serial.print(gps.location.lng(), 10);
    Serial.print(", Location: ");
    Serial.println(locationName);
  
}
