#include <stdio.h>
#include "NUC100Series.h"
#include "SYS_init.h"
#include "MCU_init.h"
#include "LCD.h"
#include "Scankey.h"

#define MODE_INPUT       0
#define MODE_CHANGE_PIN  1

#define PIN_LENGTH       6

uint8_t state=0;
uint8_t mode=MODE_INPUT;
char PINInput[PIN_LENGTH+1] = ""; //6 student ID + 1 end of string
char PIN[PIN_LENGTH+1] = "740961"; //secret key

void handlePinCorrect (void) {
    print_Line(1, "");
    print_Line(2, "Accepted! Opening door");
    //reset state
    state=0;
}

void handlePinIncorrect(void) {
    print_Line(1, "");
    print_Line(2, "PIN incorrect!");
    //reset state
    state=0;
}

void handleChangePINPress(void) {
    print_Line(1, "");
    print_Line(2, "New PIN");
    //reset state
    state=0;
    //go into change pin mode
    mode=MODE_CHANGE_PIN;
}

void handleChangePIN(void) {
    print_Line(1, "");
    print_Line(2, "Changed!");
    //change PIN
    strcpy(PIN, PINInput);
    //reset state
    state=0;
    //back to input mode
    mode=MODE_INPUT;
}

int main(void)
{
    /* key input */
	uint8_t keyin;
    /* system init */
	SYS_Init();
    /* lcd init */
	init_LCD();
	clear_LCD();
	/* keypad init */
	OpenKeyPad();
    /* init button */
    GPIO_SetMode(PB, BIT15, GPIO_PMD_INPUT);

	print_Line(0,"Door Lock System");
	 
	while(1)
	{
        if(PB15 == 0) { //init button press change PIN
            handleChangePINPress();
            CLK_SysTickDelay(2000);
        }
	    keyin = ScanKey(); 	     // scan keypad to input
        if(keyin == 0) {
            /* do nothing */
            continue;
        }
        sprintf(PINInput+state, "%d\0", keyin);	//copy to string buffer display
		print_Line(1, PINInput);  // display string on LCD
        if(state == PIN_LENGTH-1) { //enough
            if(mode == MODE_INPUT) {
                /* INPUT PIN mode */
                if(strcmp(PINInput, PIN) == 0) {
                    //PIN correct
                    handlePinCorrect();
                }
                else {
                    //PIN incorrect
                    handlePinIncorrect();
                }
            }
            else {
                /* change PIN mode */
                handleChangePIN();
            }
        } else {
		    CLK_SysTickDelay(2000);
        }
        /* increase state */
        state++; 																	 
	}
}