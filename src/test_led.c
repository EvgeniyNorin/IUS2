#include "aduc812.h"
#include "led.h"
#include "max.h"

static unsigned char display_option = 0;
static unsigned long systime = 0; 

void init_systimer( void ) { 
	TCON = 0xFF; 
	TMOD = 0x01; 
	TH0 = 0xFF; 
	TL0 = 0x67;  
} 

unsigned long get_ms_counter( void ) { 
	unsigned long res; 
	ET0 = 0; 
	res = systime; 
	ET0 = 1; 
	return res; 
} 

unsigned long diff_time_ms( unsigned long t2 ) { 
	return (unsigned long)get_ms_counter() - t2; 
} 

void delay_ms( unsigned long ms ) { 
	unsigned long tl = (unsigned long)get_ms_counter();  
	while ( 1 ) { 
		if (diff_time_ms(tl) > ms) break; 
	} 
} 

void T0_ISR(void) __interrupt(1) {  
	// systime ++; 
	// TH0 = 0xFC;
	// TL0 = 0x67;
	leds(0xff); 
}


void IE0_ISR(void) {
	leds(0x00);
}

void run_cycle(void) {
	// unsigned char i = 0;
	// unsigned char j = 0;
	// while(1) {
	// 	for (j = 0; j < 10; ++j) {
	// 		for (i = 0; i <= 7; ++i) {
	// 			led(i, 7 - i);
	// 			delay_ms(200);
	// 		}	
	// 	}
	// 	break;
	// }

}

void set_vector(unsigned char xdata *address, void * vector) { 
	unsigned char xdata * tmp_vector; 
	*address = 0x02; 
	tmp_vector = (unsigned char xdata *)(address + 1); 
	*tmp_vector = (unsigned char)((unsigned short)vector >> 8); 
	++tmp_vector; 
	*tmp_vector = (unsigned char)vector;  
}

void main( void ) { 
	unsigned char light = 1; 
	unsigned char ena;
	init_systimer(); 
	leds(0xAA);
	delay_ms(500);
	set_vector(0x200B, (void*)T0_ISR); 
	ET0 = 1; 
	EA = 1; 
	ena = read_max(ENA);
	write_max(ENA, ena | 0x20);
	while(1) { 
		if(display_option) {
			leds(get_ms_counter());
		} else {
			run_cycle();
		} 
	} 
}   