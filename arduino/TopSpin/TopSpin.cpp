// Do not remove the include below
#include "TopSpin.h"

TopSpinApp theApp;

//The setup function is called once at startup of the sketch
void setup()
{

#ifdef __DEBUG
	// Initialize hardware serial port
	Serial.begin(115200);
	Serial.println("INITIALIZING");
#endif

	// Initialize the application
	theApp.initialize();

	// start the application
	theApp.start();


#ifdef __DEBUG
	// never get here, but...
	Serial.println("PROGRAM END");
#endif

	// stay here for ever
	while(1);

}

// The loop function is called in an endless loop
void loop()
{
}





