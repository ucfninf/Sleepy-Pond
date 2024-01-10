#include <DHT.h>                //input
#include <DHT_U.h>
#include <Adafruit_NeoPixel.h>  //Output
#include <FastLED.h> //extension

#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
#define MAX4466_PIN A0  // Analog pin for noise sensor connection
#define LDR_PIN A1      // Analog pin for photoresistor connection

#define LED_PIN 4   // Digital pins for LED strip connections
#define NUM_LEDS 8  // The number of LEDs on the LED strip

#define Vcc 5.0  // Board Voltage

int color1 = 0;
int color2 = 0;
int color3 = 0;

uint8_t DHTPin = 2;        // on Pin 12 of the Huzzah
DHT dht(DHTPin, DHTTYPE);   // Initialize DHT sensor.
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);
CRGB leds[NUM_LEDS];

// microphone window
const int sampleWindow = 50;  // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;
int brightness = 128;  // Initialize brightness to a default value

// Zero point calibration value and sensitivity calibration value, adjusted according to sensor specifications *********************************
float sensitivityCalibration = 5.0;  // Sensor sensitivity
int zeroPointCalibration = 100;  // Sensor zero point calibration value


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  strip.begin();

  //LDR
  int dutyCycle = 100;
  analogWrite(LDR_PIN, dutyCycle);

  pinMode(DHTPin, INPUT);
  dht.begin();

  // Initialize FastLED 
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
  FastLED.show(); // Initialize all LEDs to "off"
}


void loop() {

  // Read temperature and humidity data
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Reading noise level
  int noiseLevel = analogRead(MAX4466_PIN);
  unsigned long startMillis = millis(); // Start of sample window
  unsigned int peakToPeak = 0;   // peak-to-peak level
 
  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;

// Convert to voltage********************************************************
float volts = (noiseLevel * 5.0) / 1024.0;

  // collect data for 50 mS
  while (millis() - startMillis < sampleWindow)
  {
    sample = analogRead(MAX4466_PIN);
    if (sample < 1024)  // toss out spurious readings
    {
      if (sample > signalMax)
      {
        signalMax = sample;  // save just the max levels
      }
      else if (sample < signalMin)
      {
        signalMin = sample;  // save just the min levels
      }
    }
  }

  peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
  // double volts = (peakToPeak * 5.0) / 256;  // convert to volts************************
  Serial.println(volts);

 // Convert sensor readings to decibels********************************
  float dB = 20 * log10(volts / sensitivityCalibration) + zeroPointCalibration;
  Serial.println(noiseLevel);

  // Read light intensity on LDR
  int lightIntensity = analogRead(LDR_PIN);

  // Print information to Serial Monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C, Humidity: ");
  Serial.print(humidity);

  Serial.print(" %, Noise Level: ");
  Serial.println(noiseLevel);

  Serial.print(" LDR Value: ");
  Serial.println(lightIntensity);

  delay(100);  // Wait for a while before doing the next read

// Display light intensity on LED strip
  int mappedIntensity = map(lightIntensity, 0, 1023, 0, 255);
  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, strip.Color(mappedIntensity, mappedIntensity, 0));
  }


  // Set the brightness of the LED strip
  for (int i = 5; i < NUM_LEDS; i++) {
    leds[i].nscale8(brightness);  // Gradient based on brightness
  }
   
  // Control light strip color based on noise level
  if (noiseLevel > 600) {
    for (int i = 1; i < 7 ; i++) {
      strip.setPixelColor(i, strip.Color(255, 0, 0));  // Red
    } //Above 500, red light lights up
  }
  
  else if (noiseLevel > 300 && noiseLevel < 600){
    for (int i = 1; i < 7 ; i++) {
      strip.setPixelColor(i, strip.Color(87, 25, 0));  // Yellow
    } 
  // Below 500, yellow light appears in the middle of the value
  }

  else {
    for (int i = 1; i < 7 ; i++) {
      strip.setPixelColor(i, strip.Color(87, 70, 89));  // light blue
    } 
  // Below 500, minimum display blue light
  }

  if (temperature > 26) {
      strip.setPixelColor(0, strip.Color(255, 0, 0));  // red
  }
  
  else if (temperature > 21 && temperature < 26){
      strip.setPixelColor(0, strip.Color(87, 25, 0));  // yellow
  // Below 500, yellow light appears in the middle of the value
  }

  else {
      strip.setPixelColor(0, strip.Color(87, 70, 89));  // light blue
  // Below 500, minimum display blue light
  }

  if (humidity < 30) {
      strip.setPixelColor(7, strip.Color(255, 0, 0));  // Red
  }
  
  else if (humidity > 30 && humidity < 60){
      strip.setPixelColor(7, strip.Color(87, 25, 0));  // Yellow
  // Below 500, yellow light appears in the middle of the value
  }

  else {
      strip.setPixelColor(7, strip.Color(87, 70, 89));  // blue
  // Below 500, minimum display blue light
  }

  strip.show(); // Show Adafruit_NeoPixel LED colors
  //FastLED.show(); // Show FastLED LED brightness

  delay(100);

}
