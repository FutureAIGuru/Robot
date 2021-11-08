#include "Credentials.h"

BluetoothSerial SerialBT;

nvs_handle my_handle; //handle to readwrite nvs  YOU MUST READ FIRST to set this global handle

int bluetoothConnected = 0;

String	ssid = "";
String	password = "";
String	machineName = "RobotCamera";


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
bool  getWifiInfoFromBt() {
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
		return false;
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
			if (bluetoothConnected == 0) return false; //give up on loss of bt connection
			delay(100);
		}
		String ssidString = SerialBT.readString();
		int n = ssidString.toInt();
		ssid = WiFi.SSID(n);
		SerialBT.println("Wifi password:");
		while (!SerialBT.available()) {
			if (bluetoothConnected == 0) return false; //give up on loss of bt connection
			delay(100);
		}
		password = SerialBT.readString();
		password.trim();

		if (connectToWiFi(ssid, password) == WL_CONNECTED) {
			SerialBT.println("Sucessfully connected to WiFi.");
			SerialBT.print("IP Address: ");
			SerialBT.println(WiFi.localIP());
			writeCredentialsToNvs(ssid, password);
			return true;
		}
		else {
			SerialBT.println("Connection failed");
			return false;
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
	if (status == WL_CONNECTED || status == WL_IDLE_STATUS) {
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

void readCredentialsFromNvs(String& ssid, String& passwword) {
	
	//if there are wifi credentials stored in the nvs, use them to connect
	//in the following line, "Camera" is a namespace.  Set it to whatever so long as it doesn't collide 
	//with other apps on this machine which might use the NVS 
	ssid = "";
	password = "";
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
//initialize the BlueTooth
void BTInit() {
	SerialBT.begin(machineName); //Bluetooth device name
	Serial.println("The device started, connect via bluetooth to set/modify wifi credentials");
	SerialBT.register_callback(callback);
}

