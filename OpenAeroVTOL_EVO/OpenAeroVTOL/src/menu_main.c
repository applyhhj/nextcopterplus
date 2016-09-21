//***********************************************************
//* menu_main.c
//***********************************************************

//***********************************************************
//* Includes
//***********************************************************

#include <avr/pgmspace.h> 
#include <avr/io.h>
#include <avr/eeprom.h>
#include <stdbool.h>
#include <util/delay.h>
#include "io_cfg.h"
#include "init.h"
#include "mugui.h"
#include "glcd_menu.h"
#include "menu_ext.h"
#include "glcd_driver.h"
#include "main.h"

//************************************************************
// Prototypes
//************************************************************

// Menu items
void menu_main(void);
void do_main_menu_item(uint8_t menuitem);
void Wait_BUTTON1(void);
void Wait_BUTTON4(void);

//************************************************************
// Defines
//************************************************************

#ifdef ERROR_LOG
#define MAINITEMS 24	// Number of menu items
#else
#define MAINITEMS 23	// Number of menu items
#endif

#define MAINSTART 77	// Start of Menu text items

//************************************************************
// Main menu-specific setup
//************************************************************

uint16_t main_top = MAINSTART;
uint16_t sub_top = 0;
uint8_t menu_flag = 1;

void menu_main(void)
{
	static uint16_t main_cursor = LINE0;	// These are now static so as to remember the main menu position
	static uint16_t main_temp = 0;
	static uint8_t old_menu = 0;

	button = NONE;

	menu_beep(1);

	// Wait until user's finger is off button 1
	Wait_BUTTON1();
	
	while(button != BACK)
	{
		// Clear buffer before each update
		clear_buffer(buffer);	

		// Print menu
		print_menu_frame(BASIC);													// Frame
		
		for (uint16_t i = 0; i < 4; i++)
		{
			LCD_Display_Text(main_top+i,(const unsigned char*)Verdana8,ITEMOFFSET,(uint8_t)pgm_read_byte(&lines[i]));	// Lines
		}

		print_cursor(main_cursor);												// Cursor
		write_buffer(buffer);

		// Poll buttons when idle
		poll_buttons(true);

		// Handle menu changes
		update_menu(MAINITEMS, MAINSTART, 0, button, &main_cursor, &main_top, &main_temp);

		// If main menu item has changed, reset sub-menu positions
		// and flag to sub-menus that positions need to be reset
		if (main_temp != old_menu)
		{
			cursor = LINE0;
			menu_temp = 0;
			old_menu = main_temp;
			menu_flag = 1;
		}

		// If ENTER pressed, jump to menu 
		if (button == ENTER)
		{
			do_main_menu_item(main_temp);
			button = NONE;

			// Wait until user's finger is off button 1
			Wait_BUTTON1();
		}
	}
}

void do_main_menu_item(uint8_t menuitem)
{
	switch(menuitem) 
	{
		case MAINSTART:
			menu_rc_setup(2); 		// 1.General
			break;
		case MAINSTART+1:
			menu_rc_setup(1); 		// 2.RX setup
			break;
		case MAINSTART+2:
			Display_rcinput();		// 3.RX inputs
			break;
		case MAINSTART+3:
			Display_sticks(); 		// 4.Stick polarity
			break;
		case MAINSTART+4:
			Display_sensors();		// 5.Sensor calibration
			break;
		case MAINSTART+5:
			Display_balance();		// 6.Level meter
			break;
		case MAINSTART+6:
			menu_flight(0);			// 7.Flight profile 1
			break;
		case MAINSTART+7:
			menu_flight(1); 		// 8.Flight profile 2
			break;
		case MAINSTART+8:
			menu_curves();			// 9.Curves menu
			break;
		case MAINSTART+9:
			menu_offsets();			// 10.Output offsets
			break;
		case MAINSTART+10:
			menu_mixer(0);			// 11.OUT1 Mixer
			break;
		case MAINSTART+11:
			menu_mixer(1);			// 12.OUT2 Mixer
			break;
		case MAINSTART+12:
			menu_mixer(2);			// 13.OUT3 Mixer
			break;
		case MAINSTART+13:
			menu_mixer(3);			// 14.OUT4 Mixer
			break;
		case MAINSTART+14:
			menu_mixer(4);			// 15.OUT5 Mixer
			break;
		case MAINSTART+15:
			menu_mixer(5);			// 16.OUT6 Mixer
			break;
		case MAINSTART+16:
			menu_mixer(6);			// 17.OUT7 Mixer
			break;
		case MAINSTART+17:
			menu_mixer(7);			// 18.OUT8 Mixer
			break;
		case MAINSTART+18:
			menu_servo_setup(1);	// 19.Servo direction
			break;
		case MAINSTART+19:
			menu_servo_setup(2); 	// 20.Neg. Servo trvl. (%)
			break;
		case MAINSTART+20:
			menu_servo_setup(3); 	// 21.Pos. Servo trvl. (%)
			break;
		case MAINSTART+21:
			menu_channel();			// 22.Custom Ch. order
			break;
		case MAINSTART+22:
			Display_in_out();		// 23.IO menu
			break;
		case MAINSTART+23:
			menu_log();				// 24.Error log
			break;
		default:
			break;	
	} // Switch
}

void Wait_BUTTON1(void)
{
	while(BUTTON1 == 0)
	{
		_delay_ms(50);
	}
}

void Wait_BUTTON4(void)
{
	while(BUTTON4 == 0)
	{
		_delay_ms(50);
	}
}