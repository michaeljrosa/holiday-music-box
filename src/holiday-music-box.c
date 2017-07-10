/**
 * Author: Michael Rosa, Elliot Williams
 * Date: December 2016 
 *
 * Plays music through a speaker and changes lights
 * every time a new note plays
 * Circuit to be placed in a box that turns the 
 * on when opened
 *
 * Modifies code from Elliot William's "avr_music_box" from "Make AVR"
 *
 * See pg. 50 of engineering notebook for more circuit/box info
 */
 
// ------ Preamble ------ //
#include <avr/io.h>
#include <util/delay.h> 

#include "scale16.h"           // music notes

#define      OUTPUT_DDR         DDRB
#define      OUTPUT_PORT        PORTB
#define      SPEAKER            PORTB3
#define      LEDs               PORTB2


#define  SONG_LENGTH  (sizeof(song) / sizeof(uint16_t))

#define  dH    65000      // note lengths
#define   H    43690
#define  dQ    32768
#define   Q    21845
#define   E    10923

void change_LEDs(void)
{
  OUTPUT_PORT ^= (1 << LEDs);
}

void playNote(uint16_t wavelength, uint16_t duration) {
  uint16_t elapsed;
  uint16_t i;
  for (elapsed = 0; elapsed < duration; elapsed += wavelength) {
                     //For loop with variable delay selects the pitch 
    for (i = 0; i < wavelength; i++) {
      _delay_us(1);
    }
    OUTPUT_PORT ^= (1 << SPEAKER);
  }
}

void rest(uint16_t duration) {
  do {
    _delay_us(1);
  } while (--duration);
}

int main(void) 
{
 
  // rests taken out, didn't sound right
  const uint16_t song[] = {
    F6, G6, F6, D6,       // silent night
    F6, G6, F6, D6,       // holy night
    C7, C7, A6,           // all is calm
    Ax6, Ax6, F6, //R,      // all is bright
    
    G6, G6, Ax6, A6, G6,  // 'round yon irgin
    F6, G6, F6, D6, //R,     // mother and child
    G6, G6, Ax6, A6, G6,  // holy infant so
    F6, G6, F6, D6,       // tender and mild
    
    C7, C7, Dx7, C7, A6,  // sleep in heavenly
    Ax6, D7,              // peace
    Ax6, F6, D6, F6, Dx6, C6,  // sleep in heavenly 
    Ax5,                  // peace
  };
  const uint16_t duration[] = {
    dQ, E, Q, dH,
    dQ, E, Q, dH,
    H, Q, dH,
    H, Q, dH, //E,
    
    H, Q, dQ, E, Q,
    dQ, E, Q, dH, //E,
    H, Q, dQ, E, Q,
    dQ, E, Q, dH,
    
    H, Q, dQ, E, Q,
    dH, dH,
    Q, Q, Q, dQ, E, Q,
    dH,
  };
  
  
  // ------ Inits ------ //
  OUTPUT_DDR |= (1 << SPEAKER) | (1 << LEDs);  
  
  // ------ Event loop ------ // 
  uint8_t whichNote = 0;
  while(whichNote < SONG_LENGTH)
  {
    change_LEDs();
    if(song[whichNote] != R) {
      playNote(song[whichNote], duration[whichNote]);  // decreases tempo, couldn't use a larger datatype
      playNote(song[whichNote], duration[whichNote]); 
      playNote(song[whichNote], duration[whichNote]); 
      if(whichNote == SONG_LENGTH - 1)   // make last note longer
        playNote(song[whichNote], Q); 
    } else {
      rest(duration[whichNote]);
    }
    
    if(whichNote < SONG_LENGTH - 1 && song[whichNote + 1] == R) 
      whichNote++;
    else
      rest(1000);
    
    whichNote++;
  }
  
  while(1){change_LEDs(); _delay_ms(1000);};
  
  return 0;
}
