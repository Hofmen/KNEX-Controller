#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10); // CE, CSN pins

int joystick1XPin = A0;
int joystick1YPin = A1;
int joystick2XPin = A2;
int joystick2YPin = A3;

int redPin = 3;
int greenPin = 4;
int bluePin = 5;

unsigned long lastTransmissionTime = 0;
unsigned long lastGreenLedTime = 0;
unsigned long lastRedLedBlinkTime = 0;  // Nieuwe variabele toegevoegd
const unsigned long transmissionInterval = 20000; // 20 seconden
const unsigned long blueLedDuration = 1000; // 1 seconde
const unsigned long greenLedTimeout = 20000; // 20 seconden
const unsigned long greenLedDuration = 1000; // 1 seconde
const unsigned long redLedBlinkInterval = 20000; // 20 seconden
const unsigned long redLedBlinkDuration = 1000; // Duur van het knipperen van het rode LED-je

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(0xF0F0F0F0E1LL); // Pipe 0, Adres 0xF0F0F0F0E1LL

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

   // Rode LED
  digitalWrite(redPin, HIGH);
  digitalWrite(greenPin, LOW);
  digitalWrite(bluePin, LOW);
  delay(1000); // Wacht 1 seconde

  // Groene LED
  digitalWrite(redPin, LOW);
  digitalWrite(greenPin, HIGH);
  digitalWrite(bluePin, LOW);
  delay(1000); // Wacht 1 seconde

  // Blauwe LED
  digitalWrite(redPin, LOW);
  digitalWrite(greenPin, LOW);
  digitalWrite(bluePin, HIGH);
  delay(1000); // Wacht 1 seconde
digitalWrite(bluePin, LOW);
}

void loop() {
  // Lees de waarden van de joysticks
  int joystick1XValue = analogRead(joystick1XPin);
  int joystick1YValue = analogRead(joystick1YPin);
  int joystick2XValue = analogRead(joystick2XPin);
  int joystick2YValue = analogRead(joystick2YPin);

  // Creëer een char-array om de gegevens te verzenden
  char data[8];
  memcpy(data, &joystick1XValue, sizeof(joystick1XValue));
  memcpy(data + sizeof(joystick1XValue), &joystick1YValue, sizeof(joystick1YValue));
  memcpy(data + 2 * sizeof(joystick1XValue), &joystick2XValue, sizeof(joystick2XValue));
  memcpy(data + 3 * sizeof(joystick1XValue), &joystick2YValue, sizeof(joystick2YValue));

  // Verstuur de gegevens via de radio
  radio.write(&data, sizeof(data));




  // Controleer of de joystickgegevens buiten het bereik [400, 600] vallen
if ((joystick1XValue < 400 || joystick1XValue > 600) ||
    (joystick1YValue < 400 || joystick1YValue > 600) ||
    (joystick2XValue < 400 || joystick2XValue > 600) ||
    (joystick2YValue < 400 || joystick2YValue > 600)) {
  // Zet het groene LED-je aan als de joystickgegevens buiten het bereik vallen
  digitalWrite(greenPin, HIGH);
} else {
  // Zet het groene LED-je uit als de joystickgegevens binnen het bereik vallen
  digitalWrite(greenPin, LOW);
}


  unsigned long currentTime = millis();
  // Schakel het rode LED-je 1 seconde aan elke 20 seconden als het groene LED-je niet heeft gebrand
  if ((currentTime - lastGreenLedTime) >= redLedBlinkInterval && digitalRead(greenPin) == LOW) {
    // Knipper het rode LED-je
    unsigned long elapsedTime = currentTime - lastRedLedBlinkTime;
    if (elapsedTime <= redLedBlinkDuration) {
      digitalWrite(redPin, HIGH);
    } else if (elapsedTime <= redLedBlinkDuration * 2) {
      digitalWrite(redPin, LOW);
    } else {
      lastRedLedBlinkTime = currentTime;
    }
  } else {
    digitalWrite(redPin, LOW);
  }

  // Update de tijd van de laatste transmissie
  lastTransmissionTime = currentTime;
  // Wacht voor een korte periode voordat je opnieuw leest en verzendt
  delay(100);

}
