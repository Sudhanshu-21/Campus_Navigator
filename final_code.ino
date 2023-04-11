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
  {21.1222526, 79.041468, "Nagpur"}
};

// Define the number of locations in the lookup table
const int numLocations = sizeof(locations) / sizeof(Location);
int startsWith(const char* str, const char* prefix) {
  while (*prefix) {
    if (*prefix++ != *str++) {
      return 0;
    }
  }
  return 1;
}

int indexOf(char* str, int start, char* substr) {
    int len1 = strlen(str);
    int len2 = strlen(substr);
    int i, j;

    for (i = start; i <= len1 - len2; i++) {
        for (j = 0; j < len2; j++) {
            if (str[i+j] != substr[j]) {
                break;
            }
        }
        if (j == len2) {
            return i-len2;
        }
    }

    return -1;
}

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

  if (millis() > 10000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    delay(7000);
  }
}

void displayInfo() {
  // Read data from the GPS module
    float lattitude=0;
    float longitude=0;
    String gpsData = gpsSerial.readStringUntil('\n');
    
    lcd.print("check0: ");
    delay(1000);
    lcd.clear();
    if (gpsData.startsWith("$GPGGA")) { // only process GGA messages
      // Parse the latitude and longitude from the GPS data
      lcd.setCursor(0, 1);
      lcd.print("check1: ");
      delay(1000);
      char* ptr = strcpy(new char[gpsData.length() + 1], gpsData.c_str());
      int comma1 = indexOf(ptr, ",", 0);
      int comma2 = indexOf(ptr, ",", comma1 + 1);
      int comma3 = indexOf(ptr, ",", comma2 + 1);
      int comma4 = indexOf(ptr, ",", comma3 + 1);
      int comma5 = indexOf(ptr, ",", comma4 + 1);
      float lattitude = gps.location.lat();
      float longitude = gps.location.lng();
    }
    // Look up the name of the location from the latitude and longitude
    String locationName = "Location not found";
    for (int i = 0; i < numLocations; i++) {
      if (abs(locations[i].lattitude - lattitude) < 10 && abs(locations[i].longitude - longitude) < 10) { // allow for some rounding error
        locationName = locations[i].nameOfPlace;
        break;
      }
    }
    
    // Print the location name to the LCD screen
      lcd.clear();
      lcd.print("Lat: ");
      delay(1000);
      lcd.setCursor(0, 1);
      lcd.print(lattitude);
      delay(1000);
      lcd.clear();
      lcd.print("Lon: ");
      delay(1000);
      lcd.setCursor(0, 1);
      lcd.print(longitude);
      delay(1000);
      lcd.clear();
      lcd.print(locationName);
      delay(10000);
    
  
    // output
    Serial.print("Lat: ");
    Serial.print(gps.location.lat(), 10);

    Serial.print(", Lon: ");
    Serial.print(gps.location.lng(), 10);
    Serial.print(", Location: ");
    Serial.println(locationName);
  
}
