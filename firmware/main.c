#include <avr/io.h>
#include <util/delay.h>


#define LED_ON()  (PORTB |= (1<<PB1))
#define LED_OFF() (PORTB &= ~(1<<PB1))
int main(void)
{

	DDRB |= (1 << PB1);

	LED_OFF();

	while(1)
		LED_ON();
		_delay_ms(500);
		LED_OFF();
		_delay_ms(500);
		LED_ON();
		_delay_ms(1000);
		LED_OFF();
		_delay_ms(1000);
	}

	return 0;
}
