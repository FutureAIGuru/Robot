
#ifndef CREDENTIALS_H_
#define CREDENTIALS_H_


#include <WiFi.h>
#include "BluetoothSerial.h"
#include "nvs.h"
#include "nvs_flash.h"

extern String ssid;
extern String password;
extern String machineName;
extern int bluetoothConnected;

void BTInit();

wl_status_t  connectToWiFi(String, String);
void readCredentialsFromNvs(String&, String&);
void writeCredentialsToNvs(String, String);
bool getWifiInfoFromBt();

#endif

