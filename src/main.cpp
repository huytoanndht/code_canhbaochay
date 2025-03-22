#include <Arduino.h>
#define BLYNK_TEMPLATE_ID "TMPL6LPLmYRKv"
#define BLYNK_TEMPLATE_NAME "canhbaochayBlynk"
#define BLYNK_AUTH_TOKEN "BZVwpSfr1Xh5_BDr2hSlqPmPnZVBuBXA"
#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

BlynkTimer timer;

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
//char auth[] = "YourAuthToken";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "WIFIPHONGKHACH";
char pass[] = "pk@99999";

#define MQ2_1 33
#define MQ2_2 35
#define BUZZER 23
#define PUMP1 18
#define PUMP2 19

int sensorValue1 = 0;
int sensorValue2 = 0;
boolean state = false;
boolean SW_PUMP1 = false;
boolean SW_PUMP2 = false;


void sendUptime()
{
  sensorValue1 = analogRead(MQ2_1);
  Blynk.virtualWrite(V1, sensorValue1);
  Serial.println(sensorValue1);
  sensorValue2 = analogRead(MQ2_2);
  Blynk.virtualWrite(V2, sensorValue2);
  Serial.println(sensorValue2);

  if (sensorValue1 > 700 || sensorValue2 > 1200) 
  {
    //Blynk.logEvent("canhbao", String("Cảnh báo! Khí gas vượt quá mức cho phép!"));
    digitalWrite(BUZZER, HIGH);
    Blynk.virtualWrite(V3, HIGH);
  }
  else
  {
    digitalWrite(BUZZER, LOW);
    Blynk.virtualWrite(V3, LOW);
  }

}

void SetControl () {

  if (SW_PUMP1 || sensorValue1 > 700) {
    digitalWrite(PUMP1, HIGH);
  } else {
    digitalWrite(PUMP1, LOW);
  }

  if (SW_PUMP2 || sensorValue2 > 1200 ) {
    digitalWrite(PUMP2, HIGH);
  } else {
    digitalWrite(PUMP2, LOW);
  }

}

BLYNK_WRITE(V4) {
  SW_PUMP1 = param.asInt();
}
BLYNK_WRITE(V5) {
  SW_PUMP2 = param.asInt();
}

void setup()
{
  // Debug console
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  pinMode(MQ2_1, INPUT);
  pinMode(MQ2_2, INPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(PUMP1, OUTPUT);
  pinMode(PUMP2, OUTPUT);
  timer.setInterval(1000L, sendUptime);
  timer.setInterval(1000L, SetControl);
  digitalWrite(BUZZER, LOW);
}

void loop()
{
  Blynk.run();
  timer.run();
}
