#define F_CPU 1000000UL // Specifying 1MHz as CPU frequency. Chec your fuse bit.

#include <avr/io.h>
#include <util/delay.h>

/* PIN DEFINITION */

// NEGATIVE LOGIC
#define BTN_SOURCE PB0
#define BTN_SINK   PD7

// ON when HIGH
#define SOURCE_RESET PC5 // source 0
#define SOURCE_SET   PC4 // source 1
#define SINK_RESET   PC3 // sink 0
#define SINK_SET     PC2 // sink 1
/* END PIN DEFINITION */

/* PIN CONTROL MACRO */
#define sbi(PORT, BIT) PORT |= _BV(BIT)
#define cbi(PORT, BIT) PORT &= ~_BV(BIT)
/* END PIN CONTROL MACRO */

/* PARAMS */
#define RELAY_MS 30
/* END PARAMS */

void select_source_0(void);
void select_source_1(void);
void select_sink_0(void);
void select_sink_1(void);
void init(void) {
  // DATA DIRECTION.
  DDRB = 0b11111110; // B0 is input, others are output mode
  DDRC = 0b11111111; // PORTC is all output
  DDRD = 0b01111111; // D7 is input, others are output mode

  // initial status: select source 0, sink 0
  select_source_0();
  select_sink_1(); //test

  // initial delay detecting reset
  _delay_ms(1000);
}

void select_source_0(void) {
  sbi(PORTC, SOURCE_RESET);
  _delay_ms(RELAY_MS);
  cbi(PORTC, SOURCE_RESET);
}

void select_source_1(void) {
  sbi(PORTC, SOURCE_SET);
  _delay_ms(RELAY_MS);
  cbi(PORTC, SOURCE_SET);
}

void select_sink_0(void) {
  sbi(PORTC, SINK_RESET);
  _delay_ms(RELAY_MS);
  cbi(PORTC, SINK_RESET);
}

void select_sink_1(void) {
  sbi(PORTC, SINK_SET);
  _delay_ms(RELAY_MS);
  cbi(PORTC, SINK_SET);
}

// return 1 when pressed
int get_btn_source(void) {
  return ! bit_is_set(PINB, BTN_SOURCE);
}

// return 1 when pressed
int get_btn_sink(void) {
  return ! bit_is_set(PIND, BTN_SINK);
}

int main(void) {
  int source_selection = 0;
  int sink_selection = 0;

  init();

  while (1) {
    if (get_btn_source()) {
      if (source_selection == 0) {
        select_source_1();
        source_selection = 1;
      } else {
        select_source_0();
        source_selection = 0;
      }
    }
    while(get_btn_source()) {}

    if (get_btn_sink()) {
      if (sink_selection == 0) {
        select_sink_1();
        sink_selection = 1;
      } else {
        select_sink_0();
        sink_selection = 0;
      }
    }
    while(get_btn_sink()) {}

    // suppress chattering
    _delay_ms(50);
  }
}
