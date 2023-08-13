#ifndef OTA_H
#define OTA_H

extern AsyncWebServer server;

void setupOTA() {
  server.on("/ota", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/html", R"(
        <form method='POST' action='/update' enctype='multipart/form-data'>
            Firmware:<br>
            <input type='file' name='update'>
            <input type='submit' value='Update'>
        </form>
        <br>
        <p>For more details or to check for updates, visit the 
            <a href='https://github.com/vr6syncro/Bambulab_Esp32_LEDController/releases' target='_blank'>
                GitHub Releases page
            </a>.
        </p>
        <p>
            <strong>Quick Start Guide:</strong>
            <ul>
                <li>Read the release notes.</li>
                <li>Make sure you want to install the update now.</li>
                <li>Download the *.bin file from the homepage.</li>
                <li>Select the file by clicking on the left button above.</li>
                <li>Click on the update button.</li>
                <li>Wait until the controller has restarted.</li>
            </ul>
        </p>
    )");
  });

  server.on(
    "/update", HTTP_POST, [](AsyncWebServerRequest *request) {
        if (Update.hasError()) {
            AsyncWebServerResponse *response = request->beginResponse(500, "text/plain", "FAIL");
            response->addHeader("Connection", "close");
            response->addHeader("Access-Control-Allow-Origin", "*");
            request->send(response);
        } else {
            AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", "OK, restarting now!");
            response->addHeader("Connection", "close");
            response->addHeader("Access-Control-Allow-Origin", "*");
            request->send(response);

            delay(1000);  // Warte kurz, bevor du den ESP32 neu startest
            ESP.restart();  // Starte den ESP32 neu
        }
    },
    [](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
        if (!index) {
            Serial.printf("UploadStart: %s\n", filename.c_str());
            uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
            if (!Update.begin(maxSketchSpace)) {
                Update.printError(Serial);
            }
        }
        if (Update.write(data, len) != len) {
            Update.printError(Serial);
        }
        if (final) {
            if (Update.end(true)) {
                Serial.printf("Update Success: %uB\n", index + len);
            } else {
                Update.printError(Serial);
            }
        }
    });

// Umleitung von /update (GET-Request) zur Hauptseite
server.on("/update", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->redirect("/");
});




#endif