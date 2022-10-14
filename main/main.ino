/* Started Date : 14 Oct 2022 */
/* TemperatureControlledKettle code is here . It is running the 
communication through blynk server and the new BLYNK 2.0 platform. */

#define BLYNK_TEMPLATE_ID "TMPLxxxxxx"
#define BLYNK_DEVICE_NAME "Device"
#define BLYNK_AUTH_TOKEN "YourAuthToken"


// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "88888888";
char pass[] = "88888888";

BlynkTimer timer;  // Initialize timer as a constructor of object BlynkTimer
#define stat_pin D1

int state = 0;

void status()  /* Function for checking the mode_status . If the digitalpin reads high 
then the kettle is in Manual Mode and if reads low then it is in Auto mode */
{
  state = digitalRead(stat_pin);
  Blynk.virtualWrite(V0, state);
}


void setup() {
  Serial.begin(115200);  //Initialize serial communication
  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);


  timer.setInterval(1000L, status); // Calling the status function every second.
}

void loop() {
  Blynk.run();
  timer.run();
}