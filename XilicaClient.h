// XilicaClient.h

#ifndef _XILICACLIENT_h
#define _XILICACLIENT_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <SPI.h>
#include <Ethernet3.h>

class XilicaClient
{
 protected:


 public:
	 XilicaClient();
	 void init();
	 void clientLoop();
	 void checkConnected();
	 String sendCommand(String command);
};


#endif

