#include "notes.h"
int BUZZER_PIN = 1;
int WHOLENOTE = 60000 * 4 / 105;

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
}

void playNote(int buzzerTone, int duration) {
  tone(BUZZER_PIN, buzzerTone, duration * 0.9);
  delay(duration);
  noTone(BUZZER_PIN);
}

void loop() {
  playNote(NOTE_B4, WHOLENOTE / 16);
      playNote(NOTE_B5, WHOLENOTE / 16);
      playNote(NOTE_FS5, WHOLENOTE / 16);
      playNote(NOTE_DS5, WHOLENOTE / 16);
      playNote(NOTE_B5, WHOLENOTE / 32);
      playNote(NOTE_FS5, WHOLENOTE / 16 * 1.5);
      playNote(NOTE_DS5, WHOLENOTE / 8);
      playNote(NOTE_C5, WHOLENOTE / 16);
      playNote(NOTE_C6, WHOLENOTE / 16);
      playNote(NOTE_G6, WHOLENOTE / 16);
      playNote(NOTE_E6, WHOLENOTE / 16);
      playNote(NOTE_C6, WHOLENOTE / 32);
      playNote(NOTE_G6, WHOLENOTE / 16 * 1.5);
      playNote(NOTE_E6, WHOLENOTE / 8);
      
      playNote(NOTE_B4, WHOLENOTE / 16);
      playNote(NOTE_B5, WHOLENOTE / 16);
      playNote(NOTE_FS5, WHOLENOTE / 16);
      playNote(NOTE_DS5, WHOLENOTE / 16);
      playNote(NOTE_B5, WHOLENOTE / 32);
      playNote(NOTE_FS5, WHOLENOTE / 16 * 1.5);
      playNote(NOTE_DS5, WHOLENOTE / 8);
      playNote(NOTE_DS5, WHOLENOTE / 32);
      playNote(NOTE_E5, WHOLENOTE / 32);
      playNote(NOTE_F5, WHOLENOTE / 32);
      playNote(NOTE_F5, WHOLENOTE / 32);
      playNote(NOTE_FS5, WHOLENOTE / 32);
      playNote(NOTE_G5, WHOLENOTE / 32);
      playNote(NOTE_G5, WHOLENOTE / 32);
      playNote(NOTE_GS5, WHOLENOTE / 32);
      playNote(NOTE_A5, WHOLENOTE / 16);
      playNote(NOTE_B5, WHOLENOTE / 8);
}
