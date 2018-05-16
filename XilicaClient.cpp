// 
// 
// 

#include "XilicaClient.h"

bool socketOpen = false;

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
	0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xE
};
IPAddress ip(192, 168, 1, 19);

// Enter the IP address of the server you're connecting to:
IPAddress server(192, 168, 1, 28);

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 23 is default for telnet;
// if you're using Processing's ChatServer, use port 10002):
EthernetClient client;

XilicaClient::XilicaClient()
{
	init();
}

void XilicaClient::init()
{
	// start the Ethernet connection:
	Ethernet.begin(mac, ip);
	
	// give the Ethernet shield a second to initialize:
	delay(1000);
	Serial.println(Ethernet.localIP());
	Serial.print("connecting to: ");
	Serial.println(server);
	// if you get a connection, report back via serial:
	if (client.connect(server, 10007)) {
		Serial.println("connected");
	}
	else {
		// if you didn't get a connection to the server:
		Serial.println("connection failed");

	}
}

void XilicaClient::clientLoop()
{
	if (client.connected()) {
		socketOpen = true;
		if (client.available()) {
			while (client.available()) {
				Serial.print((char)client.read());
			}
			Serial.println("");
		}
		
	}
	else if (socketOpen) {
		Serial.println("lost connection");
		client.stop();
		socketOpen = false;
	}
}

void XilicaClient::checkConnected()
{
	if (!client.connected()) {
		if (socketOpen) client.stop();

		if (client.connect(server, 10007)) {
			Serial.println("connected");
		}
		else {
			// if you didn't get a connection to the server:
			Serial.println("connection failed");
			delay(10000);
			return;
		}
	}
}

String XilicaClient::sendCommand(String command)
{
	checkConnected();
	String buffer = "";
	client.print(command.c_str());
	client.print('\r');
	Serial.println(command.c_str());
	// if there are incoming bytes available
	// from the server, read them and print them:
	
	return buffer;
}
