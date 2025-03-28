/* Nodemcu ESP8266 WiFi Control Car
 * Optimized version with stability fixes
 */

#define BLYNK_TEMPLATE_ID "TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "TEMPLATE_NAME"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

BlynkTimer timer;  // ✅ Replace SimpleTimer with BlynkTimer

// Motor PINs
#define ENA D1
#define IN1 D3  
#define IN2 D4  
#define IN3 D5  
#define IN4 D6  
#define ENB D2  

// LED Pin
#define LED D7  

bool forward = 0, backward = 0, left = 0, right = 0;
int Speed = 500;  

char auth[] = "AUTH_KEY";  
char ssid[] = "WIFI_SSID";  
char pass[] = "WIFI_PASSWORD";  

void setup() {
  Serial.begin(9600);
  
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(LED, OUTPUT);
  
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  
  // ✅ Use BlynkTimer instead of SimpleTimer
  timer.setInterval(100L, smartcar);
}

BLYNK_WRITE(V0) { forward = param.asInt(); }
BLYNK_WRITE(V1) { backward = param.asInt(); }
BLYNK_WRITE(V2) { left = param.asInt(); }
BLYNK_WRITE(V3) { right = param.asInt(); }
BLYNK_WRITE(V4) { Speed = param.asInt(); }
BLYNK_WRITE(V5) { digitalWrite(LED, param.asInt()); }  

void smartcar() {
  if (forward && !backward && !left && !right) carForward();
  else if (backward && !forward && !left && !right) carBackward();
  else if (left && !right && !forward && !backward) carTurnLeft();
  else if (right && !left && !forward && !backward) carTurnRight();
  else carStop();
}

void loop() {
  Blynk.run();
  timer.run();  // ✅ Use BlynkTimer
}

void carForward() {
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void carBackward() {
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void carTurnLeft() {
  analogWrite(ENA, Speed / 2);
  analogWrite(ENB, Speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void carTurnRight() {
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed / 2);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void carStop() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
