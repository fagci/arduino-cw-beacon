// Arduino 8MHz CW BEACON
// Tested on Arduino Pro Mini

#define ANTENNA_PIN_MASK 2  // Use antenna on 9 PIN (8..13 register)
#define LED_PIN_MASK 32     // 13 pin (PINB5)
#define MORSE_CHAR_START 32

#define DIT_T 80
#define DAH_T 240
#define SPACE_T 560
#define CHAR_DELAY_T 160
#define REPEAT_DELAY 5000

#define SPACE 0b11111111
#define DAH_MASK 0b10000000
#define NA 0b10000000

const char message[] = "CQ CQ CQ DE Arduino 8MHz";

const uint8_t morsechar[] PROGMEM = {
  SPACE,
  NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA,
  0b10000110,  // "-"
  NA,
  0b10010100,  // "/"
  0b11111100,  // "0"
  0b01111100,  // "1"
  0b00111100,  // "2"
  0b00011100,  // "3"
  0b00001100,  // "4"
  0b00000100,  // "5"
  0b10000100,  // "6"
  0b11000100,  // "7"
  0b11100100,  // "8"
  0b11110100,  // "9"
  NA, NA, NA,
  0b10001100,  // "="
  NA,
  0b00110010,  // "?"
  NA,
  0b01100000,  // "A"
  0b10001000,  // "B"
  0b10101000,  // "C"
  0b10010000,  // "D"
  0b01000000,  // "E"
  0b00101000,  // "F"
  0b11010000,  // "G"
  0b00001000,  // "H"
  0b00100000,  // "I"
  0b01111000,  // "J"
  0b10110000,  // "K"
  0b01001000,  // "L"
  0b11100000,  // "M"
  0b10100000,  // "N"
  0b11110000,  // "O"
  0b01101000,  // "P"
  0b11011000,  // "Q"
  0b01010000,  // "R"
  0b00010000,  // "S"
  0b11000000,  // "T"
  0b00110000,  // "U"
  0b00011000,  // "V"
  0b01110000,  // "W"
  0b10011000,  // "X"
  0b10111000,  // "Y"
  0b11001000,  // "Z"
  NA, NA, NA, NA, NA, NA,
  0b01100000,  // "a"
  0b10001000,  // "b"
  0b10101000,  // "c"
  0b10010000,  // "d"
  0b01000000,  // "e"
  0b00101000,  // "f"
  0b11010000,  // "g"
  0b00001000,  // "h"
  0b00100000,  // "i"
  0b01111000,  // "j"
  0b10110000,  // "k"
  0b01001000,  // "l"
  0b11100000,  // "m"
  0b10100000,  // "n"
  0b11110000,  // "o"
  0b01101000,  // "p"
  0b11011000,  // "q"
  0b01010000,  // "r"
  0b00010000,  // "s"
  0b11000000,  // "t"
  0b00110000,  // "u"
  0b00011000,  // "v"
  0b01110000,  // "w"
  0b10011000,  // "x"
  0b10111000,  // "y"
  0b11001000   // "z"
};

void sig(bool state) {
  DDRB = state ? DDRB | ANTENNA_PIN_MASK : DDRB & ~ANTENNA_PIN_MASK;
  PORTB = state ? PORTB | LED_PIN_MASK : PORTB & ~LED_PIN_MASK;  // 13 pin (LED)
}

void send(char c) {
  if (!c) return;
  uint8_t cur_character = pgm_read_byte(&morsechar[c - MORSE_CHAR_START]);

  if (cur_character == SPACE) {
    delay(SPACE_T);
    return;
  }

  for (; cur_character != DAH_MASK; cur_character <<= 1) {
    bool is_dah = (cur_character & DAH_MASK) == DAH_MASK;
    sig(1);
    delay(is_dah ? DAH_T : DIT_T);
    sig(0);
    delay(DIT_T);
  }

  delay(CHAR_DELAY_T);
}

void setup() {
  TCCR1A = 1 << COM1A0;             // Toggle OC1A on Compare Match
  TCCR1B = 1 << WGM12 | 1 << CS10;  // CTC genmode, no prescaling
}

void loop() {
  for (auto c : message) send(c);
  delay(REPEAT_DELAY);
}
