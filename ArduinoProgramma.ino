#include <EthernetUdp3.h>
#include <EthernetServer.h>
#include <EthernetClient.h>
#include <Ethernet3.h>
#include <Dns.h>
#include <Dhcp.h>
#include "XilicaClient.h"
#include "FastLED.h" // FastLED library
 
// begin rotary / LED init

// define LED count
#define NUM_LEDS 8

// define LEDS pins
#define VOLUME_LEDS_SPRAAK_PIN 8
#define VOLUME_LEDS_MUZIEK_PIN 9

// define rotaries pins
#define PIN_CLK_SPRAAK 33
#define PIN_DT_SPRAAK 35
#define PIN_CLK_MUZIEK 32
#define PIN_DT_MUZIEK 34

// define IDLE time rotaries
int idleMillis = 80;

long lastMillisSpraak;
long lastMillisMuziek;

int encoderPosCountSpraak = 0;
int encoderPosCountMuziek = 0;

int ledOnCountSpraak = 0;
int ledOnCountMuziek = 0;

// last PIN_CLK position
int pinClkLastValSpraak;
int pinClkLastValMuziek;

int clkValSpraak;
int clkValMuziek;

int lastEncoderPosCountSpraak = 0;
int lastEncoderPosCountMuziek = 0;

// Clockwise? true/false
boolean pinDtClockwiseSpraak;
boolean pinDtClockwiseMuziek;

CRGB ledsSpraak[NUM_LEDS];
CRGB ledsMuziek[NUM_LEDS];

XilicaClient *xilicaClient;

// end rotary / LEDs init

// begin button init

int pinButtons[] = { 22,23,24,25,26,27 };
int buttonLedInput[] = { 2,3,4,5,6,7 };

char mutemic[] = "SETRAW mutemic";
char spraakvolume[] = "SET volspraak";
char muziekvolume[] = "SET volmuziek";


int stateButton[] = { LOW,LOW,LOW,LOW,LOW,LOW };
int stateButtonLed[] = { LOW,LOW,LOW,LOW,LOW,LOW };
int previous = LOW;
long time = 0;
long debounce = 200;







void setup() {
	FastLED.addLeds<WS2812B, VOLUME_LEDS_SPRAAK_PIN>(ledsSpraak, NUM_LEDS);
	FastLED.addLeds<WS2812B, VOLUME_LEDS_MUZIEK_PIN>(ledsMuziek, NUM_LEDS);
	FastLED.setBrightness(20);
		
	// set pins Encoder 
	pinMode(PIN_CLK_SPRAAK, INPUT);
	pinMode(PIN_DT_SPRAAK, INPUT);

	pinMode(PIN_CLK_MUZIEK, INPUT);
	pinMode(PIN_DT_MUZIEK, INPUT);

	/* Read PIN_CLK
	Whatever state it's in will reflect the last position
	*/
	pinClkLastValSpraak = digitalRead(PIN_CLK_SPRAAK);
	pinClkLastValMuziek = digitalRead(PIN_CLK_MUZIEK);
	
	////////////////////start animation/////////////////////////

	for (int i = 0; i < NUM_LEDS; i++) {
		// set our current dot to red
		ledsMuziek[i] = CRGB::Blue;
		ledsSpraak[i] = CRGB::Blue;
		FastLED.show();
		// clear our current dot before we move on
		ledsMuziek[i] = CRGB::Blue;
		ledsSpraak[i] = CRGB::Blue;
		delay(100);
		}
		{  
		delay(1000);
		}
	for (int i = NUM_LEDS -1; i >= 0; i--) {
		// set our current dot to red
		ledsMuziek[i] = CRGB::Blue;
		ledsSpraak[i] = CRGB::Blue;
		FastLED.show();
		// clear our current dot before we move on
		ledsMuziek[i] = CRGB::Black;
		ledsSpraak[i] = CRGB::Black;
		delay(100);
	}
	////////////////////end animation/////////////////////////

	Serial.begin (115200);
	FastLED.clear();
	FastLED.setBrightness(20);
	FastLED.show();

	// init buttons and buttonLeds
	for (int i = 0; i < 6; ++i)
	{
		pinMode(pinButtons[i], INPUT);
		pinMode(buttonLedInput[i], OUTPUT);
	}


	xilicaClient = new XilicaClient();
}

void spraakVolumeEvent() {

	if (digitalRead(PIN_DT_SPRAAK) != clkValSpraak) { // Means pin CLK Changed first -We're Rotating Clockwise

		pinDtClockwiseSpraak = true;
				
		if ((encoderPosCountSpraak < 16) && ((millis() - lastMillisSpraak) > idleMillis)) {

			//String command = spraakvolume + String(" +") + String(lastEncoderPosCountSpraak + encoderPosCountSpraak);
			String command = spraakvolume + String(" +") + String(lastEncoderPosCountSpraak = encoderPosCountSpraak);
			Serial.println(xilicaClient->sendCommand(command));

			encoderPosCountSpraak = 

			encoderPosCountSpraak = encoderPosCountSpraak + 1;
			if (encoderPosCountSpraak % 2 == 0) {
				ledOnCountSpraak = ledOnCountSpraak + 1;
			}
		}
	}
	else { // Otherwise PIN_DT changed first and we're moving CCW
		pinDtClockwiseSpraak = false;
		if ((encoderPosCountSpraak > 0) && ((millis() - lastMillisSpraak) > idleMillis)) {


			
			String command = spraakvolume + String(" -") + String(lastEncoderPosCountSpraak = encoderPosCountSpraak);
			//String command = spraakvolume + String(" -") + String(lastEncoderPosCountSpraak + encoderPosCountSpraak);
			Serial.println(xilicaClient->sendCommand(command));


			encoderPosCountSpraak = encoderPosCountSpraak - 1;
						
			if (encoderPosCountSpraak % 2 == 0) {
				ledsSpraak[ledOnCountSpraak - 1] = CRGB::Black;
				ledOnCountSpraak = ledOnCountSpraak - 1;

				//Serial.print("ledOnCount: ");
				//Serial.println(ledOnCount);
			}
			FastLED.show();
		}
	}

	/*if (encoderPosCountSpraak % 4 == 0) {
		delay(5);
	}*/

	//Serial.print("pos: ");
	//Serial.println(encoderPosCount);

	//Serial.print("count: ");
	//Serial.println(ledOnCount);
	//Serial.println(millis());





	for (int i = 0; i < ledOnCountSpraak; i = i + 1) {
		ledsSpraak[i] = CRGB::Blue;

		FastLED.show();
	}

	lastMillisSpraak = millis();
}


void muziekVolumeEvent() {


	if (digitalRead(PIN_DT_MUZIEK) != clkValMuziek) { // Means pin CLK Changed first -We're Rotating Clockwise

		pinDtClockwiseMuziek = true;

		if ((encoderPosCountMuziek < 16) && ((millis() - lastMillisMuziek) > idleMillis)) {

			//xilicaClient->sendCommand("SETRAW volumemuziek+ 1");

			//
			String command = muziekvolume + encoderPosCountMuziek - 1 + String("1");
			Serial.println(xilicaClient->sendCommand(command));

			encoderPosCountMuziek = encoderPosCountMuziek + 1;
			if (encoderPosCountMuziek % 2 == 0) {
				ledOnCountMuziek = ledOnCountMuziek + 1;
			}
		}
	}
	else { // Otherwise PIN_DT changed first and we're moving CCW
		pinDtClockwiseMuziek = false;
		if ((encoderPosCountMuziek > 0) && ((millis() - lastMillisMuziek) > idleMillis)) {

			//xilicaClient->sendCommand("SET volumemuziek- 1");

			String command = muziekvolume + encoderPosCountMuziek -1 + String("+1");
			Serial.println(xilicaClient->sendCommand(command));
			


			encoderPosCountMuziek = encoderPosCountMuziek - 1;



			if (encoderPosCountMuziek % 2 == 0) {
				ledsMuziek[ledOnCountMuziek - 1] = CRGB::Black;
				ledOnCountMuziek = ledOnCountMuziek - 1;

				//Serial.print("ledOnCount: ");
				//Serial.println(ledOnCount);
			}
			FastLED.show();
		}
	}

	/*if (encoderPosCountMuziek % 4 == 0) {
		delay(5);
	}*/

	//Serial.print("pos: ");
	//Serial.println(encoderPosCount);

	//Serial.print("count: ");
	//Serial.println(ledOnCount);
	//Serial.println(millis());



	for (int i = 0; i < ledOnCountMuziek; i = i + 1) {
		ledsMuziek[i] = CRGB::Blue;

		FastLED.show();
	}

	lastMillisMuziek = millis();
}

void buttonsLoop() {
	for (int i = 0; i < 6; ++i)
	{
		stateButton[i] = digitalRead(pinButtons[i]);

		if (stateButton[i] == HIGH && previous == LOW && millis() - time > debounce) {

			if (stateButtonLed[i] == HIGH) {
				if (stateButton[5]) {
					Serial.println(xilicaClient->sendCommand("SETRAW declamatie 0"));
				
				}
				else {
					String command = mutemic + String(i + 1) + String(" 1");
					Serial.println(xilicaClient->sendCommand(command));
				}

				stateButtonLed[i] = LOW;



			}
			else {
				if (stateButton[5]) {
					Serial.println(xilicaClient->sendCommand("SETRAW declamatie 1"));

				}
				else {
					String command = mutemic + String(i + 1) + String(" 0");
					Serial.println(xilicaClient->sendCommand(command));
				}

				stateButtonLed[i] = HIGH;

			}

			time = millis();

		}
		digitalWrite(buttonLedInput[i], stateButtonLed[i]);
		previous == stateButton[i];
	}
}

void loop() {

	buttonsLoop();

	clkValSpraak = digitalRead(PIN_CLK_SPRAAK);
	clkValMuziek = digitalRead(PIN_CLK_MUZIEK);

	if ((clkValSpraak != pinClkLastValSpraak)) {
		spraakVolumeEvent();
		pinClkLastValSpraak = clkValSpraak;
	}

	if ((clkValMuziek != pinClkLastValMuziek)) {
		muziekVolumeEvent();
		pinClkLastValMuziek = clkValMuziek;
	}

	xilicaClient->clientLoop();
}

