#include <Wire.h>
#include <LiquidCrystal.h>
#define LM75A_ADDR 0x48
LiquidCrystal lcd(16, 17, 13, 14, 27, 26, 25, 33, 32, 4);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(1000);
  lcd.begin(16, 2);      // 16 columns, 2 rows
  lcd.print("Temp Sensor");
  delay(2000);
  lcd.clear();

  // Initialize I2C
  pinMode(21, INPUT_PULLUP);
  pinMode(22, INPUT_PULLUP);
  Wire.begin(21, 22); // uses ESP32 default I2C pints
  Serial.println("LM75A Temperature Reader");

}

// Function to get temperature from temp sensor
float readLM75ATemperature() {

  Wire.beginTransmission(LM75A_ADDR);
  Wire.write(0x00);

  if (Wire.endTransmission(false) != 0) {
    Serial.println("Write failed");
    return NAN;
  }

  delay(2);

  int bytes = Wire.requestFrom(LM75A_ADDR, 2);
  Serial.print("Bytes: ");
  Serial.println(bytes);

  if (bytes != 2) {
    Serial.println("Read failed");
    return NAN;
  }

  uint8_t msb = Wire.read();
  uint8_t lsb = Wire.read();

  int16_t raw = (msb << 8) | lsb;
  raw >>= 5;

  return raw * 0.125;
}

void loop() {
  // put your main code here, to run repeatedly:
  float temperature = readLM75ATemperature();
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");
  
  lcd.setCursor(0, 0);
  lcd.print("Temperature:");

  lcd.setCursor(0, 1);
  lcd.print(temperature);
  lcd.print(" C   ");   // extra spaces clear leftovers
  delay(1000);
}


