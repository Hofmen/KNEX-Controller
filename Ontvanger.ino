#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10); // CE, CSN pins

int joystick1XValue, joystick1YValue, joystick2XValue, joystick2YValue;

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(1, 0xF0F0F0F0E1LL); // Pipe 1, Adres 0xF0F0F0F0E1LL
  radio.startListening();
}

void loop() {
  if (radio.available()) {
    // Ontvang de gegevens van de verzender
    char data[8];
    radio.read(&data, sizeof(data));

    // Haal de waarden van de joysticks op uit de ontvangen gegevens
    memcpy(&joystick1XValue, data, sizeof(joystick1XValue));
    memcpy(&joystick1YValue, data + sizeof(joystick1XValue), sizeof(joystick1YValue));
    memcpy(&joystick2XValue, data + 2 * sizeof(joystick1XValue), sizeof(joystick2XValue));
    memcpy(&joystick2YValue, data + 3 * sizeof(joystick1XValue), sizeof(joystick2YValue));

    // Verwerk de ontvangen waarden (je kunt hier bijvoorbeeld de waarden afdrukken)
    Serial.print("Joystick 1 - X: ");
    Serial.print(joystick1XValue);
    Serial.print(", Y: ");
    Serial.println(joystick1YValue);

    Serial.print("Joystick 2 - X: ");
    Serial.print(joystick2XValue);
    Serial.print(", Y: ");
    Serial.println(joystick2YValue);
  }
}
