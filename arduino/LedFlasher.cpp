/**
 * TODO save last setting - power on to last setting
 * TODO implement gyro
 */

// Do not remove the include below
#include "LedFlasher.h"

#define MODE_ALTERNATE_1	 		1
#define MODE_ALTERNATE_1_FADE		2

#define MODE_ALTERNATE_2			3
#define MODE_ALTERNATE_3			4

#define MODE_BLINK_1_SLOW			5
#define MODE_BLINK_1_SLOW_FADE		6
#define MODE_BLINK_1_SHORT			7
#define MODE_BLINK_2_SHORT			8
#define MODE_BLINK_3_SHORT			9

#define MODE_SEQUENCE_3				10 // Cycles through 1-3 outputs
#define MODE_SEQUENCE_4				11 // Cycles through 1-4 outputs
#define MODE_SEQUENCE_5				12 // Cycles through 1-5 outputs
#define MODE_SEQUENCE_6				13 // Cycles through 1-6 outputs

#define MODE_SEQUENCE_3_FAST		14 // Cycles through 1-3 outputs
#define MODE_SEQUENCE_4_FAST		15 // Cycles through 1-4 outputs
#define MODE_SEQUENCE_5_FAST		16 // Cycles through 1-5 outputs
#define MODE_SEQUENCE_6_FAST		17 // Cycles through 1-6 outputs

#define MODE_SEQUENCE_3_FADE		18 // Fades through 1-3 outputs
#define MODE_SEQUENCE_4_FADE		19 // Fades through 1-4 outputs
#define MODE_SEQUENCE_5_FADE		20 // Fades through 1-5 outputs
#define MODE_SEQUENCE_6_FADE		21 // Fades through 1-5 outputs

#define MODE_SEQUENCE_3_BOUNCE		22 // Cycles through 1-3, then 3-1
#define MODE_SEQUENCE_4_BOUNCE		23 // Cycles through 1-4, then 4-1
#define MODE_SEQUENCE_5_BOUNCE		24 // Cycles through 1-5, then 5-1
#define MODE_SEQUENCE_6_BOUNCE		25 // Cycles through 1-6, then 6-1

#define MODE_SEQUENCE_3_BOUNCE_FAST	26 // Cycles through 1-3, then 3-1
#define MODE_SEQUENCE_4_BOUNCE_FAST	27 // Cycles through 1-4, then 4-1
#define MODE_SEQUENCE_5_BOUNCE_FAST	28 // Cycles through 1-5, then 5-1
#define MODE_SEQUENCE_6_BOUNCE_FAST	29 // Cycles through 1-6, then 6-1

#define MODE_SEQUENCE_3_BOUNCE_FADE	30 // Fades through 1-3, then 3-1
#define MODE_SEQUENCE_4_BOUNCE_FADE	31 // Fades through 1-4, then 4-1
#define MODE_SEQUENCE_5_BOUNCE_FADE	32 // Fades through 1-5, then 5-1
#define MODE_SEQUENCE_6_BOUNCE_FADE	33 // Fades through 1-6, then 6-1


#define MODE_BEACON					34 // old school rotating beacon
#define MODE_BEACON_FAST			35 // old school rotating beacon
#define MODE_BEACON_SLOW			36 // slow beacon

#define MODE_BEACON2				37 // old school rotating beacon
#define MODE_BEACON2_FAST			38 // old school rotating beacon

#define MODE_STOP_LIGHT				39 // red/yellow/green stop light

#define MODE_GYRO					50 // gyro light

#define MODE_FIRST					MODE_ALTERNATE_1
#define MODE_LAST					MODE_STOP_LIGHT

#define NUM_OUTPUTS 6
#define NUM_INPUTS 1

#define INTERRUPT_SOURCE	1

#define ON 		255
#define OFF		0

#define UP		1
#define DOWN	0

#define CONFIG_ADDRESS		0x20 // address where last config is saved
#define CONFIG_UNSET		255 // value of config if unset


//TODO will likely need an IFDEF here for HW specific things
uint8_t output_pins[NUM_OUTPUTS] = {3,5,6,9,10,11};
uint8_t max_output_value[NUM_OUTPUTS] = {255,255,255,255,120,120};

uint8_t input_pins[NUM_INPUTS] = {2}; // leonardo INT1 = pin 2

volatile uint8_t mode = MODE_FIRST;
volatile uint8_t modeChanged = false;

volatile unsigned long interruptTime = 0;
uint8_t counter = 0;

void sequence(uint8_t startPin, uint8_t endPin, uint8_t flash, uint16_t onTime, uint16_t offTime, uint16_t breakTime, uint16_t fadeTime);
void beacon(uint8_t pin, uint16_t fadeUpTime, uint16_t onTime, uint16_t fadeDownTime, uint16_t offTime);
void stopLight();

void multi_flash_fade(uint8_t pin, uint8_t repeat, uint16_t onTime, uint16_t offTime, uint16_t fadeTime );
void fade(uint8_t pin, uint16_t fadeTime, uint8_t fadeDirection);
void fade_beacon(uint8_t pin, uint16_t time, uint8_t fadeDirection);

void error();

void changeMode(); // changes the mode
void changeModeISR(); // receives change mode interrupt

void writeOutput(uint8_t pinIndex, uint8_t outputValue);

/**
 * Typical arudino setup routine.  Initializes the HW and SW
 *
 */
void setup()
{
	uint8_t i;

#ifdef __DEBUG
	Serial.begin(115200);
	while (!Serial); // needed for Leonardo only
	Serial.println( "SETUP" );
#endif


	for(i=0; i<NUM_OUTPUTS; i++)
	{
		pinMode(output_pins[i],OUTPUT);
		digitalWrite(output_pins[i], LOW );
	}
	for(i=0; i<NUM_INPUTS; i++)
	{
		pinMode(input_pins[i],INPUT_PULLUP);
	}

	// Interrupt comes from INT1; exact pin is HW specific
	noInterrupts();
	attachInterrupt(INTERRUPT_SOURCE, changeModeISR, RISING);
	interrupts();

	// Set up which mode we start with
	mode = EEPROM.read(CONFIG_ADDRESS);
#ifdef __DEBUG
	Serial.print("Mode EE: ");
	Serial.println( mode, HEX );
#endif

	if( mode < 1 || mode > MODE_LAST )
	{
		mode = MODE_FIRST;
	}

} // end setup


/**
 * Typical arduino loop.  Loops through different modes and executes the one selected.
 *
 */
void loop()
{
	uint8_t i;

	if( modeChanged == true )
	{
		changeMode();
	}

	switch(mode)
	{
	case MODE_ALTERNATE_1: // slow alternating, 600ms
		sequence(0, 2, 1, 600, 0, 0, 0);
		break;
	case MODE_ALTERNATE_1_FADE: // slow alternating with fade, 500us fade up/down
		sequence(0, 2, 1, 300, 0, 0, 500);
		break;
	case MODE_ALTERNATE_2: // double flash, alternating, 60ms on, 30ms off, 200ms break
		sequence(0, 2, 2, 60, 30, 200, 0);
		break;
	case MODE_ALTERNATE_3: // triple flash, alternating, 60ms on, 30ms off, 200ms break
		sequence(0, 2, 3, 60, 30, 200, 0);
		break;
	case MODE_BLINK_1_SLOW: // single flash, 500ms on, 2secs off
		multi_flash_fade(0, 1, 500, 2000, 0);
		break;
	case MODE_BLINK_1_SLOW_FADE:
		multi_flash_fade(0, 1, 500, 2500, 750); // single flash, 500ms on, 2.5secs off, ramp 750us
		break;
	case MODE_BLINK_1_SHORT: // single flash, 75ms on, 2secs off
		multi_flash_fade(0, 1, 75, 2000, 0);
		break;
	case MODE_BLINK_2_SHORT: // double flash, 60ms on, 35ms off, 2secs break
		multi_flash_fade(0, 2, 60, 35, 0);
		delay(2000);
		break;
	case MODE_BLINK_3_SHORT: // triple flash, 60ms on, 35ms off, 2secs break
		multi_flash_fade(0, 3, 60, 35, 0);
		delay(2000);
		break;

	// Chase from 1-n
	case MODE_SEQUENCE_3:
		sequence(0, 3, 1, 150, 0, 0, 0); // 150ms on, 0 secs off
		break;
	case MODE_SEQUENCE_4:
		sequence(0, 4, 1, 150, 0, 0, 0); // 150ms on, 0 secs off
		break;
	case MODE_SEQUENCE_5:
		sequence(0, 5, 1, 150, 0, 0, 0); // 150ms on, 0 secs off
		break;
	case MODE_SEQUENCE_6:
		sequence(0, 6, 1, 150, 0, 0, 0); // 150ms on, 0 secs off
		break;

	// Chase from 1-n fast
	case MODE_SEQUENCE_3_FAST:
		sequence(0, 3, 1, 75, 0, 0, 0); // 75ms on, 0 secs off
		break;
	case MODE_SEQUENCE_4_FAST:
		sequence(0, 4, 1, 75, 0, 0, 0); // 75ms on, 0 secs off
		break;
	case MODE_SEQUENCE_5_FAST:
		sequence(0, 5, 1, 75, 0, 0, 0); // 75ms on, 0 secs off
		break;
	case MODE_SEQUENCE_6_FAST:
		sequence(0, 6, 1, 75, 0, 0, 0); // 75ms on, 0 secs off
		break;

	// Chase from 1-n w/ fade
	case MODE_SEQUENCE_3_FADE:
		sequence(0, 3, 1, 75, 0, 0, 250); // 75ms on, 0 secs off, 250us fade up/down
		break;
	case MODE_SEQUENCE_4_FADE:
		sequence(0, 4, 1, 75, 0, 0, 250); // 75ms on, 0 secs off, 250us fade up/down
		break;
	case MODE_SEQUENCE_5_FADE:
		sequence(0, 5, 1, 75, 0, 0, 250); // 75ms on, 0 secs off, 250us fade up/down
		break;
	case MODE_SEQUENCE_6_FADE:
		sequence(0, 6, 1, 75, 0, 0, 250); // 75ms on, 0 secs off, 250us fade up/down
		break;

	// Bounce 1-n, n-1
	case MODE_SEQUENCE_3_BOUNCE:
		sequence(0, 3, 1, 150, 0, 0, 0); // 150ms on, 0 secs off
		sequence(1, 0, 1, 150, 0, 0, 0); // 150ms on, 0 secs off
		break;
	case MODE_SEQUENCE_4_BOUNCE:
		sequence(0, 4, 1, 150, 0, 0, 0); // 150ms on, 0 secs off
		sequence(2, 0, 1, 150, 0, 0, 0); // 150ms on, 0 secs off
		break;
	case MODE_SEQUENCE_5_BOUNCE:
		sequence(0, 5, 1, 150, 0, 0, 0); // 150ms on, 0 secs off
		sequence(3, 0, 1, 150, 0, 0, 0); // 150ms on, 0 secs off
		break;
	case MODE_SEQUENCE_6_BOUNCE:
		sequence(0, 6, 1, 150, 0, 0, 0); // 150ms on, 0 secs off
		sequence(4, 0, 1, 150, 0, 0, 0); // 150ms on, 0 secs off
		break;

	// Bounce 1-n, n-1 fast
	case MODE_SEQUENCE_3_BOUNCE_FAST:
		sequence(0, 3, 1, 75, 0, 0, 0); // 75ms on, 0 secs off
		sequence(1, 0, 1, 75, 0, 0, 0); // 75ms on, 0 secs off
		break;
	case MODE_SEQUENCE_4_BOUNCE_FAST:
		sequence(0, 4, 1, 75, 0, 0, 0); // 75ms on, 0 secs off
		sequence(2, 0, 1, 75, 0, 0, 0); // 75ms on, 0 secs off
		break;
	case MODE_SEQUENCE_5_BOUNCE_FAST:
		sequence(0, 5, 1, 75, 0, 0, 0); // 75ms on, 0 secs off
		sequence(3, 0, 1, 75, 0, 0, 0); // 75ms on, 0 secs off
		break;
	case MODE_SEQUENCE_6_BOUNCE_FAST:
		sequence(0, 6, 1, 75, 0, 0, 0); // 75ms on, 0 secs off
		sequence(4, 0, 1, 75, 0, 0, 0); // 75ms on, 0 secs off
		break;

	// Bounce 1-n, n-1, fade
	case MODE_SEQUENCE_3_BOUNCE_FADE:
		sequence(0, 3, 1, 75, 0, 0, 250); // 75ms on, 0 secs off, 250us fade up/down
		sequence(1, 0, 1, 75, 0, 0, 250); // 75ms on, 0 secs off, 250us fade up/down
		break;
	case MODE_SEQUENCE_4_BOUNCE_FADE:
		sequence(0, 4, 1, 75, 0, 0, 250); // 75ms on, 0 secs off, 250us fade up/down
		sequence(2, 0, 1, 75, 0, 0, 250); // 75ms on, 0 secs off, 250us fade up/down
		break;
	case MODE_SEQUENCE_5_BOUNCE_FADE:
		sequence(0, 5, 1, 75, 0, 0, 250); // 75ms on, 0 secs off, 250us fade up/down
		sequence(3, 0, 1, 75, 0, 0, 250); // 75ms on, 0 secs off, 250us fade up/down
		break;
	case MODE_SEQUENCE_6_BOUNCE_FADE:
		sequence(0, 6, 1, 75, 0, 0, 250); // 75ms on, 0 secs off, 250us fade up/down
		sequence(4, 0, 1, 75, 0, 0, 250); // 75ms on, 0 secs off, 250us fade up/down
		break;

	//Beacon
	case MODE_BEACON:
		beacon(4, 800, 100, 100, 500); // 800ms up, flash 100ms, 100ms down, break 500ms
		break;
	case MODE_BEACON_SLOW:
		beacon(4, 1500, 100, 250, 2000); // 1500ms up, flash 100ms, 250ms down, break 2000ms
		break;
	case MODE_BEACON_FAST:
		beacon(4, 700, 100, 100, 200); // 700ms up, flash 100ms, 100ms down, break 200ms
		break;
	case MODE_BEACON2:
		beacon(4, 800, 100, 100, 300); // 800ms up, flash 100ms, 100ms down, break 300ms
		beacon(5, 800, 100, 100, 300); // 800ms up, flash 100ms, 100ms down, break 300ms
		break;
	case MODE_BEACON2_FAST:
		beacon(4, 700, 100, 100, 70); // 700ms up, flash 100ms, 100ms down, break 70ms
		beacon(5, 700, 100, 100, 70); // 700ms up, flash 100ms, 100ms down, break 70ms
		break;
	case MODE_STOP_LIGHT:
		stopLight();
		break;

	case MODE_GYRO:
		error(); // not implemented
		break;
	default:
		error();
	}

} // end loop


/**
 * ISR for changing current operating mode
 */
void changeModeISR()
{
	if( interruptTime > millis() )
	{
		return;
	}
	modeChanged = true;
	interruptTime = millis()+500;

} // end changeMode

/**
 * Changes the current operating mode
 */
void changeMode()
{
	mode++;
	if(mode > MODE_LAST)
	{
		mode=MODE_FIRST;
	}

	// Save mode setting
	EEPROM.write( CONFIG_ADDRESS, mode );
	modeChanged = false;
}

/**
 * Routine to signal something went wrong
 *
 */
void error()
{
	multi_flash_fade(0, 4, 30, 30, 0);

} // end error

/**
 * Writes to outputs; ensures max value is not exceeded
 */
void writeOutput(uint8_t pinIndex, uint8_t outputValue)
{
	if( outputValue > max_output_value[pinIndex] )
	{
		outputValue = max_output_value[pinIndex];
	}
	analogWrite(output_pins[pinIndex], outputValue);
}

/**
 * Writes to outputs without max value check
 */
void writeOutputMax(uint8_t pinIndex, uint8_t outputValue)
{
	analogWrite(output_pins[pinIndex], outputValue);
}

/**
 * simulates a rotating beacon
 * @pin - pin to use for output
 * @fadeTimeUp - time to fade up to low-on value (~60 analog)
 * @onTime - on time for max value (255 analog)
 * @fadeDownTime - time to fade down (should fade very quickly)
 * @offTime - time remaining off
 */
void beacon(uint8_t pin, uint16_t fadeUpTime, uint16_t onTime, uint16_t fadeDownTime, uint16_t offTime)
{
	fade_beacon(pin, fadeUpTime, UP); // fade up
	delay(onTime); // super bright time
	fade_beacon(pin, fadeDownTime, DOWN); // fade down
	delay(offTime); // off time
}

/**
 * Simulates a ordinary stop light; green and red are artificially short
 */
void stopLight()
{
	// Green
	multi_flash_fade(0, 1, 10000, 25, 0); // max is 32K seconds
	// Yellow
	multi_flash_fade(1, 1, 3000, 25, 0);
	// Red
	multi_flash_fade(2, 1, 10000, 25, 0);
}

/**
 * Sequences through outputs one by one
 *
 * @startPin - starting output pin
 * @endPin - ending output pin (real pin is endPin-1)
 * @flash - number of times to flash output
 * @onTime - time on
 * @offTime - time off between flashes
 * @breakTime - time off between sequences
 * @fadeTime -time to fade up and down
 */
void sequence(uint8_t startPin, uint8_t endPin, uint8_t flash, uint16_t onTime, uint16_t offTime, uint16_t breakTime, uint16_t fadeTime)
{
	uint8_t i;

	// multi_flash automatically holds for offtime at the end.  As such, breaktime is
	// really (breaktime + offtime).  If offtime is large, it will affect the functionality of breaktime by
	// making if longer than desired by the amount (offtime).  So, if offtime > 0, we subtract offtime from breaktime
	// Also, we ensure breakTime > offTime to avoid an unwanted rollover with the unsigned int math.
	if( (offTime > 0) && (breakTime > offTime) )
	{
		breakTime = breakTime - offTime;
	}

	if( startPin < endPin )
	{
		// start -> end
		for(i=startPin; i<endPin; i++)
		{
			multi_flash_fade(i, flash, onTime, offTime, fadeTime);
			delay(breakTime);
		}
	}
	else
	{
		// end -> start
		for(i=startPin; i>endPin; i--)
		{
			multi_flash_fade(i, flash, onTime, offTime, fadeTime); // on once, 100ms, off 0secs, ramp 350us
			delay(breakTime);
		}
	}
}

/**
 * Flashes output with a fade option
 *
 * @pin - pin to fade
 * @repeat - number of times to toggle output
 * @onTime - time on during flash
 * @offTime - time off during flash
 * @fadeTime - time to fade up and down
 */
void multi_flash_fade(uint8_t pin, uint8_t repeat, uint16_t onTime, uint16_t offTime, uint16_t fadeTime )
{
	uint8_t i;

	for(i=0; i<repeat; i++)
	{
		fade(pin, fadeTime, UP);
		delay(onTime);
		fade(pin, fadeTime, DOWN);
		delay(offTime);
	}
}

/**
 * Fades output
 *
 * @pin - pin to modulate
 * @fadeTime - time to fade
 * @fadeDirection - UP or DOWN
 */
void fade(uint8_t pin, uint16_t fadeTime, uint8_t fadeDirection)
{
	uint8_t i;
	if( fadeDirection == UP)
	{
		// Check if we're skipping the fade itself
		if( fadeTime == 0)
		{
			writeOutput(pin, ON);
		}
		else
		{
			for(int i=0; i<255; i++)
			{
				writeOutput(pin, i);
				delayMicroseconds(fadeTime);
			}
			writeOutput(pin, ON); // ensure output if fully on
		}
	}
	else
	{
		// Check if we're skipping fade
		if( fadeTime == 0)
		{
			writeOutput(pin, OFF);
		}
		else
		{
			for(int i=255; i > 0; i--)
			{
				writeOutput(pin, i);
				delayMicroseconds(fadeTime);
			}
			writeOutput(pin, OFF); // ensure output is fully off
		}
	}
}

/**
 * Simulates beacon - uses standard linear fade technique to a "max" low value,
 * then jumps to max analog value.  Simulates slowly increasing intensity with
 * large flash at the end
 *
 * @pin - pin to modulate
 * @time - time to fade
 * @fadeDireciton - UP or DOWN
 */
void fade_beacon(uint8_t pin, uint16_t time, uint8_t fadeDirection)
{
	uint16_t i;
	double value;
	double m, b;

	value = 0;

	if( fadeDirection == UP)
	{
		// y = mx+b;
		i = 0;
		m = .3;
		b = 0;
		value = 0;
		while( (int)value < 255)
		{
			writeOutputMax(pin, (int)value);
			delayMicroseconds(time);

			value = m*i+b;
			i++;

			if( value > 65)
			{
				m = 5;
			}
		}
		writeOutputMax(pin, 255);
	}
	else
	{
		i = 0;
		m = -10;
		b = 255;
		value = 255;

		while( (int)value > 0)
		{
			writeOutputMax(pin, (int)value);
			delayMicroseconds(time);
			value = m*i+b;
			i++;
		}
		writeOutputMax(pin, 0);

	}

} // end fade_beacon


