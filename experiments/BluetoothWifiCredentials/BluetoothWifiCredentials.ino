/*
 Name:		BluetoothWifiCredentials.ino
 Created:	9/23/2021 7:57:36 AM
 Author:	c_sim
*/

#include <WiFi.h>
#include "BluetoothSerial.h"
#include "nvs.h"
#include "nvs_flash.h"

BluetoothSerial SerialBT;


//per the spec, the maximum length of the ssid is 32 and the maximum password is 63 (plus nulls?)
String ssid = "";
String password = "";
String machineName = "RobotCamera"; //This is here because we might have multiple cameras (e.g.) but not currently settable

int bluetoothConnected = 0; //flag set when a connection is established

nvs_handle my_handle; //handle to readwrite nvs  YOU MUST READ FIRST to set this global handle

//this callback is called with the bluetooth changes state
void callback(esp_spp_cb_event_t event, esp_spp_cb_param_t* param) {
	if (event == ESP_SPP_SRV_OPEN_EVT) {
		Serial.println("Bluetooth connected");
		bluetoothConnected = 1;
	}
	if (event == ESP_SPP_CLOSE_EVT) {
		bluetoothConnected = 0;
		Serial.println("Bluetooth disconnected");
	}
}

//get the wifi connection info from bluetooth
void getWifiInfoFromBt() {
	SerialBT.printf("Connected to %s \n", machineName.c_str());

	if (ssid != "" && WiFi.status() == WL_IDLE_STATUS || WiFi.status() == WL_CONNECTED) {
		SerialBT.printf("Currently connected to wifi: %s\n", ssid.c_str());
		SerialBT.print("IP Address: ");
		SerialBT.println(WiFi.localIP());
	}

	//scan for available networks
	int n = WiFi.scanNetworks();
	if (n == 0) {
		SerialBT.println("no networks found");
	}
	else {
		SerialBT.print(n);
		SerialBT.println(" networks found");
		for (int i = 0; i < n; ++i) {
			// Print SSID for each network found
			SerialBT.printf("%i: %s\n", i, WiFi.SSID(i));
		}
		SerialBT.println("Wifi #:");
		while (!SerialBT.available()) {
			if (bluetoothConnected == 0) return; //give up on loss of bt connection
			delay(100);
		}
		String ssidString = SerialBT.readString();
		int n = ssidString.toInt();
		ssid = WiFi.SSID(n);
		SerialBT.println("Wifi password:");
		while (!SerialBT.available()) {
			if (bluetoothConnected == 0) return; //give up on loss of bt connection
			delay(100);
		}
		password = SerialBT.readString();
		password.trim();

		if (connectToWiFi(ssid, password) == WL_CONNECTED) {
			SerialBT.println("Sucessfully connected to WiFi.");
			SerialBT.print("IP Address: ");
			SerialBT.println(WiFi.localIP());
			writeCredentialsToNvs(ssid, password);
		}
		else {
			SerialBT.println("Connection failed");
		}
	}
}
wl_status_t  connectToWiFi(String ssid, String password) {

	Serial.printf("Attempting to connect wifi: (%s, %s)\n", ssid.c_str(), password.c_str());

	//attempt the wifi connection
	WiFi.mode(WIFI_STA);
	WiFi.begin(ssid.c_str(), password.c_str()); //this begins the connection attempt

	//now loop until status shows success or failure
	wl_status_t status;
	for (int i = 0; i < 10; i++) {
		status = WiFi.status();
		if (status != WL_DISCONNECTED && status != WL_NO_SHIELD) break;
		Serial.print(WiFi.status());
		SerialBT.print('.');
		delay(1000);
	}
	//done or timed out (10 sec.)
	Serial.print("Connection result:");
	Serial.println(status);
	if (status == WL_CONNECTED) {
		Serial.print("IP Address: ");
		Serial.println(WiFi.localIP());
	}

	return status;
}

void writeCredentialsToNvs(String ssid, String password) {
	Serial.println("Writing ssid/pwd to nvs");
	esp_err_t err = nvs_set_str(my_handle, "SSID", ssid.c_str());
	if (err == ESP_OK) {
		err = nvs_set_str(my_handle, "PWD", password.c_str());
		if (err == ESP_OK) {
			err = nvs_commit(my_handle);
			if (err == ESP_OK) {
				Serial.println("ssid/pwd written successfully");
			}
			else {
				Serial.printf("Error (%s) committing NVS handle!\n", esp_err_to_name(err));
			}
		}
		else {
			Serial.printf("Error (%s) writing PWD NVS handle!\n", esp_err_to_name(err));
		}
	}
	else {
		Serial.printf("Error (%s) writing SSID NVS handle!\n", esp_err_to_name(err));
	}
}

void readCredentialsFromNvs(String &ssid, String &passwword ) {
	//if there are wifi credentials stored in the nvs, use them to connect
	//in the following line, "Camera" is a namespace.  Set it to whatever so long as it doesn't collide 
	//with other apps on this machine which might use the NVS
	esp_err_t err = nvs_open("Camera", NVS_READWRITE, &my_handle);
	if (err != ESP_OK) {
		Serial.printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
	}
	else {
		char tempVal[100]; //this is so we can use Strings while getting info from char* interfaces
		size_t maxLength = 100;
		err = nvs_get_str(my_handle, "SSID", tempVal, &maxLength);
		if (err == ESP_OK) {
			ssid = String(tempVal);
			maxLength = 100;
			err = nvs_get_str(my_handle, "PWD", tempVal, &maxLength);
			if (err == ESP_OK) {
				password = String(tempVal);
			}
			else {
				Serial.printf("Error (%s) reading PWD !\n", esp_err_to_name(err));
			}
		}
		else {
			Serial.printf("Error (%s) reading SSID !\n", esp_err_to_name(err));
		}
	}
}


void setup() {
	Serial.begin(115200);
	SerialBT.begin(machineName); //Bluetooth device name
	Serial.println("The device started, connect via bluetooth to set/modify wifi credentials");
	SerialBT.register_callback(callback);

	readCredentialsFromNvs(ssid, password);
	if (ssid != "")
		connectToWiFi(ssid, password); //try to connect with whatever credentials...if wrong this will fail quietly
}

void loop() {
	if (bluetoothConnected == 1) //is there a new bluetooth connection
	{
		bluetoothConnected = 2;
		getWifiInfoFromBt();
	}

	//some test stuff
	if (Serial.available()) {
		SerialBT.write(Serial.read());
	}
	if (SerialBT.available()) {
		Serial.write(SerialBT.read());
	}
	delay(20);
}
