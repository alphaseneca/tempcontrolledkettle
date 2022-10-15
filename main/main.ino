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
#define relay_pin D2
#define probe_pin A0

int state = 0;
int mode_select = 0;
int thres_temp;
int current_temp;
int heatersw;

void status() /* Function for checking the mode_status . If the digitalpin reads high 
then the kettle is in Manual Mode and if reads low then it is in Auto mode */
{
  state = digitalRead(stat_pin);
  Blynk.virtualWrite(V0, state);
}

BLYNK_WRITE(V1) {
  mode_select = param.asInt();
}

BLYNK_WRITE(V2) {  // Get the value from the slider
  thres_temp = param.asInt();
  Serial.println(thres_temp);
}

void readtemperature() { /* Function reading the current temperature and sending the data to 
the virtual pin V4 and setting the value to the current_temp*/
  current_temp = analogRead(probe_pin);
  current_temp = map(current_temp, 0, 1024, 0, 100);
  Blynk.virtualWrite(V4, current_temp);
}




  void setup() {
    Serial.begin(9600);  //Initialize serial communication
    Blynk.begin(auth, ssid, pass);
    // You can also specify server:
    //Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
    //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);


    timer.setInterval(1000L, status);
    timer.setInterval(1000L, readtemperature);   // Calling the status function every second.
  }

  void loop() {
    Blynk.run();
    timer.run();
  }