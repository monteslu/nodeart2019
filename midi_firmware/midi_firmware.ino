
#include "MIDIUSB.h"

#define NUM_BUTTONS  8
#define MIDDLE_C 53

const uint8_t button1 = 2;
const uint8_t button2 = 3;
const uint8_t button3 = 4;
const uint8_t button4 = 5;
const uint8_t button5 = 6;
const uint8_t button6 = 7;
const uint8_t button7 = 8;
const uint8_t button8 = 9;

const uint8_t buttons[NUM_BUTTONS] = {button1, button2, button3, button4, button5, button6, button7, button8};
uint8_t buttonStates[NUM_BUTTONS] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH};
uint8_t newButtonStates[NUM_BUTTONS] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH};

uint8_t ctlStates[4] = {200, 200, 200, 200};
uint8_t newCtlStates[4] = {200, 200, 200, 200};

void controlChange(byte control, byte val) {
  Serial.print("\r\n controlChange val");
  Serial.print(val);
  midiEventPacket_t mod_packet = {0x0B, 0xB0, control, val};
  MidiUSB.sendMIDI(mod_packet);
}

void noteOn(byte note, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90, note, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte note, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80, note, velocity};
  MidiUSB.sendMIDI(noteOff);
}

void setup() {
  Serial.begin(9600);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
}

void loop() {
  for (int a = 0; a < 4; a++)
  {
    newCtlStates[a] = analogRead(a + 1) >> 3; // read potentiometer and truncate to 7 bits
    if(ctlStates[a] != newCtlStates[a]) {
      ctlStates[a] = newCtlStates[a];
      controlChange(a + 1, ctlStates[a]);
      MidiUSB.flush();
      delay(5);
    }
  }

  for (int i = 0; i < NUM_BUTTONS; i++)
  {
    newButtonStates[i] = digitalRead(buttons[i]);

    if (newButtonStates[i]  == LOW && buttonStates[i] == HIGH){
      // If button pushed, turn LED on
      buttonStates[i] = LOW;
      Serial.print("\r\n button down");
      noteOn(MIDDLE_C + i, 64);
      MidiUSB.flush();
      delay(5);
      
     } else if(newButtonStates[i] == HIGH && buttonStates[i] == LOW){
       buttonStates[i] = HIGH;
       Serial.print("\r\n button up");
       noteOff(MIDDLE_C + i, 64);
       MidiUSB.flush();
       delay(5);
     }
    
  }
}
