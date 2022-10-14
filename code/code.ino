

/*Irrigation monitoring system with Nodemcu
 * https://srituhobby.com 
 */

//#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

//LiquidCrystal_I2C lcd(0x27, 16, 2);
char auth[] = "Ng0491DvMc6e1j81hCiHithkRE2ZVxNF";  //Enter your Auth token
char ssid[] = "srituhobby";                        //Enter your WIFI name
char pass[] = "123456";                            //Enter your WIFI password

BlynkTimer timer;

void moisture() {
  int value = analogRead(A0);
  value = map(value, 0, 1023, 0, 100);
  Blynk.virtualWrite(V0, value);
  /*lcd.setCursor(0, 1);
  lcd.print("Moisture: ");
  lcd.print(value);
  lcd.print("  ");
  */Serial.println(value);
}

void setup() {
  Serial.begin(9600);
  Wire.begin(D2, D1);
  //lcd.init();
  //lcd.backlight();
  pinMode(D4, OUTPUT);
  digitalWrite(D4, HIGH);
 // lcd.setCursor(0, 0);
  //lcd.print("IrrigationSystem");
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(100L, moisture);
}
void loop() {
  Blynk.run();
  timer.run();
}