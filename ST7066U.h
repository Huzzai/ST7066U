#ifndef ST7066U_H_
#define ST7066U_H_

#include <Arduino.h>

/**
 *  Initialize ST7066U for interface with Arduino
 */
void init_st7066u();

/**
 *  Write to display when it is in 2-line mode
 */
void write_to_display(uint8_t* line_1, uint8_t* line_2);

/**
 * Sends a command byte to the driver
 */
void command(uint8_t byte);

/**
 * Write to internal RAM
 */
void write(uint8_t byte);

/**
 * Read from RAM
 */
uint8_t read();

/**
 *  Clear display
 */
void clear_display();

/**
 *  Return home
 */
void return_home();

/**
 *  Entry Mode Set
 */
void entry_mode_set(int inc_dec, int shift);

/**
 *  Display On/Off
 */
void display_on_off(int display, int cursor, int blink);

/**
 *  Cursor or Display Shift
 */
void cursor_or_display_shift(int shift_cursor, int dir);

/**
 *  Function Set
 */
void function_set(int DL, int N, int F);

/**
 *  Set CGRAM Address to AC
 */
void set_cgram_address(uint8_t addr);

/**
 *  Set DDRAM Address to AC
 */
void set_ddram_address(uint8_t addr);

#endif /* ST7066U_H_ */
