#include "system_constants.h"
#include "tones.h"

class Buzzer {
    // notes
    int currentNote;

    // timers
    unsigned long lastChangedBetweenNotes;


    // --- GENERIC FUNCTIONS FOR PLAYING A SOUND ---
    // playing a note to the buzzer for a given duration
    void playNote (int note, int noteDuration) {
      noTone(BUZZER_PIN);

      // calculating the duration of a note in seconds
      int duration = WHOLENOTE / abs(noteDuration);

      if (noteDuration < 0) {
        // negative duration => doted note
        // duration increases in half
        duration *= 1.5;
      }

      tone(BUZZER_PIN, note, duration);
    }

    // playing a sound to the buzzer for 3 seconds
    void playSound (int sound) {
      tone(BUZZER_PIN, sound, 3000);
      noTone(BUZZER_PIN);
    }

  public:
    Buzzer() {
      // buzzer pin is in OUTPUT mode
      pinMode(BUZZER_PIN, OUTPUT);

      noTone(BUZZER_PIN);
    }


    // --- GAME ---
    // intializing variables for playing a song
    void startSong() {
      lastChangedBetweenNotes = millis() - BETWEEN_NOTES_INTERVAL;
      currentNote = 0;
    }

    void playGameSong() {
      int pacManMelody[62] = {
        NOTE_B4, 16, NOTE_B5, 16, NOTE_FS5, 16, NOTE_DS5, 16,
        NOTE_B5, 32, NOTE_FS5, -16, NOTE_DS5, 8, NOTE_C5, 16,
        NOTE_C6, 16, NOTE_G6, 16, NOTE_E6, 16, NOTE_C6, 32, NOTE_G6, -16, NOTE_E6, 8,

        NOTE_B4, 16,  NOTE_B5, 16,  NOTE_FS5, 16,   NOTE_DS5, 16,  NOTE_B5, 32,
        NOTE_FS5, -16, NOTE_DS5, 8,  NOTE_DS5, 32, NOTE_E5, 32,  NOTE_F5, 32,
        NOTE_F5, 32,  NOTE_FS5, 32,  NOTE_G5, 32,  NOTE_G5, 32, NOTE_GS5, 32,  NOTE_A5, 16, NOTE_B5, 8
      };

      if (millis() - lastChangedBetweenNotes > BETWEEN_NOTES_INTERVAL) {
        if (currentNote < 31) {
          playNote(pacManMelody[currentNote], pacManMelody[currentNote + 1]);
        }

        currentNote += 2;
        lastChangedBetweenNotes = millis();
      }
    }

    // playing a sound when the player meets and enemy
    void playMeetEnemy() {
      playNote(NOTE_B5, 16);
      playNote(NOTE_B5, 16);
      playNote(NOTE_FS5, 16);
    }

    // playing a sound when the player eat a food item
    void playEatFoodItem() {
      playNote(NOTE_DS3, 16);
      playNote(NOTE_C3, 16);
      playNote(NOTE_C3, 16);
    }
};
