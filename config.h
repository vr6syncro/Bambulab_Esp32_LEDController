#ifndef CONFIG_H
#define CONFIG_H

const char* firmwareVersion = "V1.1";

int szenario = 1;
bool updateAvailable = false;
bool updateChecked = false;
String latestVersion = "";
bool debug = true;
bool hmsErrorExists = false;
bool hmsErrorPresent = false;
bool previousHmsErrorExists = false;
bool autoPushAllEnabled = true;
bool pushAllCommandSent = false;
bool wifiConnected = false;
bool mqttConnected = false;
int printer_stage = -2;
int printer_real_stage = -2;
bool f_layerInspection = false;
int bigFan1Speed = 0;
int coolingFanSpeed = 0;
int bigFan2Speed = 0;
int chamberTemperature = 0;
int heatbreakFanSpeed = 0;
int nozzleTargetTemperature = 0;
int nozzleTemperature = 0;
int bedTargetTemperature = 0;
int bedTemperature = 0;
String gcodeState;
unsigned int sequence_id = 0;
unsigned long previousMillis = 0;

#define MAX_HMS_ERRORS 10  // Maximum number of HMS errors that can be tracked at once

String full_codes[MAX_HMS_ERRORS];  // Array to store the full HMS codes
String urls[MAX_HMS_ERRORS];        // Array to store the URLs
int hms_error_count = 0;            // Variable to keep track of the number of HMS errors

// mqtt not connected
unsigned long lastAttemptTime = 0;
const unsigned long RECONNECT_INTERVAL = 5000;  // 5 seconds

#endif