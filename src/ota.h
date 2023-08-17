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
        String htmlResponse = R"(
          <html>
            <body onload="showModalAndCheckESP();">
              <div id="myModal" style="display: none; position: fixed; top: 0; left: 0; width: 100%; height: 100%; background-color: rgba(0,0,0,0.5);">
                <div style="position: absolute; top: 50%; left: 50%; transform: translate(-50%, -50%); background-color: white; padding: 20px; text-align: center;">
                  The Bambulab Esp32 LEDController is restarting. Please wait...
                </div>
              </div>
              <script>
                function showModalAndCheckESP() {
                  document.getElementById("myModal").style.display = "block";
                  setInterval(function() {
                    fetch('/')
                      .then(response => {
                        if (response.ok) {
                          window.location.replace("/");
                        }
                      })
                      .catch(error => {
                        console.log('Bambulab Esp32 LEDController is not yet available:', error);
                      });
                  }, 1000);
                }
              </script>
            </body>
          </html>
        )";
        
        AsyncWebServerResponse *response = request->beginResponse(200, "text/html", htmlResponse);
        response->addHeader("Connection", "close");
        response->addHeader("Access-Control-Allow-Origin", "*");
        request->send(response);

        delay(2000);  // Wait for 3 seconds before restarting the ESP32
        ESP.restart();
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

  server.on("/update", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->redirect("/");
  });
}

#endif
