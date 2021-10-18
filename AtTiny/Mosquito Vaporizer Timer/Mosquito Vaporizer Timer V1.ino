/*
Mosquito Vaporizer Timer V1
16 Oct 2021
*/

#define pin_relay PB3
#define ON_TIME 600
#define OFF_TIME 60
#define SHUTDOWN_TIME 28800
#define STATE_ON 1
#define STATE_OFF 2
#define STATE_SHUTDOWN 3

volatile byte state;
volatile int cycle_count = 0;
volatile int master_count = 0;

void setup()
{
  DDRB |= (1 << pin_relay);
  //When pin_relay LOW relay is on
  state = STATE_ON;
}

void loop()
{
 if(state != STATE_SHUTDOWN)  
 {
	cycle_count++;
	master_count++;

	if(state == STATE_ON)
	{
		if(cycle_count == ON_TIME)
		{
		  PORTB |= (1 << pin_relay);
		  state = STATE_OFF;
		  cycle_count = 0;
		}
	}

	if(state == STATE_OFF)
	{
		if(cycle_count == OFF_TIME)
		{
		  PORTB &= ~(1 << pin_relay);
		  state = STATE_ON;
		  cycle_count = 0;
		}
	}

	if(master_count == SHUTDOWN_TIME)
	{
		PORTB |= (1 << pin_relay);
		state = STATE_SHUTDOWN;
	}
 }

  delay(1000);
}