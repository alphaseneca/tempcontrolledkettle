/* Started Date : 14 Oct 2022 */
/* TemperatureControlledKettle code is here . It is running the 
communication through blynk server and the new BLYNK 2.0 platform. */
/* Author: Ukesh Aryal */
/* https://github.com/alphaseneca */

#include "secret.h"

// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = ID;
char pass[] = PASSWORD;

BlynkTimer timer;         // Initialize timer as a constructor of object BlynkTimer
#define stat_pin D1       // Get the status of the kettle whether it is in Auto or Manual
#define relay_pin D7      // Relay for the heater
                          /* Future Implementation Power MOSFET to dim the heater and Obtain PID controller*/
#define probe_pin A0      //Temperature Probe connected to the analog pin

int state = 0;            // Initialization of all the variables
int mode_select = 0;
int thres_temp;
int current_temp;
int heatersw;

void status()           /*  Function for checking the mode_status . If the digitalpin reads high 
                            then the kettle is in Manual Mode and if reads low then it is in Auto mode
                        */
{
  state = digitalRead(stat_pin);   // Reset all the values and latches the relay off if Manual mode is selected. 
  Blynk.virtualWrite(V0, state);
  if (state == 1) {
    heatersw = 0;
    mode_select = 0;
    digitalWrite(relay_pin, LOW);
    Blynk.virtualWrite(V5, heatersw);
    Blynk.virtualWrite(V1, mode_select);
    Blynk.virtualWrite(V6, 0);
  }
}

BLYNK_WRITE(V1)     /*  Normally Cut Off mode is selected as we have initialized the mode_select value as 0 
                        But when we want to manitain the temperature then this fumction gets called when there 
                        is change in data and the data i.e 1 is set to the mode_select 
                    */
{
  mode_select = param.asInt();
}

BLYNK_WRITE(V2)
{
  thres_temp = param.asInt();             // Get the value from the slider_ set temperature
  Serial.println(thres_temp);
  Blynk.virtualWrite(V3, thres_temp);     // Display the set temperature to the display at V3
}

void readtemperature() {  /*  Function reading the current temperature and sending the data to 
                              the virtual pin V4 and setting the value to the current_temp 
                          */

  current_temp = analogRead(probe_pin);
  current_temp = map(current_temp, 0, 1024, 0, 100);  // Mapping the obtained value from 0-1024 to 0-100 and setting that as new value
  Blynk.virtualWrite(V4, current_temp);               // Uploading the currenttemperature reading to the server
  tempcontrol();          // Calling the temperature control function
}

void tempcontrol() {      // ALL THE CONTROL LOGIC LIES IN HERE 

  int tempdiff;
  int re_activationtemp = 5;

  switch (mode_select) {                    // Switch for mode_select value 
    case 0:                                 // If mode_select == 0 i.e Cut OFF mode
      if (current_temp >= thres_temp && heatersw == 1) {     // If the currrent temperarature is equals to or more than the thres_temp then turn off the heater 
        digitalWrite(relay_pin, LOW);
        heatersw = 0;                 
        Blynk.virtualWrite(V5, heatersw);   // Update the value to the server
        Blynk.virtualWrite(V6, 0);
        Serial.println("Threshold Temperature reached and heater has been turned off");
      }
      else 
        Serial.println("Waiting..."); 
      break;

    case 1:                                 // If mode_select == 1 i.e Maintain mode
      if (current_temp >= thres_temp && heatersw == 1) {
        digitalWrite(relay_pin, LOW);
        Blynk.virtualWrite(V6, 0);
        Serial.println("Threshold Temperature reached and heater has been turned off");        
                
      }
       else if (thres_temp > current_temp && heatersw == 1) {
        tempdiff = thres_temp - current_temp;
        if (tempdiff > re_activationtemp) {
          digitalWrite(relay_pin, HIGH);
          Blynk.virtualWrite(V6, 1);
          Serial.println("Heater has been reactivated to maintain the temperature");
        }
        else
        Serial.println("Waiting...");       
      }
  }
}

BLYNK_WRITE(V5) {
  heatersw = param.asInt();
                                              /*  Checks if the status is in Manual or Auto, only if the 
                                                  status is in auto mode the heater switch is turned on 
                                              */
  if (state == 0 && heatersw == 1) 
  {
    digitalWrite(relay_pin, HIGH);
    Blynk.virtualWrite(V6, 1);
    Serial.println("Heater Turned On");
  }
   else if (state == 0 && heatersw == 0) {
    digitalWrite(relay_pin, LOW);
    Blynk.virtualWrite(V6, 0);   
    Serial.println("Heater Turned Off");
  } else
    Serial.println("Cannot turn on the heater. Set the heater to AUTO mode ");
}



void setup() {
  Serial.begin(9600);  //Initialize serial communication
  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  pinMode(relay_pin, OUTPUT);

  timer.setInterval(1000L, status);
  timer.setInterval(1000L, readtemperature);  // Calling the readtemperature function every second.
}

void loop() {
  Blynk.run();
  timer.run();
}