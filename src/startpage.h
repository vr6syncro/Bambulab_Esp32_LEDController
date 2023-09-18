#ifndef STARTPAGE_H
#define STARTPAGE_H

extern const char* firmwareVersion;
extern char mqtt_server[40];
extern char mqtt_port[6];
extern char mqtt_user[30];
extern char mqtt_password[30];
extern char serial_number[20];
extern bool debug;
extern int szenario;
extern int brightness;
extern int led_scenario_3;
extern bool updateAvailable;
extern String latestVersion;

void loadPreferences();

String generateFooter() {
  String footer = R"rawliteral(
    <div class='center-align'>
    <footer>
      <p>Firmware Version: )rawliteral"
  + String(firmwareVersion);

  if (updateAvailable) {
    footer += " | <a href='/ota' target='_blank'>New Update: " + latestVersion + " is available!</a>";
  }

  footer += R"rawliteral(</p>
    </footer>
    </div>
  )rawliteral";

  return footer;
}

String getStartPage() {
  loadPreferences();
  String html = R"rawliteral(
<!DOCTYPE html>
<html lang='en'>
<head>
  <title>Startseite</title>
  <meta charset='utf-8'/>
  <meta name='viewport' content='width=device-width, initial-scale=1'>
  <link rel='stylesheet' href='https://cdnjs.cloudflare.com/ajax/libs/materialize/1.0.0/css/materialize.min.css'>
<style>
    .sidenav-trigger {
        display: block !important;
        margin-top: 10px;
    }
    .sidenav {
        width: 350px !important;
    }
    .sidenav h5, .sidenav li {
        text-align: center;
    }
    .sidenav input[type="text"], .sidenav input[type="password"], .sidenav input[type="number"] {
        width: 80%;
        margin: 0 auto;
        display: block;
        text-align: center;
    }
    #mqttConfigForm {
        text-align: center;
    }
    #mqttConfigForm li {
        margin-bottom: 10px;
    }
    #mqttConfigForm button {
        background-color: #333;
        color: #fff;
        border: none;
        border-radius: 4px;
        padding: 10px 20px;
        font-size: 16px;
        cursor: pointer;
        transition: background-color 0.3s ease;
    }
    #mqttConfigForm button:hover {
        background-color: #555;
    }
    .info-box {
        border-radius: 10px;
        background-color: #E5E4E2;
        padding: 20px;
        margin: 20px auto;
        max-width: 800px;
        box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
        display: flex;
        flex-direction: row;
        justify-content: space-between;
        align-items: center;
    }
    .info-box h5 {
        margin-left: 30px;
        margin-right: 30px;
    }
    .info-box div a {
        display: block;
        text-align: center;
    }
    .info-box img {
    margin: auto;
    display: block;
    max-width: 100%; /* stellt sicher, dass das Bild nicht größer als die box ist */
    max-height: 100%; /* gleicher Grund */
    }
    nav {
        background-color: #333;
        padding: 0 20px;
        position: relative;
    }
    nav a {
        color: #fff;
    }
    nav .brand-logo img {
        height: 60px;
        margin: 5px 0;
    }
    html {
        height: 100%;
    }
    body {
        min-height: 100vh;
        margin: 0;
        display: flex;
        flex-direction: column;
        padding-top: 20px;
    }
    .container {
        flex: 1;
    }
    footer {
        position: fixed;
        bottom: 0;
        left: 0;
        width: 100%;
        display: flex;
        justify-content: center;
        align-items: center;
        text-align: center;
    }
    .center-align {
        text-align: center;
    }
    .input-field {
        width: 70%;
        text-align: center;
    }
    .row .col {
        width: 45%;
        padding: 0 30px;
    }
    .logo-container {
        text-align: center;
        margin-bottom: 20px;
        margin-top: 20px;
    }
    nav ul li.active {
        background-color: #e0e0e0 !important;
    }
    nav ul li.active a {
        color: #555 !important;
    }
    .modal-content {
        text-align: center;
    }
    .modal-content h4, .modal-content p {
        margin: 20px 0;
    }
    nav a.left {
        margin-left: 0px;
    }
    nav .brand-logo {
        display: block;
    }
    @media (max-width: 900px) {
        nav .brand-logo {
            display: none;
        }
    }
    .range-field {
     padding-left: 30px;
     padding-right: 30px;
    }
    li > label {
    margin-bottom: 5px;
    }
</style>
  <script src='https://cdnjs.cloudflare.com/ajax/libs/materialize/1.0.0/js/materialize.min.js'></script>
</head>
<body>
<ul id="slide-out" class="sidenav right-aligned">
    <li><h5>Update the Config</h5></li>
    <form id="mqttConfigForm">
<li>
  <label for="server">Printer IP</label>
  <input type="text" id="server" placeholder="IP" value=")rawliteral"
            + String(mqtt_server) + R"rawliteral(">
</li>
<li>
  <input type="text" id="port" style="display:none;" placeholder="Port" value=")rawliteral"
            + String(mqtt_port) + R"rawliteral(">
</li>
<li>
  <input type="text" id="user" style="display:none;" placeholder="User" value=")rawliteral"
            + String(mqtt_user) + R"rawliteral(">
</li>
<li>
  <label for="password">Printer Secret</label>
  <input type="password" id="password" placeholder="Secret" value=")rawliteral"
            + String(mqtt_password) + R"rawliteral(">
</li>
<li>
  <label for="erial">Printer Serial</label>
  <input type="text" id="serial" placeholder="Serial" value=")rawliteral"
            + String(serial_number) + R"rawliteral(">
</li>
<li>
  <label for="brightness">Set Brightness</label>
  <input type="number" id="brightness" placeholder="Brightness (0-255)" value=")rawliteral"
            + String(brightness) + R"rawliteral(" min="0" max="255">
</li>
<li>
  <label>Szenario wählen</label>
  <select id="szenario" data-selected=")rawliteral" + String(szenario) + R"rawliteral(">
    <option value="1")rawliteral" + (szenario == 1 ? " selected" : "") + R"rawliteral(">Icon Cover</option>
    <option value="2")rawliteral" + (szenario == 2 ? " selected" : "") + R"rawliteral(">Logo Cover</option>
    <option value="3")rawliteral" + (szenario == 3 ? " selected" : "") + R"rawliteral(">Strip Mode</option>
  </select>
</li>
<li id="numLedsContainer" style="display:none;">
  <p>Number of Leds you use</p>
  <input type="number" id="led_scenario_3" placeholder="Number of Leds you use" value=")rawliteral"
+ String(led_scenario_3) + R"rawliteral(">
</li>
        <li>
            <label>
            <input type="hidden" id="debugValue" value=")rawliteral"
                + (debug ? "true" : "false") + R"rawliteral(">
                <input type="checkbox" id="debug" />
               <span>Debug</span>
            </label>
        </li>
        <li><button type="submit">Save</button></li>
    </form>
</ul>
<nav>
  <div class="nav-wrapper">
    <a href="#" class="brand-logo center">Bambulab Esp32 LEDController</a>
    <ul>
        <li class="active"><a href="/" class="left">Welcome</a></li>
        <li><a href="/info" class="left">Printer Info</a></li>
    </ul>
    <a href="#" data-target="slide-out" class="sidenav-trigger right">Config</a>
  </div>
</nav>
<div class="container">
  <div class='info-box'>
    <div>
        <h5>Discord</h5>
        <a href="http://discord.vr6syncro.de" target="_blank">Join Server</a>
    </div>
    <div>
        <h5>Github</h5>
        <a href="https://github.com/vr6syncro/Bambulab_Esp32_LEDController" target="_blank">Open  Repository</a>
    </div>
    <div>
        <h5>Printables</h5>
        <a href="https://www.printables.com/de/model/501355-bambulab-esp32-ledcontroller" target="_blank">3D Models</a>
    </div>
  </div>
  <div class="container">
  <div class='info-box'>
    <img src="https://github.com/vr6syncro/vr6syncro/blob/main/img/ak_rund.png?raw=true" alt="vr6syncro">
  </div>
</div>
<div class="container">
  <div class='info-box center-align'>
    <div class='buy-me-a-coffee'>
        <script type="text/javascript" src="https://cdnjs.buymeacoffee.com/1.0.0/button.prod.min.js" data-name="bmc-button" data-slug="vr6syncro" data-color="#01c3ff" data-emoji=""  data-font="Bree" data-text="Buy me a drink" data-outline-color="#000000" data-font-color="#000000" data-coffee-color="#FFDD00" ></script>
    </div>
  </div>
</div>
    <div id="modal" class="modal">
    <div class="modal-content">
        <h4>Configuration saved</h4>
        <p>The ESP32 will restart in <span id="countdown">5</span> seconds.</p>
    </div>
  </div>
)rawliteral"
+ generateFooter() +
R"rawliteral(
</div>
<script>
var isSocketOpen = false;  // Zustand der WebSocket-Verbindung

function connectWebSocket() {
  var socket = new WebSocket('ws://' + location.hostname + ':81/ws');

  socket.onopen = function(event) {
    console.log("WebSocket connection opened.");
    isSocketOpen = true;
  };

  socket.onerror = function(event) {
    console.error("WebSocket error observed:", event);
    isSocketOpen = false;
  };

  socket.onclose = function(event) {
    console.log("WebSocket connection closed:", event);
    isSocketOpen = false;
    setTimeout(connectWebSocket, 5000);  // Versuche, alle 5 Sekunden wiederzuverbinden
  };

  document.addEventListener('DOMContentLoaded', function() {
    var sidenavElems = document.querySelectorAll('.sidenav');
    M.Sidenav.init(sidenavElems, {edge: 'right'});

    var modalElems = document.querySelectorAll('.modal');
    M.Modal.init(modalElems);

    var selectElems = document.querySelectorAll('select');
    M.FormSelect.init(selectElems);

    // Manuelle Aktualisierung des Dropdown-Menüs
    var szenarioSelect = document.getElementById("szenario");
    for (var i = 0; i < szenarioSelect.options.length; i++) {
      if (szenarioSelect.options[i].value === szenarioSelect.getAttribute("data-selected")) {
        szenarioSelect.options[i].selected = true;
        break;
      }
    }
    
    // Materialize Dropdown aktualisieren
    M.FormSelect.init(szenarioSelect);
    var instance = M.FormSelect.getInstance(szenarioSelect);
    instance._setValueToInput();

    let debugValue = document.getElementById("debugValue").value === "true";
    document.getElementById("debug").checked = debugValue;

    var numLedsContainer = document.getElementById("numLedsContainer");

    function toggleNumLedsInput() {
      let selectedSzenario = parseInt(document.getElementById("szenario").value, 10);
      console.log("selectedSzenario:", selectedSzenario);
      if (selectedSzenario === 3) {
        numLedsContainer.style.display = "block";
      } else {
        numLedsContainer.style.display = "none";
      }
    }

    toggleNumLedsInput();
    document.getElementById("szenario").addEventListener("change", toggleNumLedsInput);
  });

  document.getElementById("mqttConfigForm").addEventListener("submit", function(event) {
    event.preventDefault();

    if (!isSocketOpen) {
      console.error("WebSocket is not open. Cannot send data.");
      return;
    }

    var formDataConfig = {
        type: "mqttConfig",
        server: document.getElementById("server").value,
        port: document.getElementById("port").value,
        user: document.getElementById("user").value,
        password: document.getElementById("password").value,
        serial: document.getElementById("serial").value,
        debug: document.getElementById("debug").checked,
        szenario: document.getElementById("szenario").value,
        led_scenario_3: document.getElementById("led_scenario_3").value,
        brightness: document.getElementById("brightness").value,
    };

    socket.send(JSON.stringify(formDataConfig));

    var modalInstance = M.Modal.getInstance(document.getElementById('modal'));
    modalInstance.open();

    var countdown = 5;
    var countdownInterval = setInterval(function() {
      countdown--;
      document.getElementById("countdown").textContent = countdown;
      if (countdown <= 0) {
        clearInterval(countdownInterval);
        location.reload();
      }
    }, 1000);
  });
}

connectWebSocket();
</script>
</body>
</html>
)rawliteral";
  return html;
}

#endif