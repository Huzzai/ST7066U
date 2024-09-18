#include <Arduino.h>
#include "ST7066U.h"

// Pin assignments (you can adjust based on your wiring)
const int RS = 8;
const int RW = 9;
const int EN = 10;
const int D0 = 2;
const int D1 = 3;
const int D2 = 4;
const int D3 = 5;
const int D4 = 6;
const int D5 = 7;
const int D6 = 8;
const int D7 = 9;

/**
 *  Write to display when it is in 2-line mode
 */
void write_to_display(uint8_t* line_1, uint8_t* line_2) {
    command(0x80);  // Set DDRAM address

    for (int i = 0; i < 8; i++) {
        write(*line_1++);
        delayMicroseconds(160);  // Delay 160us
    }

    command(0xC0);  // Set DDRAM address for 2nd line

    for (int i = 0; i < 8; i++) {
        write(*line_2++);
        delayMicroseconds(160);  // Delay 160us
    }
}

/**
 * Sends a command byte to the driver
 */
void command(uint8_t byte) {
    // Set data lines
    for (int i = 0; i < 8; i++) {
        digitalWrite(D0 + i, (byte >> i) & 0x01);
    }

    // RS=0 -> Send Instruction
    digitalWrite(RS, LOW);
    // RW=0 -> Write
    digitalWrite(RW, LOW);

    // Enable pulse
    digitalWrite(EN, HIGH);
    delayMicroseconds(1);  // >= 1us delay
    digitalWrite(EN, LOW);
}

/**
 * Initialize ST7066U for interface with Arduino
 */
void init_st7066u() {
    // Set pin modes
    pinMode(RS, OUTPUT);
    pinMode(RW, OUTPUT);
    pinMode(EN, OUTPUT);
    for (int i = 0; i < 8; i++) {
        pinMode(D0 + i, OUTPUT);
    }

    // E=0 -> Enable bit set to 0
    digitalWrite(EN, LOW);
    delay(40);  // Delay >40ms

    // Wake up (Function set DL=1; 8 bits)
    command(0x30);
    delay(5);  // Delay 5ms
    command(0x30);
    delayMicroseconds(160);
    command(0x30);
    delayMicroseconds(160);

    // Function set: 8-bit/2-line/5x8 chars
    command(0x38);
    delayMicroseconds(160);

    // Set cursor (C/L)
    command(0x10);
    delayMicroseconds(160);

    // Display ON; Cursor ON
    command(0x0F);
    delayMicroseconds(160);

    // Entry mode set
    command(0x06);
}

/**
 * Write to internal RAM
 */
void write(uint8_t byte) {
    // Set data lines
    for (int i = 0; i < 8; i++) {
        digitalWrite(D0 + i, (byte >> i) & 0x01);
    }

    // RS=1 -> Data
    digitalWrite(RS, HIGH);
    // RW=0 -> Write
    digitalWrite(RW, LOW);

    // Enable pulse
    digitalWrite(EN, HIGH);
    delayMicroseconds(1);  // >= 1us delay
    digitalWrite(EN, LOW);
}

/**
 * Read from RAM
 */
uint8_t read() {
    // Set data lines as input
    for (int i = 0; i < 8; i++) {
        pinMode(D0 + i, INPUT);
    }

    // RS=1 -> Data, RW=1 -> Read
    digitalWrite(RS, HIGH);
    digitalWrite(RW, HIGH);

    // Enable pulse
    digitalWrite(EN, HIGH);
    delayMicroseconds(1);  // >= 1us delay
    uint8_t byte = 0;
    for (int i = 0; i < 8; i++) {
        byte |= (digitalRead(D0 + i) << i);
    }
    digitalWrite(EN, LOW);

    return byte;
}

/**
 *  Clear display
 */
void clear_display() {
    command(0x01);
}

/**
 *  Return home
 */
void return_home() {
    command(0x02);
}

/**
 *  Entry Mode Set
 */
void entry_mode_set(int inc_dec, int shift) {
    command(0x04 | (inc_dec << 1) | shift);
}

/**
 *  Display On/Off
 */
void display_on_off(int display, int cursor, int blink) {
    command(0x08 | (display << 2) | (cursor << 1) | blink);
}

/**
 *  Cursor or Display Shift
 */
void cursor_or_display_shift(int shift_cursor, int dir) {
    command(0x10 | (shift_cursor << 3) | (dir << 2));
}

/**
 *  Function Set
 */
void function_set(int DL, int N, int F) {
    command(0x20 | (DL << 4) | (N << 3) | (F << 2));
}

/**
 *  Set CGRAM Address to AC
 */
void set_cgram_address(uint8_t addr) {
    command(0x40 | (addr & 0x3F));
}

/**
 *  Set DDRAM Address to AC
 */
void set_ddram_address(uint8_t addr) {
    command(0x80 | (addr & 0x7F));
}
