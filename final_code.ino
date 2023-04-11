#include <SoftwareSerial.h> // include the SoftwareSerial library for serial communication with the GPS module
#include <LiquidCrystal.h> // include the LiquidCrystal library for the LCD screen

SoftwareSerial gpsSerial(10, 11); // set up a software serial port for the GPS module (RX=10, TX=11)
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // set up the LCD screen (RS=12, E=11, D4=5, D5=4, D6=3, D7=2)

// Define a struct to hold location data
struct Location {
  float lat;
  float Lon;
  String name;
};

// Define a lookup table of locations (replace with your own)
Location locations[] = {
  {40.758701, -73.985682, "Times Square, NYC"},
  {51.507351, -0.127758, "Big Ben, London"},
  {21.1222526, 79.041468, "Nagpur"}
};

// Define the number of locations in the lookup table
const int numLocations = sizeof(locations) / sizeof(Location);

void setup() {
  Serial.begin(9600); // initialize the serial port for debugging
  gpsSerial.begin(9600); // initialize the software serial port for the GPS module
  lcd.begin(16, 2); // initialize the LCD screen with 16 columns and 2 rows
  lcd.print("GPS Lookup Table"); // print a header to the LCD screen
  delay(2000);
}

void loop() {
      char locationName[] = "yes";
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("Lat: ");
      delay(1000);
      //lcd.print(Location.lat, 6);
      delay(10);
      lcd.setCursor(0, 1);
      lcd.print("Lon: ");
      delay(1000);
      //lcd.print(Location.lon, 6);
      delay(1000);
      //lcd.setCursor(0, 1);
      lcd.print(locationName);
      delay(10);
  // Read data from the GPS module
  String gpsData = gpsSerial.readStringUntil('\n');
  if (gpsData.startsWith("$GPGGA")) { // only process GGA messages
    // Parse the latitude and longitude from the GPS data
    int comma1 = gpsData.indexOf(",");
    int comma2 = gpsData.indexOf(",", comma1 + 1);
    int comma3 = gpsData.indexOf(",", comma2 + 1);
    int comma4 = gpsData.indexOf(",", comma3 + 1);
    int comma5 = gpsData.indexOf(",", comma4 + 1);
    float lat = gpsData.substring(comma1 + 1, comma2).toFloat();
    float lon = gpsData.substring(comma3 + 1, comma4).toFloat();
    // Look up the name of the location from the latitude and longitude

//    for (int i = 0; i < numLocations; i++) {
//      if (abs(locations[i].lat - lat) < 10 && abs(locations[i].lon - lon) < 10) { // allow for some rounding error
//        locationName = locations[i].name;
//        break;
//      }
//    }
    
    // Print the location name to the LCD screen

    
  
    // output
    Serial.print("Lat: ");
    Serial.print(lat, 6);
    Serial.print(", Lon: ");
    Serial.print(lon, 6);
    Serial.print(", Location: ");
    Serial.println(locationName);
  }
}
