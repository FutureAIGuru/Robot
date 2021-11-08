// 
// 
// 

#include "UDPResponder.h"
#include <esp_WiFi.h>
#include <WiFiUdp.h>

WiFiUDP udp;

const size_t bufferSize = 255;
char buffer[(int)bufferSize];
IPAddress remoteServer;
uint16_t remoteServerPort = 3333;


void WifiPktReceived(void* pkt, wifi_promiscuous_pkt_type_t pktType) {
	checkUDPAndRespond();
}

void SetupUDPResponder() {

	esp_wifi_set_promiscuous(true);
	esp_wifi_set_promiscuous_rx_cb(WifiPktReceived);

	udp.begin(remoteServerPort);
	Serial.println("UDP Initialized");
}

//esp_err_t esp_wifi_set_promiscuous_rx_cb(wifi_promiscuous_cb_t cb);


size_t pollUDPListener() {
	size_t numberOfBytesRead = 0;
	int byteCount = udp.parsePacket();
	//Serial.print(byteCount);
	if (byteCount > 0) {
		numberOfBytesRead = udp.read(buffer, bufferSize);
		remoteServer = udp.remoteIP();
	}
	return numberOfBytesRead;
}

void checkUDPAndRespond() {
	if (pollUDPListener() != 0) {
		if (strcmp(buffer, "DevicePoll") == 0) {
			//Serial.println("Poll Request Detected Sending response to: ");
			//Serial.println(remoteServer);
			//Serial.printf("b %i ", millis());
			udp.beginPacket(remoteServer, remoteServerPort);
			char* outMessage = "Camera";
			udp.write((uint8_t*)outMessage, strlen(outMessage));
			udp.endPacket();
			//Serial.printf("c %i ",millis());
		}
	}
}



