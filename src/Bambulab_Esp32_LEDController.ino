#include "welcome.h"
#include "config.h"
#include <Preferences.h>
#include "ws2812b.h"
#include "ledhandle.h"
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <WiFiManager.h>
#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <WebSocketsServer.h>
#include <Update.h>
#include "ota.h"

// include the webpages
#include "startpage.h"
#include "infopage.h"



// Function to get the printer stage description
const char* getPrinterStageDescription(int stage) {
  switch (stage) {
    case -1: return "Idle";
    case 0: return "Printing";
    case 1: return "Auto Bed Leveling";
    case 2: return "Heatbed Preheating";
    case 3: return "Sweeping XY Mech Mode";
    case 4: return "Changing Filament";
    case 5: return "M400 Pause";
    case 6: return "Paused due to filament runout";
    case 7: return "Heating Hotend";
    case 8: return "Calibrating Extrusion";
    case 9: return "Scanning Bed Surface";
    case 10: return "Inspecting First Layer";
    case 11: return "Identifying Build Plate Type";
    case 12: return "Calibrating Micro Lidar";
    case 13: return "Homing Toolhead";
    case 14: return "Cleaning Nozzle Tip";
    case 15: return "Checking Extruder Temperature";
    case 16: return "Printing was paused by the user";
    case 17: return "Pause of front cover falling";
    case 18: return "Calibrating Micro Lidar";
    case 19: return "Calibrating Extrusion Flow";
    case 20: return "Paused due to nozzle temperature malfunction";
    case 21: return "Paused due to heat bed temperature malfunction";
    default: return "Unknown stage";
  }
}

// Math code for HMS Error
String DecimalHexTwosComplement(int decimal) {
  char buffer[9];
  sprintf(buffer, "%08X", decimal);
  return String(buffer);
}

//for printer light led control
String createJsonLedControl(const char* led_node, const char* led_mode, int led_on_time, int led_off_time, int loop_times, int interval_time) {
  DynamicJsonDocument doc(1024);
  doc["system"]["sequence_id"] = sequence_id;
  doc["system"]["command"] = "ledctrl";
  doc["system"]["led_node"] = led_node;
  doc["system"]["led_mode"] = led_mode;
  doc["system"]["led_on_time"] = led_on_time;
  doc["system"]["led_off_time"] = led_off_time;
  doc["system"]["loop_times"] = loop_times;
  doc["system"]["interval_time"] = interval_time;

  String jsonString;
  serializeJson(doc, jsonString);
  return jsonString;
}

// für command
String createJsonCommand(const char* command, const char* param) {
  DynamicJsonDocument doc(1024);
  doc["print"]["sequence_id"] = String(sequence_id);
  doc["print"]["command"] = command;
  doc["print"]["param"] = param;

  String jsonString;
  serializeJson(doc, jsonString);
  return jsonString;
}

// für pushall
String createJsonMessage(const char* messageContent) {
  DynamicJsonDocument doc(1024);
  doc["message"] = messageContent;
  doc["sequence_id"] = sequence_id;

  String jsonString;
  serializeJson(doc, jsonString);
  return jsonString;
}

// MQTT-Server-Informationen
char mqtt_server[40];
char mqtt_port[6] = "8883";
char mqtt_user[30] = "bblp";
char mqtt_password[30];
char serial_number[20];
char mqtt_topic[200];

AsyncWebServer server(80);
WebSocketsServer ws(81);
WiFiClientSecure espClient;
PubSubClient client(espClient);

Preferences preferences;

void loadPreferences();
void savePreferences(WiFiManagerParameter& server, WiFiManagerParameter& port, WiFiManagerParameter& user, WiFiManagerParameter& password, WiFiManagerParameter& serial, WiFiManagerParameter& szenario_param);
void mqttCallback(char* topic, byte* payload, unsigned int length);
void configureSerial();
void sendPushAllCommand();
void ledControl();
void processHms(JsonArray hmsArray);
String askForInput(const String& prompt, const String& currentValue);
void publishMessage(const char* messageContent);
void sendPrintCommand(const char* command, const char* param);
void handleSerialCommands();
void publishPushAllMessage();
void sendGcodeCommand(const char* command, const char* param);
void sendLedControlCommand(const char* ledNode, const char* ledMode, int ledOnTime, int ledOffTime, int loopTimes, int intervalTime);

//save config from web
void saveConfigFromWeb(const String& server, const String& port, const String& user, const String& password, const String& serial, bool debugValue, int szenarioValue) {
  preferences.begin("mqtt_config", false);
  preferences.putString("server", server.c_str());
  preferences.putString("port", port.c_str());
  preferences.putString("user", user.c_str());
  preferences.putString("password", password.c_str());
  preferences.putString("serial", serial.c_str());
  preferences.putBool("debug", debugValue);
  preferences.putString("szenario", String(szenarioValue).c_str());
  preferences.end();
}

// wifimanager external config
void configModeCallback(WiFiManager* myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());

  Serial.println(myWiFiManager->getConfigPortalSSID());
}

// clear hms error
void clearHmsErrorInfo() {
  hms_error_count = 0;
  for (int i = 0; i < MAX_HMS_ERRORS; i++) {
    full_codes[i] = "";
    urls[i] = "";
  }
}

void setup() {
  Serial.begin(115200);
  client.setBufferSize(20000);


  initFastLED();
  setFastLedAllBlack();

  //Serial.println("ESP-IDF version is: " + String(esp_get_idf_version()));

  // Preferences
  loadPreferences();

  // WiFiManager
  WiFiManager wifiManager;
  setWifiConnecting();
  wifiManager.setClass("invert");
  wifiManager.setAPCallback(configModeCallback);

  // Custom parameters for configuration
  WiFiManagerParameter custom_mqtt_server("server", "Printer IP", mqtt_server, 40);
  WiFiManagerParameter custom_mqtt_port("port", "MQTT Port", mqtt_port, 6);
  WiFiManagerParameter custom_mqtt_user("user", "MQTT User", mqtt_user, 30);
  WiFiManagerParameter custom_mqtt_password("password", "Printer Accesscode", mqtt_password, 30);
  WiFiManagerParameter custom_serial_number("serial", "Printer Serial Number", serial_number, 20);
  WiFiManagerParameter custom_szenario("szenario", "Szenario (1=Icon, 2=Logo)", String(szenario).c_str(), 2);

  wifiManager.addParameter(&custom_mqtt_server);
  wifiManager.addParameter(&custom_mqtt_port);
  wifiManager.addParameter(&custom_mqtt_user);
  wifiManager.addParameter(&custom_mqtt_password);
  wifiManager.addParameter(&custom_serial_number);
  wifiManager.addParameter(&custom_szenario);

  wifiManager.setDebugOutput(true);

  if (wifiManager.autoConnect("Bambulab_Esp32_LEDController", "password")) {
    Serial.println("Connected to WiFi.");
    // Start the web server
    server.begin();
  } else {
    Serial.println("Failed to connect to WiFi.");
  }

  // Save updated parameters
  savePreferences(custom_mqtt_server, custom_mqtt_port, custom_mqtt_user, custom_mqtt_password, custom_serial_number, custom_szenario);

  // MQTT-Server-Konfiguration
  client.setServer(mqtt_server, atoi(mqtt_port));
  espClient.setInsecure();
  client.setCallback(mqttCallback);
  sprintf(mqtt_topic, "device/%s/report", serial_number);

  Serial.println("Initial setup complete.");
  Serial.println("Send 'config' to configure settings via serial console.");


  setupOTA();
  printWelcomeMessage();
  delay(3000);
  checkForUpdates();


  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    String startpage_html_content = getStartPage();
    request->send(200, "text/html", startpage_html_content);
  });

  server.on("/info", HTTP_GET, [](AsyncWebServerRequest* request) {
    String infopage_html_content = getInfoPage();
    request->send(200, "text/html", infopage_html_content);
  });



  server.on("/config", HTTP_GET, [](AsyncWebServerRequest* request) {
    // HTML-Formular für zusätzliche Konfigurationen senden
  });

  server.on("/control", HTTP_GET, [](AsyncWebServerRequest* request) {
    // Seite zur Steuerung senden
  });

  ws.begin();
  ws.onEvent([](uint8_t clientNum, WStype_t type, uint8_t* payload, size_t length) {
    switch (type) {
      case WStype_DISCONNECTED:
        if (debug) {
          Serial.printf("[%u] Disconnected!\n", clientNum);
        }
        break;
      case WStype_CONNECTED:
        {
          IPAddress ip = ws.remoteIP(clientNum);
          if (debug) {
            Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", clientNum, ip[0], ip[1], ip[2], ip[3], payload);
          }
        }
        break;
      case WStype_TEXT:
        if (debug) {
          Serial.printf("[%u] Received text: %s\n", clientNum, payload);
        }

        String receivedData = String((char*)payload);
        DynamicJsonDocument doc(1024);
        deserializeJson(doc, receivedData);

        if (doc["type"] == "mqttConfig") {
          String server = doc["server"].as<String>();
          String port = doc["port"].as<String>();
          String user = doc["user"].as<String>();
          String password = doc["password"].as<String>();
          String serial = doc["serial"].as<String>();
          bool debugValue = doc["debug"].as<bool>();
          int szenarioValue = doc["szenario"].as<int>();

          saveConfigFromWeb(server, port, user, password, serial, debugValue, szenarioValue);

          ws.sendTXT(clientNum, "Config saved successfully");

          delay(5000);
          ESP.restart();
        } else if (doc["type"] == "printCommand") {
          String command = doc["command"].as<String>();
          String value = doc["value"].as<String>();
          sendPrintCommand(command.c_str(), value.c_str());
        } else if (doc["type"] == "ledControl") {
          String light = doc["light"].as<String>();
          String state = doc["state"].as<String>();
          int arg1 = doc["arg1"].as<int>();
          int arg2 = doc["arg2"].as<int>();
          int arg3 = doc["arg3"].as<int>();
          int arg4 = doc["arg4"].as<int>();
          sendLedControlCommand(light.c_str(), state.c_str(), arg1, arg2, arg3, arg4);
        } else if (doc["type"] == "gcodeCommand") {
          String command = doc["command"].as<String>();
          String value = doc["value"].as<String>();
          sendGcodeCommand(command.c_str(), value.c_str());
        }
        break;
    }
  });
}

void loop() {
  // Prüfen, ob WiFi verbunden ist
  if (WiFi.status() == WL_CONNECTED) {
    if (!wifiConnected) {
      wifiConnected = true;
      pushAllCommandSent = false;
    }
  } else {
    wifiConnected = false;
  }

  // Prüfen, ob MQTT verbunden ist
  if (client.connected()) {
    if (!mqttConnected) {
      mqttConnected = true;
      sendPushAllCommand();
      pushAllCommandSent = true;
    }
  } else {
    mqttConnected = false;
  }

  // Wenn MQTT nicht verbunden ist, Verbindung versuchen herzustellen
  if (!client.connected() && (millis() - lastAttemptTime) > RECONNECT_INTERVAL) {
    if (client.connect("ESP32Client", mqtt_user, mqtt_password)) {
      Serial.println("Mit MQTT-Server verbunden");
      setMqttConnected();
      client.subscribe(mqtt_topic);
    } else {
      Serial.print("Fehler bei der Verbindung zum MQTT-Server: ");
      setMqttDisconnected();
      Serial.println(client.state());
      lastAttemptTime = millis();
    }
  }

  if (autoPushAllEnabled) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= 10000) {  // 10 seconds
      previousMillis = currentMillis;

      // Überprüfen, ob eine MQTT-Verbindung besteht, bevor publishPushAllMessage() aufgerufen wird
      if (client.connected()) {
        if (debug) {
          Serial.println("Requesting pushAll...");
        }
        publishPushAllMessage();
      }
    }
  }

  if (hmsErrorExists != previousHmsErrorExists) {
    if (!hmsErrorExists) {
      clearHmsErrorInfo();
    }
    previousHmsErrorExists = hmsErrorExists;
  }

  ledControlSwitch();
  client.loop();
  handleSerialCommands();
  ws.loop();
}

//send pushall mqtt message
void publishPushAllMessage() {
  if (client.connected()) {
    char publish_topic[128];
    sprintf(publish_topic, "device/%s/request", serial_number);

    DynamicJsonDocument doc(1024);
    doc["pushing"]["sequence_id"] = sequence_id;
    doc["pushing"]["command"] = "pushall";

    String jsonMessage;
    serializeJson(doc, jsonMessage);

    // Log the JSON message if debugging is enabled
    if (debug) {
      Serial.println("JSON message to send: " + jsonMessage);
    }

    bool success = client.publish(publish_topic, jsonMessage.c_str());

    if (success) {
      if (debug) {
        Serial.print("Nachricht erfolgreich gesendet an: ");
        Serial.println(publish_topic);
      }
    } else {
      Serial.println("Fehler beim Senden der Nachricht.");
    }

    // Log the result of client.publish() if debugging is enabled
    if (debug) {
      Serial.println("client.publish() returned: " + String(success));
    }

    // Erhöhe die sequence_id um 1
    sequence_id++;
  } else {
    Serial.println("Nicht mit MQTT-Broker verbunden!");
  }
}

//send mqtt message
void publishMessage(const char* messageContent) {
  if (client.connected()) {
    char publish_topic[128];
    sprintf(publish_topic, "device/%s/request", serial_number);

    String jsonMessage = createJsonMessage(messageContent);

    // Log the JSON message if debugging is enabled
    if (debug) {
      Serial.println("JSON message to send: " + jsonMessage);
    }

    bool success = client.publish(publish_topic, jsonMessage.c_str());

    if (success) {
      Serial.print("Nachricht erfolgreich gesendet an: ");
      Serial.println(publish_topic);
    } else {
      Serial.println("Fehler beim Senden der Nachricht.");
    }

    // Log the result of client.publish() if debugging is enabled
    if (debug) {
      Serial.println("client.publish() returned: " + String(success));
    }

    // Erhöhe die sequence_id um 1
    sequence_id++;
  } else {
    Serial.println("Nicht mit MQTT-Broker verbunden!");
  }
}

//load config
void loadPreferences() {
  preferences.begin("mqtt_config", false);
  preferences.getString("server", mqtt_server, sizeof(mqtt_server));
  preferences.getString("port", mqtt_port, sizeof(mqtt_port));
  preferences.getString("user", mqtt_user, sizeof(mqtt_user));
  preferences.getString("password", mqtt_password, sizeof(mqtt_password));
  preferences.getString("serial", serial_number, sizeof(serial_number));
  debug = preferences.getBool("debug");                        // Laden des Debug-Status
  String szenario_string = preferences.getString("szenario");  // Default to "icon"
  szenario = szenario_string.toInt();
  brightness = preferences.getInt("brightness", 84);
  preferences.end();
}

//save config
void savePreferences(WiFiManagerParameter& server, WiFiManagerParameter& port, WiFiManagerParameter& user, WiFiManagerParameter& password, WiFiManagerParameter& serial, WiFiManagerParameter& szenario_param) {
  preferences.begin("mqtt_config", false);
  preferences.putString("server", server.getValue());
  preferences.putString("port", port.getValue());
  preferences.putString("user", user.getValue());
  preferences.putString("password", password.getValue());
  preferences.putString("serial", serial.getValue());
  preferences.putBool("debug", debug);
  preferences.putString("szenario", szenario_param.getValue());
  preferences.putInt("brightness", brightness);
  preferences.end();
}

// mqtt json parser
void mqttCallback(char* topic, byte* payload, unsigned int length) {
  if (length == 2 && payload[0] == '{' && payload[1] == '}') {
    if (debug) {
      Serial.println("Leere Nachricht empfangen");
    }
    return;
  }

  DynamicJsonDocument doc(20000);
  DeserializationError error = deserializeJson(doc, payload, length);

  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return;
  }

  if (doc.containsKey("print") && doc["print"].containsKey("stg_cur")) {
    printer_stage = doc["print"]["stg_cur"].as<int>();
  }
  if (doc.containsKey("print") && doc["print"].containsKey("print_real_action")) {
    printer_real_stage = doc["print"]["print_real_action"];
  }
  unsigned int received_sequence_id = doc["print"]["sequence_id"].as<unsigned int>();

  if (doc.containsKey("print")) {
    JsonObject printObject = doc["print"];

    if (printObject.containsKey("gcode_state")) {
      gcodeState = printObject["gcode_state"].as<String>();
      if (debug) {
        Serial.println("Gcode State: " + gcodeState);
      }
    }

    if (printObject.containsKey("hms")) {
      JsonArray hmsArray = printObject["hms"];
      if (hmsArray.size() == 0) {
        // HMS-Fehler wurde aufgehoben
        if (debug) {
          Serial.println("Kein HMS-Fehler vorhanden.");
        }
        hmsErrorExists = false;
      } else {
        // HMS-Fehler vorhanden
        processHms(hmsArray);
        hmsErrorExists = true;
      }
    }

    // Abgreifen der neuen Felder (falls vorhanden)
    if (printObject.containsKey("big_fan1_speed")) {
      bigFan1Speed = printObject["big_fan1_speed"].as<int>();
    }

    if (printObject.containsKey("cooling_fan_speed")) {
      coolingFanSpeed = printObject["cooling_fan_speed"].as<int>();
    }

    // Weitere Felder, die abgegriffen werden können
    if (printObject.containsKey("big_fan2_speed")) {
      bigFan2Speed = printObject["big_fan2_speed"].as<int>();
    }

    if (printObject.containsKey("chamber_temper")) {
      chamberTemperature = printObject["chamber_temper"].as<int>();
    }

    if (printObject.containsKey("heatbreak_fan_speed")) {
      heatbreakFanSpeed = printObject["heatbreak_fan_speed"].as<int>();
    }

    if (printObject.containsKey("nozzle_target_temper")) {
      nozzleTargetTemperature = printObject["nozzle_target_temper"].as<int>();
    }

    if (printObject.containsKey("nozzle_temper")) {
      nozzleTemperature = printObject["nozzle_temper"].as<int>();
    }

    if (printObject.containsKey("bed_target_temper")) {
      bedTargetTemperature = printObject["bed_target_temper"].as<int>();
    }

    if (printObject.containsKey("bed_temper")) {
      bedTemperature = printObject["bed_temper"].as<int>();
    }

    if (printObject.containsKey("aux_part_fan")) {
        aux_part_fan = printObject["aux_part_fan"].as<bool>();
    }

    if (printObject.containsKey("gcode_file")) {
        gcode_file = printObject["gcode_file"].as<String>();
    }

    if (printObject.containsKey("total_layer_num")) {
        total_layer_num = printObject["total_layer_num"].as<int>();
    }

    if (printObject.containsKey("layer_num")) {
        layer_num = printObject["layer_num"].as<int>();
    }


    if (printObject.containsKey("subtask_name")) {
        subtask_name = printObject["subtask_name"].as<String>();
    }
  }

  if (received_sequence_id > sequence_id) {
    sequence_id = received_sequence_id;
  }
  if (debug) {
    Serial.print("Nachricht empfangen");
  }

  Serial.println(".");
  if (debug) {
    Serial.print(" [");
    Serial.print(topic);
    Serial.print("]");
    Serial.println();
    serializeJsonPretty(doc, Serial);
    Serial.println();
    Serial.print("Empfangene Payload-Länge: ");
    Serial.println(length);
  }
  // Websocket
  DynamicJsonDocument webData(20000);

  webData["printer_stage"] = printer_stage;
  webData["printer_real_stage"] = printer_real_stage;
  webData["gcodeState"] = gcodeState;
  webData["hmsErrorExists"] = hmsErrorExists;
  webData["bigFan1Speed"] = bigFan1Speed;
  webData["coolingFanSpeed"] = coolingFanSpeed;
  webData["bigFan2Speed"] = bigFan2Speed;
  webData["chamberTemperature"] = chamberTemperature;
  webData["heatbreakFanSpeed"] = heatbreakFanSpeed;
  webData["nozzleTargetTemperature"] = nozzleTargetTemperature;
  webData["nozzleTemperature"] = nozzleTemperature;
  webData["bedTargetTemperature"] = bedTargetTemperature;
  webData["bedTemperature"] = bedTemperature;
  webData["aux_part_fan"] = aux_part_fan;
  webData["gcode_file"] = gcode_file;
  webData["total_layer_num"] = total_layer_num;
  webData["subtask_name"] = subtask_name;
  webData["layer_num"] = layer_num;
  webData["gcodeState"] = gcodeState;


  JsonArray fullCodesArray = webData.createNestedArray("full_codes");
  JsonArray urlsArray = webData.createNestedArray("urls");

  for (int i = 0; i < hms_error_count; i++) {
    fullCodesArray.add(full_codes[i]);
    urlsArray.add(urls[i]);
  }


  // Konvertieren Sie das JSON-Dokument in einen String
  String webMessage;
  serializeJson(webData, webMessage);

  // Senden Sie die Daten an alle verbundenen WebSocket-Clients
  ws.broadcastTXT(webMessage);
}

// debug for hms error
void debugHmsErrorStatus() {
  Serial.print("hmsErrorExists: ");
  Serial.println(hmsErrorExists ? "true" : "false");
}

// HMS Error Code Analyse
void processHms(JsonArray hmsArray) {
  Serial.println("Entering processHms function.");
  Serial.print("hmsArray size: ");
  Serial.println(hmsArray.size());

  bool newHmsErrorExists = false;  // Variable to hold the new status

  if (hmsArray && hmsArray.size() > 0) {
    newHmsErrorExists = true;
    Serial.println("hmsErrorExists set to true");

    hms_error_count = 0;        // Reset the HMS error count
    f_layerInspection = false;  // Reset the f_layerInspection flag

    for (JsonObject hms_code : hmsArray) {
      String attr = DecimalHexTwosComplement(hms_code["attr"]);
      String code = DecimalHexTwosComplement(hms_code["code"]);

      // Check if we've reached the maximum number of HMS errors that can be tracked
      if (hms_error_count < MAX_HMS_ERRORS) {
        // Add a check here to ignore the specific combination of attr and code
        if (attr.equals("201327360") && code.equals("196619")) {
          f_layerInspection = true;  // Set the global variable to true
        } else {
          // Update the global arrays instead of creating new local variables
          full_codes[hms_error_count] = attr.substring(0, 4) + "_" + attr.substring(4) + "_" + code.substring(0, 4) + "_" + code.substring(4);
          urls[hms_error_count] = "https://wiki.bambulab.com/en/x1/troubleshooting/hmscode/" + full_codes[hms_error_count];

          Serial.print("HMS-Code: ");
          Serial.print(full_codes[hms_error_count]);
          Serial.print(", URL: ");
          Serial.println(urls[hms_error_count]);

          hms_error_count++;  // Increment the HMS error count
        }
      }
    }
  } else {
    newHmsErrorExists = false;
    f_layerInspection = false;  // Reset the f_layerInspection flag
    Serial.println("hmsErrorExists set to false");
  }

  if (previousHmsErrorExists && !newHmsErrorExists) {
    hmsErrorExists = false;  // Set to false when no more HMS errors
    Serial.println("No more HMS Error!");
  } else if (!previousHmsErrorExists && newHmsErrorExists) {
    hmsErrorExists = true;  // Set to true when a new HMS error is detected
    Serial.println("New HMS Error detected!");
  }

  previousHmsErrorExists = newHmsErrorExists;  // Update the flag with the new status
  debugHmsErrorStatus();
}

// send the pushall command to mqtt
void sendPushAllCommand() {
  if (client.connected() && !pushAllCommandSent) {
    String mqtt_topic_request = "device/";
    mqtt_topic_request += serial_number;
    mqtt_topic_request += "/request";

    StaticJsonDocument<128> doc;
    doc["pushing"]["sequence_id"] = String(sequence_id++);
    doc["pushing"]["command"] = "pushall";

    String payload;
    serializeJson(doc, payload);

    client.publish(mqtt_topic_request.c_str(), payload.c_str());
    pushAllCommandSent = true;
  }
}

void configureSerial() {
  preferences.begin("mqtt_config", false);

  Serial.println("===========================================");
  Serial.println("          MQTT Configuration Menu         ");
  Serial.println("===========================================");

  String server = askForInput("Printer IP", preferences.getString("server", ""));
  Serial.println();
  String port = askForInput("MQTT Port", preferences.getString("port", ""));
  Serial.println();
  String user = askForInput("MQTT User", preferences.getString("user", ""));
  Serial.println();
  String password = askForInput("Printer Accesscode", preferences.getString("password", ""));
  Serial.println();
  String serial = askForInput("Printer Serial Number", preferences.getString("serial", ""));
  Serial.println();
  String szenarioString = askForInput("Szenario - Type 1 for Icon Cover and 2 for Logo Cover", preferences.getString("szenario", ""));
  Serial.println();

  Serial.println("-------------------------------------------");
  Serial.println("Debugging Status (on/off)? Current: " + String(debug ? "on" : "off"));
  Serial.println();
  while (!Serial.available())
    ;
  String debugStatus = Serial.readStringUntil('\n');
  debugStatus.trim();
  debug = (debugStatus == "on");

  preferences.putString("server", server);
  preferences.putString("port", port);
  preferences.putString("user", user);
  preferences.putString("password", password);
  preferences.putString("serial", serial);
  preferences.putString("szenario", szenarioString);
  preferences.putBool("debug", debug);
  preferences.end();

  Serial.println("-------------------------------------------");
  Serial.println("Configuration saved. Reboot in 3 seconds.");
  Serial.println("===========================================");
  delay(3000);
  ESP.restart();
}


// config over serial function
String askForInput(const String& prompt, const String& currentValue) {
  Serial.print(prompt);
  Serial.print(" (Current: ");
  Serial.print(currentValue);
  Serial.print("): ");
  while (Serial.available() == 0) {
  }

  String newValue = Serial.readStringUntil('\n');
  newValue.trim();  // Entfernen von führenden oder nachfolgenden Leerzeichen

  if (newValue.length() == 0) {
    // Wenn keine Eingabe gemacht wurde, den alten Wert beibehalten
    newValue = currentValue;
  }

  return newValue;
}

// send json message to mqtt
void publishJsonMessage(const char* jsonString) {
  if (client.connected()) {
    char publish_topic[128];
    sprintf(publish_topic, "device/%s/request", serial_number);

    if (client.publish(publish_topic, jsonString)) {
      if (debug) {
        Serial.print("Nachricht erfolgreich gesendet an: ");
        Serial.println(publish_topic);
      }

    } else {
      Serial.println("Fehler beim Senden der Nachricht.");
    }

    // Erhöhe die sequence_id um 1
    sequence_id++;
  } else {
    Serial.println("Nicht mit MQTT-Broker verbunden!");
  }
}

/* aktuelle befehle:
sendPrintCommand("stop", "");
sendPrintCommand("pause", "");
sendPrintCommand("resume", "");
sendPrintCommand("print_speed", "1");  // Silent
sendPrintCommand("print_speed", "2");  // Standard
sendPrintCommand("print_speed", "3");  // Sport
sendPrintCommand("print_speed", "4");  // Ludicrous
*/

void sendPrintCommand(const char* command, const char* param) {
  DynamicJsonDocument doc(1024);
  doc["print"]["sequence_id"] = String(sequence_id);
  doc["print"]["command"] = command;
  doc["print"]["param"] = param;

  String jsonString;
  serializeJson(doc, jsonString);

  publishJsonMessage(jsonString.c_str());
}

/*aktuelle befehle:
sendLedControlCommand("chamber_light", "on", 500, 500, 1, 1000);
sendLedControlCommand("work_light", "off", 500, 500, 1, 1000);
*/
void sendLedControlCommand(const char* ledNode, const char* ledMode, int ledOnTime, int ledOffTime, int loopTimes, int intervalTime) {
  DynamicJsonDocument doc(1024);
  doc["system"]["sequence_id"] = String(sequence_id);
  doc["system"]["command"] = "ledctrl";
  doc["system"]["led_node"] = ledNode;
  doc["system"]["led_mode"] = ledMode;
  doc["system"]["led_on_time"] = ledOnTime;
  doc["system"]["led_off_time"] = ledOffTime;
  doc["system"]["loop_times"] = loopTimes;
  doc["system"]["interval_time"] = intervalTime;

  String jsonString;
  serializeJson(doc, jsonString);

  publishJsonMessage(jsonString.c_str());
}

/*
sendGcodeCommand("M106 P1 S", "0");   // Set part cooling fan speed to off
sendGcodeCommand("M106 P2 S", "255"); // Set aux fan speed to 100%
sendGcodeCommand("M106 P3 S", "127"); // Set chamber fan speed to ~50%
sendGcodeCommand("M960 S5 P", "1");   // Turn on the logo light on toolhead
sendGcodeCommand("M960 S4 P", "0");   // Turn off the nozzle light
*/
void sendGcodeCommand(const char* command, const char* param) {
  DynamicJsonDocument doc(1024);
  doc["print"]["sequence_id"] = String(sequence_id);
  doc["print"]["command"] = "gcode_line";
  doc["print"]["param"] = String(command) + " " + String(param) + "\n";

  String jsonString;
  serializeJson(doc, jsonString);

  publishJsonMessage(jsonString.c_str());
}

// handle the serial commands
void handleSerialCommands() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command == "config") {
      configureSerial();
    } else if (command == "debug on") {
      debug = true;
      Serial.println("Debugging enabled");
    } else if (command == "debug off") {
      debug = false;
      Serial.println("Debugging disabled");
    } else if (command == "print stop") {
      sendPrintCommand("stop", "");
    } else if (command == "print pause") {
      sendPrintCommand("pause", "");
    } else if (command == "print resume") {
      sendPrintCommand("resume", "");
    } else if (command.startsWith("M106 P1 S")) {
      sendGcodeCommand("M106 P1 S", command.substring(9).c_str());
    } else if (command.startsWith("M106 P2 S")) {
      sendGcodeCommand("M106 P2 S", command.substring(9).c_str());
    } else if (command.startsWith("M106 P3 S")) {
      sendGcodeCommand("M106 P3 S", command.substring(9).c_str());
    } else if (command.startsWith("M960 S5 P")) {
      sendGcodeCommand("M960 S5 P", command.substring(9).c_str());
    } else if (command.startsWith("M960 S4 P")) {
      sendGcodeCommand("M960 S4 P", command.substring(9).c_str());
    } else if (command.startsWith("chamber_light ")) {
      sendLedControlCommand("chamber_light", command.substring(14).c_str(), 500, 500, 1, 1000);
    } else if (command.startsWith("work_light ")) {
      sendLedControlCommand("work_light", command.substring(11).c_str(), 500, 500, 1, 1000);
    } else if (command.startsWith("print_speed ")) {
      sendPrintCommand("print_speed", command.substring(12).c_str());
    } 
    // Hinzugefügt: Helligkeit über die serielle Konsole ändern
    else if (command.startsWith("brightness ")) {
      int new_brightness = command.substring(15).toInt();
      if (new_brightness >= 0 && new_brightness <= 255) {
          setFastLEDBrightness(new_brightness);
          Serial.println("Brightness set to: " + String(brightness));
      } else {
          Serial.println("Invalid brightness value. Must be between 0 and 255.");
      }
    }
    else {
      Serial.println("Unrecognized command: " + command);
    }
  }
}

void checkForUpdates() {
  if (debug) Serial.println("Starting update check...");
  String payload;

  // Establish connection with the server
  if (espClient.connect("api.github.com", 443)) {
    if (debug) Serial.println("Connected to GitHub API.");
    espClient.println("GET /repos/vr6syncro/Bambulab_Esp32_LEDController/releases/latest HTTP/1.1");
    espClient.println("Host: api.github.com");
    espClient.println("User-Agent: ESP32");
    espClient.println("Connection: close");
    espClient.println();

    // Read the response
    while (espClient.connected()) {
      String line = espClient.readStringUntil('\n');
      if (line == "\r") {
        break;
      }
    }
    payload = espClient.readString();
  } else {
    if (debug) Serial.println("Failed to connect to GitHub API.");
  }

  if (payload != "") {
    if (debug) {
      Serial.println("Received response from GitHub API.");
      Serial.println(payload);
    }

    // Parse the JSON payload
    DynamicJsonDocument doc(4096);
    deserializeJson(doc, payload);

    // Get the latest version from the JSON
    latestVersion = String(doc["tag_name"].as<const char*>());

    // Compare with the current firmware version
    if (strcmp(firmwareVersion, latestVersion.c_str()) < 0) {
      // New version is available
      updateAvailable = true;
      Serial.println("New firmware version available!");
      Serial.print("Current version: ");
      Serial.println(firmwareVersion);
      Serial.print("Latest version: ");
      Serial.println(latestVersion.c_str());
    } else {
      // Firmware is up to date
      Serial.println("Firmware is up to date.");
    }
  } else {
    if (debug) Serial.println("No payload received from GitHub API.");
  }
}
