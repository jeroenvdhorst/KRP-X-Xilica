// 
// 
// 

#include "XilicaClient.h"

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
	Serial.println("connecting...");
	Serial.println(Ethernet.localIP());
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
	if (!client.connected()) {
		// if you get a connection, report back via serial:
		client.stop();
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
	//// if there are incoming bytes available
	//// from the server, read them and print them:
	//if (client.available()) {
	//	char c = client.read();
	//	Serial.print(c);
	//}

	//// as long as there are bytes in the serial queue,
	//// read them and send them out the socket if it's open:
	//while (Serial.available() > 0) {
	//	char inChar = Serial.read();
	//	if (client.connected()) {
	//		client.print(inChar);
	//	}
	//}

	//// if the server's disconnected, stop the client:
	//	if (!client.connected()) {
	//		Serial.println();
	//		Serial.println("disconnecting.");
	//		client.stop();
	//		// do nothing:
	//	}
	while (client.available()) {
		Serial.print((char)client.read());
	}
}

String XilicaClient::sendCommand(String command)
{
	String buffer = "";
	client.print(command.c_str());
	client.print('\r');
	Serial.println(command.c_str());
	// if there are incoming bytes available
	// from the server, read them and print them:
	
	return buffer;
}
