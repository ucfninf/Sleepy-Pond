#include <DHT.h>
#include <Adafruit_NeoPixel.h>
#include <FastLED.h>

#define DHT_PIN 2
#define DHT_TYPE DHT22
#define MAX4466_PIN A0
#define LDR_PIN A1
#define LED_PIN 4
#define NUM_LEDS 8
#define Vcc 5.0

DHT dht(DHT_PIN, DHT_TYPE);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);
CRGB leds[NUM_LEDS];

// microphone window
const int sampleWindow = 50;
unsigned int sample;

void setup() {
  Serial.begin(9600);
  strip.begin();

  // LDR
  int dutyCycle = 100;
  analogWrite(LDR_PIN, dutyCycle);

  // FastLED
  FastLED.addLeds<WS2811, LED_PIN, RGB>(leds, NUM_LEDS);
  FastLED.setBrightness(84);
}

void loop() {
  readSoundSensor();

  // 读取温湿度数据
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // 读取噪音水平
  int noiseLevel = analogRead(MAX4466_PIN);
  unsigned long startMillis = millis(); // Start of sample window
  unsigned int peakToPeak = 0;   // peak-to-peak level

  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;

  // collect data for 50 mS
  while (millis() - startMillis < sampleWindow) {
    sample = analogRead(MAX4466_PIN);
    if (sample < 1024) {
      if (sample > signalMax) {
        signalMax = sample;  // save just the max levels
      } else if (sample < signalMin) {
        signalMin = sample;  // save just the min levels
      }
    }
  }

  peakToPeak = signalMax - signalMin;
  double volts = (peakToPeak * 5.0) / 256;
  Serial.println(volts);

  // Read light intensity on LDR
  int lightIntensity = analogRead(LDR_PIN);

  // Print information to Serial Monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C, Humidity: ");
  Serial.print(humidity);
  Serial.print(" %, Noise Level: ");
  Serial.println(noiseLevel);
  Serial.print("LDR Value: ");
  Serial.println(lightIntensity);

  // 其他代码...

  delay(3000);
}

void readSoundSensor() {
  // 这是你之前提供的 readSoundSensor() 函数
  // 你可以根据需要进行修改或者保留原样
}

// 控制 LED 条颜色和亮度的渐变效果
void gradientEffect(int value) {
  int hue = map(value, 0, 87, 159, 145); // 根据传感器值映射颜色
  int brightness = map(value, 0, 1023, 0, 255); // 根据传感器值映射亮度

  // 设置 LED 条的颜色和亮度
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(hue, 255, brightness);
  }
  
  FastLED.show();
}