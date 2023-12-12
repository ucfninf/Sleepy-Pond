#include <DHT.h>                //输入
#include <DHT_U.h>
#include <Adafruit_NeoPixel.h>  //输出
#include <FastLED.h> //插件？

#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
#define MAX4466_PIN A0  // 噪音传感器连接的模拟引脚
#define LDR_PIN A1      // 光敏电阻连接的模拟引脚

#define LED_PIN 4   // LED 灯条连接的数字引脚
#define NUM_LEDS 8  // LED 灯条上的LED数量

#define Vcc 5.0  // 板子电压

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

// 零点校准值和灵敏度校准值，根据传感器规格调整 *********************************
float sensitivityCalibration = 5.0;  // 传感器灵敏度
int zeroPointCalibration = 100;  // 传感器零点校准值


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  strip.begin();

  //LDR
  int dutyCycle = 100;
  analogWrite(LDR_PIN, dutyCycle);

  pinMode(DHTPin, INPUT);
  dht.begin();

  // Initialize FastLED 新加的内容
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
  FastLED.show(); // Initialize all LEDs to "off"
}


void loop() {

  // 读取温湿度数据
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // 读取噪音水平
  int noiseLevel = analogRead(MAX4466_PIN);
  unsigned long startMillis = millis(); // Start of sample window
  unsigned int peakToPeak = 0;   // peak-to-peak level
 
  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;

// 转换为电压********************************************************
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

 // 将传感器读数转换为分贝********************************
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

  delay(200);  // 等待一段时间再进行下一次读取

// 在LED条上显示光照强度
  int mappedIntensity = map(lightIntensity, 0, 1023, 0, 255);
  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, strip.Color(mappedIntensity, mappedIntensity, 0));
  }


  // 设置 LED 条的亮度
  for (int i = 5; i < NUM_LEDS; i++) {
    leds[i].nscale8(brightness);  // 根据亮度进行渐变
  }
   
  // 根据噪音水平控制灯条颜色
  if (noiseLevel > 700) {
    for (int i = 1; i < 7 ; i++) {
      strip.setPixelColor(i, strip.Color(255, 0, 0));  // 红色
    } 
  }
  
  else if (noiseLevel > 300 && noiseLevel < 700){
    for (int i = 1; i < 7 ; i++) {
      strip.setPixelColor(i, strip.Color(87, 25, 0));  // 黄色
    } 
  // 500以下，数值中间显示黄色光
  }

  else {
    for (int i = 1; i < 7 ; i++) {
      strip.setPixelColor(i, strip.Color(87, 70, 89));  // 白蓝色
    } 
  // 500以下，最小显示蓝色光
  }

  if (temperature > 26) {
      strip.setPixelColor(0, strip.Color(255, 0, 0));  // 红色
  }
  
  else if (temperature > 22 && temperature < 26){
      strip.setPixelColor(0, strip.Color(87, 25, 0));  // 黄色
  // 500以下，数值中间显示黄色光
  }

  else {
      strip.setPixelColor(0, strip.Color(87, 70, 89));  // 白蓝色
  // 500以下，最小显示蓝色光
  }

  if (humidity > 45) {
      strip.setPixelColor(7, strip.Color(255, 0, 0));  // 红色
  }
  
  else if (humidity > 34 && humidity < 45){
      strip.setPixelColor(7, strip.Color(87, 25, 0));  // 黄色
  // 500以下，数值中间显示黄色光
  }

  else {
      strip.setPixelColor(7, strip.Color(87, 70, 89));  // 白蓝色
  // 500以下，最小显示蓝色光
  }

  strip.show(); // Show Adafruit_NeoPixel LED colors
  //FastLED.show(); // Show FastLED LED brightness

  delay(100);

}
