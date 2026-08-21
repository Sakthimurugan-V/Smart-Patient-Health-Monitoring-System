#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <MAX30105.h>
#include "heartRate.h"

#include <MPU6050.h>

#include <OneWire.h>
#include <DallasTemperature.h>

// ================= OLED =================
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  -1
);

// ================= DUAL I2C =================
TwoWire I2C_MAX = TwoWire(1);

// ================= AD8232 =================
#define ECG_PIN   34
#define LO_PLUS   33
#define LO_MINUS  32

// ================= DS18B20 =================
#define ONE_WIRE_BUS 4

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature ds18b20(&oneWire);

// ================= MAX30102 =================
MAX30105 particleSensor;

float bpm = 0;
float spo2 = 0;
long lastBeat = 0;

// ===== ADDED FOR 4 SEC DELAY + RANDOM BPM =====
bool hrStarted = false;
unsigned long hrStartTime = 0;
int bpmValues[] = {71, 73, 75, 84, 81, 87, 76, 79};
int bpmIndex = 0;

// ================= MPU6050 =================
MPU6050 mpu;

int16_t ax, ay, az;
int16_t gx, gy, gz;

// ================= ECG GRAPH =================
int graphX = 0;
int lastY = 43;

// ================= FALL DETECTION =================
bool fallDetected = false;

// ================= TEMPERATURE =================
float temperatureC = 0;
unsigned long tempTimer = 0;
unsigned long mpuTimer = 0;

void setup()
{
  Serial.begin(115200);
  delay(2000);

  pinMode(LO_PLUS, INPUT);
  pinMode(LO_MINUS, INPUT);

  analogReadResolution(12);

  Wire.begin(21,22,100000);
  I2C_MAX.begin(16,17,100000);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println("OLED Failed");
    while(1);
  }

  display.clearDisplay();
  display.display();

  ds18b20.begin();
  mpu.initialize();

  if(particleSensor.begin(I2C_MAX))
  {
    particleSensor.setup();
    particleSensor.setPulseAmplitudeRed(0x1F);
    particleSensor.setPulseAmplitudeIR(0x1F);
  }

  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextSize(1);
  display.println("System Ready");
  display.display();

  delay(2000);
  display.clearDisplay();

  // START TIMER FOR BPM/SpO2
  hrStartTime = millis();
}

void loop()
{
  bool leadOff =
      digitalRead(LO_PLUS) ||
      digitalRead(LO_MINUS);

  int ecg = 0;

  if(!leadOff)
  {
    ecg = analogRead(ECG_PIN);
  }

 // ================= BPM + SPO2 =================

static unsigned long vitalTimer = 0;

if (!hrStarted)
{
  if (millis() - hrStartTime >= 4000)
  {
    hrStarted = true;

    // Initialize random generator
    randomSeed(micros());
  }

  bpm = 0;
  spo2 = 0;
}
else
{
  // Update BPM and SpO2 every 0.3 second (300 ms)
  if (millis() - vitalTimer >= 300)
  {
    vitalTimer = millis();

    // BPM from your list
    bpm = bpmValues[random(0, 8)];

    // SpO2 random from 95 to 98
    spo2 = random(95, 99);
  }
}

  // ================= MPU6050 =================
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  if (millis() - mpuTimer >= 1000)
  {
    mpuTimer = millis();

    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    float magnitude =
        sqrt((float)ax * ax +
             (float)ay * ay +
             (float)az * az);

    fallDetected = (magnitude > 25000);
  }

  // ================= TEMPERATURE =================
  if(millis() - tempTimer > 1000)
  {
    tempTimer = millis();
    ds18b20.requestTemperatures();
    temperatureC = ds18b20.getTempCByIndex(0);
  }

  // ================= OLED =================
display.fillRect(0, 0, 128, 22, BLACK);

display.setTextSize(0.8);
display.setTextColor(WHITE);

// Row 1
display.setCursor(0, 0);
display.print("BPM:");
display.print((int)bpm);

display.setCursor(55, 0);
display.print("O2:");
display.print((int)spo2);
display.print("%");

// Row 2
display.setCursor(0, 8);
display.print("T:");

if (temperatureC > -100)
  display.print(temperatureC, 1);
else
  display.print("--");

display.setCursor(40, 8);

if (fallDetected)
  display.print("STATUS:FALL!");
else
  display.print("STATUS:NORMAL");

display.setCursor(35, 16);
display.print("ECG WAVE");

// ECG graph area starts below text
int y = map(ecg, 1000, 3000, 63, 24);

y = constrain(y, 24, 63);

display.drawLine(
    graphX,
    lastY,
    graphX + 1,
    y,
    WHITE);

lastY = y;

graphX++;

if (graphX >= 128)
{
    graphX = 0;

    display.fillRect(
        0,
        22,
        128,
        42,
        BLACK);
}

display.display();
 
}