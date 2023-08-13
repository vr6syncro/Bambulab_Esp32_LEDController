#ifndef INFOPAGE_H
#define INFOPAGE_H

extern const char* getPrinterStageDescription(int stage);

String getInfoPage() {
  String hmsErrorText = (hmsErrorExists ? "Yes" : "No");
  String printerStageText = getPrinterStageDescription(printer_stage);

  String html = R"rawliteral(
<!DOCTYPE html>
<html lang='en'>
<head>
  <title>Information</title>
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
    .info-box div a {
        display: block;
        text-align: center !important;
    }
    .info-box div p {
    margin-bottom: 5px;
    }
    .info-box .input-field {
    width: 100%;
    text-align: center;
    }
    .info-box .input-field input {
    width: 100%;
    text-align: center;
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
.no-error-text {
    color: green !important;
}
.error-text, .error-text a {
    color: red !important;
}
.info-content {
    display: flex;
    flex-direction: column;
    justify-content: center;
    align-items: center;
}
</style>
  <script src='https://cdnjs.cloudflare.com/ajax/libs/materialize/1.0.0/js/materialize.min.js'></script>
<script>
function roundToNextTen(n) {
    return Math.ceil(n / 10) * 10;
}
function convertToPercentage(value) {
    return (value / 15) * 100;
}

document.addEventListener('DOMContentLoaded', function() {
      var elems = document.querySelectorAll('input[type=range]');
    M.Range.init(elems);

    document.getElementById('cooling-fan-speed').addEventListener('change', function() {
        sendGcodeCommand('M106 P1 S', this.value);
    });

    document.getElementById('aux-fan-speed').addEventListener('change', function() {
        sendGcodeCommand('M106 P2 S', this.value);
    });

    document.getElementById('chamber-fan-speed').addEventListener('change', function() {
        sendGcodeCommand('M106 P3 S', this.value);
    });

      var sidenavElems = document.querySelectorAll('.sidenav');
    var sidenavInstances = M.Sidenav.init(sidenavElems, {edge: 'right'});

    var socket = new WebSocket('ws://' + location.hostname + ':81/ws');
    
    // Functions to send commands over WebSockets
    function sendPrintCommand(command, value) {
        let data = {
            type: "printCommand",
            command: command,
            value: value
        };
        socket.send(JSON.stringify(data));
    }

    function sendLedControlCommand(light, state, arg1, arg2, arg3, arg4) {
        let data = {
            type: "ledControl",
            light: light,
            state: state,
            arg1: arg1,
            arg2: arg2,
            arg3: arg3,
            arg4: arg4
        };
        socket.send(JSON.stringify(data));
    }

    function sendGcodeCommand(command, value) {
        let data = {
            type: "gcodeCommand",
            command: command,
            value: value
        };
        socket.send(JSON.stringify(data));
    }

// EventListener für die Links hinzufügen
document.getElementById('stopLink').addEventListener('click', function(event) {
    event.preventDefault();
    sendPrintCommand('stop', '');
});

document.getElementById('pauseLink').addEventListener('click', function(event) {
    event.preventDefault();
    sendPrintCommand('pause', '');
});

document.getElementById('resumeLink').addEventListener('click', function(event) {
    event.preventDefault();
    sendPrintCommand('resume', '');
});

document.getElementById('silentLink').addEventListener('click', function(event) {
    event.preventDefault();
    sendPrintCommand('print_speed', '1');
});

document.getElementById('standardLink').addEventListener('click', function(event) {
    event.preventDefault();
    sendPrintCommand('print_speed', '2');
});

document.getElementById('sportLink').addEventListener('click', function(event) {
    event.preventDefault();
    sendPrintCommand('print_speed', '3');
});

document.getElementById('ludicrousLink').addEventListener('click', function(event) {
    event.preventDefault();
    sendPrintCommand('print_speed', '4');
});

document.getElementById('chamberLightOnLink').addEventListener('click', function(event) {
    event.preventDefault();
    sendLedControlCommand('chamber_light', 'on', 500, 500, 1, 1000);
});

document.getElementById('chamberLightOffLink').addEventListener('click', function(event) {
    event.preventDefault();
    sendLedControlCommand('chamber_light', 'off', 500, 500, 1, 1000);
});

// document.getElementById('workLightOnLink').addEventListener('click', function(event) {
//     event.preventDefault();
//     sendLedControlCommand('work_light', 'on', 500, 500, 1, 1000);
// });

// document.getElementById('workLightOffLink').addEventListener('click', function(event) {
//     event.preventDefault();
//     sendLedControlCommand('work_light', 'off', 500, 500, 1, 1000);
// });

document.getElementById('logoLightOnLink').addEventListener('click', function(event) {
    event.preventDefault();  // Verhindert, dass der Standardlink-Klick ausgeführt wird
    sendGcodeCommand('M960 S5 P', '1');
});

document.getElementById('logoLightOffLink').addEventListener('click', function(event) {
    event.preventDefault();
    sendGcodeCommand('M960 S5 P', '0');
});

document.getElementById('nozzleLightOnLink').addEventListener('click', function(event) {
    event.preventDefault();
    sendGcodeCommand('M960 S4 P', '1');
});

document.getElementById('nozzleLightOffLink').addEventListener('click', function(event) {
    event.preventDefault();
    sendGcodeCommand('M960 S4 P', '0');
});

    // Handling incoming data from the ESP32
    socket.onmessage = function(event) {
        var data = JSON.parse(event.data);
if(data.hasOwnProperty('nozzleTargetTemperature')) {
    document.getElementById('nozzleTargetTemperature').value = data.nozzleTargetTemperature + '°C';
}

if(data.hasOwnProperty('nozzleTemperature')) {
    document.getElementById('nozzleTemperature').value = data.nozzleTemperature + '°C';
}

if(data.hasOwnProperty('bedTargetTemperature')) {
    document.getElementById('bedTargetTemperature').value = data.bedTargetTemperature + '°C';
}

if(data.hasOwnProperty('bedTemperature')) {
    document.getElementById('bedTemperature').value = data.bedTemperature + '°C';
}
if(data.hasOwnProperty('bigFan1Speed')) {
    let fanSpeed = data.bigFan1Speed; 
    let fanSpeedPercentage = convertToPercentage(fanSpeed);
    fanSpeedPercentage = roundToNextTen(fanSpeedPercentage);
    document.getElementById('bigFan1Speed').value = fanSpeedPercentage + '%';
}

if(data.hasOwnProperty('coolingFanSpeed')) {
    let fanSpeed = data.coolingFanSpeed; 
    let fanSpeedPercentage = convertToPercentage(fanSpeed);
    fanSpeedPercentage = roundToNextTen(fanSpeedPercentage);
    document.getElementById('coolingFanSpeed').value = fanSpeedPercentage + '%';
}

if(data.hasOwnProperty('bigFan2Speed')) {
    let fanSpeed = data.bigFan2Speed; 
    let fanSpeedPercentage = convertToPercentage(fanSpeed);
    fanSpeedPercentage = roundToNextTen(fanSpeedPercentage);
    document.getElementById('bigFan2Speed').value = fanSpeedPercentage + '%';
}

if(data.hasOwnProperty('heatbreakFanSpeed')) {
    let fanSpeed = data.heatbreakFanSpeed; 
    let fanSpeedPercentage = convertToPercentage(fanSpeed);
    fanSpeedPercentage = roundToNextTen(fanSpeedPercentage);
    document.getElementById('heatbreakFanSpeed').value = fanSpeedPercentage + '%';
}
// Aktualisieren von hmsErrorExists, full_codes und urls
var hmsText = document.getElementById('hmsErrorExistsID');
var fullCodesContainer = document.getElementById('fullCodesContainer');
if (data.hasOwnProperty('hmsErrorExists')) {
    if(data.hmsErrorExists) {
        hmsText.innerHTML = ""; // Leer, da der Fehlercode in der fullCodesList angezeigt wird
        hmsText.classList.remove('no-error-text');
        fullCodesContainer.classList.add('error-text');
        fullCodesContainer.style.display = "block";
    } else {
        hmsText.innerHTML = "No HMS Error";
        hmsText.classList.add('no-error-text');
        fullCodesContainer.style.display = "none";
    }
}

if (data.hasOwnProperty('full_codes') && data.hasOwnProperty('urls')) {
    var fullCodesHtml = "";
    for (var i = 0; i < data.full_codes.length; i++) {
        fullCodesHtml += "<a href='" + data.urls[i] + "' target='_blank'>" + data.full_codes[i] + "</a><br>";
    }
    document.getElementById('fullCodesList').innerHTML = fullCodesHtml;
}
    };
});
</script>
</head>
<body>
<ul id="slide-out" class="sidenav right-aligned">

    <!-- Print Commands -->
    <li><h5>Print Commands</h5></li>
    <li><a href="#" class="waves-effect waves-light btn" id="stopLink">Stop</a></li>
    <li><a href="#" class="waves-effect waves-light btn" id="pauseLink">Pause</a></li>
    <li><a href="#" class="waves-effect waves-light btn" id="resumeLink">Resume</a></li>

    <li><div class="divider"></div></li>

    <!-- Print Speed -->
    <li><h5>Print Speed</h5></li>
    <li><a href="#" class="waves-effect waves-light btn" id="silentLink">Silent</a></li>
    <li><a href="#" class="waves-effect waves-light btn" id="standardLink">Standard</a></li>
    <li><a href="#" class="waves-effect waves-light btn" id="sportLink">Sport</a></li>
    <li><a href="#" class="waves-effect waves-light btn" id="ludicrousLink">Ludicrous</a></li>

    <li><div class="divider"></div></li>

    <!-- Light Controls -->
    <li><h5>Light Controls</h5></li>
    <li><a href="#" class="waves-effect waves-light btn" id="chamberLightOnLink">Chamber Light On</a></li>
    <li><a href="#" class="waves-effect waves-light btn" id="chamberLightOffLink">Chamber Light Off</a></li>
    <!-- <li><a href="#" class="waves-effect waves-light btn" id="workLightOnLink">Work Light On</a></li>
    <li><a href="#" class="waves-effect waves-light btn" id="workLightOffLink">Work Light Off</a></li> -->
    <li><a href="#" class="waves-effect waves-light btn" id="logoLightOnLink">Logo Light On</a></li>
    <li><a href="#" class="waves-effect waves-light btn" id="logoLightOffLink">Logo Light Off</a></li>
    <li><a href="#" class="waves-effect waves-light btn" id="nozzleLightOnLink">Nozzle Light On</a></li>
    <li><a href="#" class="waves-effect waves-light btn" id="nozzleLightOffLink">Nozzle Light Off</a></li>

    <li><div class="divider"></div></li>

    <!-- Fan Speeds -->
    <li><h5>Fan Speeds</h5></li>
    <li><label for="cooling-fan-speed">Part Cooling Fan Speed</label>
        <p class="range-field">
            <input type="range" id="cooling-fan-speed" min="0" max="255">
        </p>
    </li>
    <li><label for="aux-fan-speed">Aux Fan Speed</label>
        <p class="range-field">
            <input type="range" id="aux-fan-speed" min="0" max="255">
        </p>
    </li>
    <li><label for="chamber-fan-speed">Chamber Fan Speed</label>
        <p class="range-field">
            <input type="range" id="chamber-fan-speed" min="0" max="255">
        </p>
    </li>

</ul>
<nav>
  <div class="nav-wrapper">
    <a href="#" class="brand-logo center">Printer Information</a>
    <ul>
        <li><a href="/" class="left">Welcome</a></li>
        <li class="active"><a href="/info" class="left">Printer Info</a></li>
    </ul>
	  <a href="#" data-target="slide-out" class="sidenav-trigger right">Control</a>
  </div>
</nav>
<div class='container'>
    
    <!-- Row for HMS Error and Fan Speeds -->
    <div class='row'>
        <!-- HMS Error Info Box (Left) -->
<div class='col s6'>
    <div class='info-box' id='hmsErrorBox'>
        <h5>HMS Error Info</h5>
        <div class='center-align'>
            <p id='hmsErrorExistsID'>No HMS Error</p>
            <div id='fullCodesContainer'>
                <ul id='fullCodesList'></ul>
            </div>
        </div>
    </div>
</div>

        <!-- Fan Speeds Info Box (Right) -->
        <div class='col s6'>
            <div class='info-box'>
                <h5>Fan Speeds Info</h5>
                <div class="info-content">
                    <p class='center-align'>Aux Fan Speed</p>
                    <div class='input-field'>
                     <input id='bigFan1Speed' type='text' readonly>
                    </div>
                    <p class='center-align'>Part Cooling Fan Speed</p>
                    <div class='input-field'>
                     <input id='coolingFanSpeed' type='text' readonly>
                    </div>
                    <p class='center-align'>Chamber Fan Speed</p>
                    <div class='input-field'>
                     <input id='bigFan2Speed' type='text' readonly>
                    </div>
                    <p class='center-align'>Heatbreak Fan Speed</p>
                    <div class='input-field'>
                     <input id='heatbreakFanSpeed' type='text' readonly>
                    </div>
                </div>
            </div>
        </div>
    </div>
    
    <!-- Row for Nozzle and Bed Temperatures -->
    <div class='row'>
        <!-- Nozzle Temperature Info Box (Left) -->
        <div class='col s6'>
            <div class='info-box'>
                <h5>Nozzle Temperature Info</h5>
                <div class="info-content">
                    <p class='center-align'>Nozzle Target Temperature</p>
                    <div class='input-field'>
                        <input id='nozzleTargetTemperature' type='text' value=')rawliteral"
                + String(nozzleTargetTemperature) + R"rawliteral(' readonly>
                    </div>
                    <p class='center-align'>Nozzle Temperature</p>
                    <div class='input-field'>
                        <input id='nozzleTemperature' type='text' value=')rawliteral"
                + String(nozzleTemperature) + R"rawliteral(' readonly>
                    </div>
                </div>
            </div>
        </div>

        <!-- Bed Temperature Info Box (Right) -->
        <div class='col s6'>
            <div class='info-box'>
                <h5>Bed Temperature Info</h5>
                <div class="info-content">
                    <p class='center-align'>Bed Target Temperature</p>
                    <div class='input-field'>
                        <input id='bedTargetTemperature' type='text' value=')rawliteral"
                + String(bedTargetTemperature) + R"rawliteral(' readonly>
                    </div>
                    <p class='center-align'>Bed Temperature</p>
                    <div class='input-field'>
                        <input id='bedTemperature' type='text' value=')rawliteral"
                + String(bedTemperature) + R"rawliteral(' readonly>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>
 <script>
        document.addEventListener('DOMContentLoaded', function() {
            var elems = document.querySelectorAll('.sidenav');
            var instances = M.Sidenav.init(elems, {edge: 'right'});
        });
    </script>
    </body>
    
</html>
)rawliteral";

  return html;
}

#endif