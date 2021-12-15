#include "notes.h"
int BUZZER_PIN = 11;
int WHOLENOTE = 60000 * 4 / 105;

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
}

void playTone(int buzzerTone, int duration) {
  tone(BUZZER_PIN, buzzerTone, duration * 0.9);
  delay(duration);
  noTone(BUZZER_PIN);
}

void loop() {
  playTone(NOTE_B4, WHOLENOTE / 16);
      playTone(NOTE_B5, WHOLENOTE / 16);
      playTone(NOTE_FS5, WHOLENOTE / 16);
      playTone(NOTE_DS5, WHOLENOTE / 16);
      playTone(NOTE_B5, WHOLENOTE / 32);
      playTone(NOTE_FS5, WHOLENOTE / 16 * 1.5);
      playTone(NOTE_DS5, WHOLENOTE / 8);
      playTone(NOTE_C5, WHOLENOTE / 16);
      playTone(NOTE_C6, WHOLENOTE / 16);
      playTone(NOTE_G6, WHOLENOTE / 16);
      playTone(NOTE_E6, WHOLENOTE / 16);
      playTone(NOTE_C6, WHOLENOTE / 32);
      playTone(NOTE_G6, WHOLENOTE / 16 * 1.5);
      playTone(NOTE_E6, WHOLENOTE / 8);
      
      playTone(NOTE_B4, WHOLENOTE / 16);
      playTone(NOTE_B5, WHOLENOTE / 16);
      playTone(NOTE_FS5, WHOLENOTE / 16);
      playTone(NOTE_DS5, WHOLENOTE / 16);
      playTone(NOTE_B5, WHOLENOTE / 32);
      playTone(NOTE_FS5, WHOLENOTE / 16 * 1.5);
      playTone(NOTE_DS5, WHOLENOTE / 8);
      playTone(NOTE_DS5, WHOLENOTE / 32);
      playTone(NOTE_E5, WHOLENOTE / 32);
      playTone(NOTE_F5, WHOLENOTE / 32);
      playTone(NOTE_F5, WHOLENOTE / 32);
      playTone(NOTE_FS5, WHOLENOTE / 32);
      playTone(NOTE_G5, WHOLENOTE / 32);
      playTone(NOTE_G5, WHOLENOTE / 32);
      playTone(NOTE_GS5, WHOLENOTE / 32);
      playTone(NOTE_A5, WHOLENOTE / 16);
      playTone(NOTE_B5, WHOLENOTE / 8);
}
