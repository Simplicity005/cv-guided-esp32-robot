#include <WiFi.h>
#include <WiFiUdp.h>
#include <ESP32Servo.h> // Library required for precise ESP32 servo timing

// NETWORK CONFIGURATION
const char* ssid     = "esp32";
const char* password = "12345678";
unsigned int localPort = 4210; 

WiFiUDP udp;
char packetBuffer[10]; 

// PHYSICAL MOTOR PINS
const int LEFT_FORWARD_PIN  = 12;
const int LEFT_BACKWARD_PIN = 13;
const int RIGHT_FORWARD_PIN = 14;
const int RIGHT_BACKWARD_PIN = 27;

// PHYSICAL SERVO PIN DEFINITION
const int SERVO_PIN = 18; // Added a free GPIO pin for the servo signal wire
Servo myServo;            // Created the servo object

void setMotors(int lf, int lb, int rf, int rb) {
    digitalWrite(LEFT_FORWARD_PIN, lf);
    digitalWrite(LEFT_BACKWARD_PIN, lb);
    digitalWrite(RIGHT_FORWARD_PIN, rf);
    digitalWrite(RIGHT_BACKWARD_PIN, rb);
    delay(200);
    digitalWrite(LEFT_FORWARD_PIN, 0);
    digitalWrite(LEFT_BACKWARD_PIN, 0);
    digitalWrite(RIGHT_FORWARD_PIN, 0);
    digitalWrite(RIGHT_BACKWARD_PIN, 0);
}

void setup() {
    Serial.begin(115200);

    // Initialize Motor Pins
    pinMode(LEFT_FORWARD_PIN, OUTPUT);
    pinMode(LEFT_BACKWARD_PIN, OUTPUT);
    pinMode(RIGHT_FORWARD_PIN, OUTPUT);
    pinMode(RIGHT_BACKWARD_PIN, OUTPUT);
    setMotors(LOW, LOW, LOW, LOW); 

    // Initialize Servo Pin
    myServo.attach(SERVO_PIN); 
    myServo.write(90); // Default position: Centered at 90 degrees on startup

    // Start the Access Point network
    WiFi.softAP(ssid, password);
    Serial.print("Access Point Active. IP Address: ");
    Serial.println(WiFi.softAPIP()); 

    // Start listening on port 4210
    udp.begin(localPort);
}

void loop() {
    int packetSize = udp.parsePacket();
    
    if (packetSize) {
        int len = udp.read(packetBuffer, 10);
        if (len > 0) {
            packetBuffer[len] = 0; 
        }
        
        char command = packetBuffer[0];
        Serial.print("UDP Directive Received: ");
        Serial.println(command);

        // 1. DC Motor Handling Layout
        if (command == 'w')      setMotors(HIGH, LOW, HIGH, LOW);  // Forward
        else if (command == 's') setMotors(LOW, HIGH, LOW, HIGH);  // Backward
        else if (command == 'a') setMotors(LOW, HIGH, HIGH, LOW);  // Spin Left
        else if (command == 'd') setMotors(HIGH, LOW, LOW, HIGH);  // Spin Right
        else if (command == 'x') setMotors(LOW, LOW, LOW, LOW);    // Stop

        // 2. New Servo Angle Handling Layout
        else if (command == 'u') myServo.write(180); // Move servo to full position (180°)
        else if (command == 'i') myServo.write(90);  // Center the servo position (90°)
        else if (command == 'j') myServo.write(0);   // Move servo to zero position (0°)
    }
    delay(10);
}