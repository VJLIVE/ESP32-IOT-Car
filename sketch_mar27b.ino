/* Nodemcu ESP8266 WiFi Control Car - Optimized & Safe */

#define BLYNK_TEMPLATE_ID "TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "TEMPLATE_NAME"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

BlynkTimer timer;

// Motor PINs
#define ENA D1
#define IN1 D3  
#define IN2 D4  
#define IN3 D5  
#define IN4 D6  
#define ENB D2  

// LED Pin
#define LED D7

// Ultrasonic Sensor
#define TRIG_PIN D8
#define ECHO_PIN D0

bool forward = 0, backward = 0, left = 0, right = 0;
int Speed = 500;
bool obstacleDetected = false;

char auth[] = "AUTH_KEY";  
char ssid[] = "SSID";  
char pass[] = "PASS";  

void setup() {
  Serial.begin(9600);
  
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(LED, OUTPUT);
  
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  timer.setInterval(100L, smartcar);  // Repeatedly check and act
}

// Virtual Pin Inputs
BLYNK_WRITE(V0) { forward = param.asInt(); }
BLYNK_WRITE(V1) { backward = param.asInt(); }
BLYNK_WRITE(V2) { left = param.asInt(); }
BLYNK_WRITE(V3) { right = param.asInt(); }
BLYNK_WRITE(V4) { Speed = param.asInt(); }
BLYNK_WRITE(V5) {
  int ledState = param.asInt();
  digitalWrite(LED, ledState);
}

void loop() {
  Blynk.run();
  timer.run();
}

void checkObstacle() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 20000); // timeout 20ms
  int distance = duration * 0.034 / 2;
   Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  obstacleDetected = (distance > 0 && distance < 50);  // < 50 cm = obstacle
}

void smartcar() {
  checkObstacle();

  if (forward && !backward && !left && !right) {
    if (!obstacleDetected) {
      carForward();
    } else {
      carStop();  // Block forward due to obstacle
    }
  }
  else if (backward && !forward && !left && !right) carBackward();
  else if (left && !right && !forward && !backward) carTurnLeft();
  else if (right && !left && !forward && !backward) carTurnRight();
  else carStop();
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
