
#ifndef OBS_H
#define OBS_H

String getOBSPage() {
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
.no-margin-bottom {
    margin-bottom: 0;
}
        /* Stil für die 1080p Box */
        .box-1080p {
          width: 1920px; /* Breite für 1080p */
            height: 1080px; /* Höhe für 1080p */
            position: relative; /* Damit das Kind-Element absolut positioniert werden kann */
        }
        /* Stil für die 720p Box */
        .box-720p {
            width: 1280px; /* Breite für 720p */
            height: 720px; /* Höhe für 720p */
            position: relative; /* Damit das Kind-Element absolut positioniert werden kann */
        }

/* Stil für den Fortschrittsbalken-Container */
.progress-bar-container {
    position: absolute;
    bottom: 0;
    width: 100%;
    padding: 10px;
    box-sizing: border-box;
    display: flex;
    flex-direction: column;
    justify-content: center;
    align-items: center;
    font-weight: bold;
}

        /* Weitere Stile für den Fortschrittsbalken */
        .progress {
		height: 30px; /* Höhe der Fortschrittsanzeige */
            width: 100%;
            background-color: rgba(238, 238, 238, 0.2);
        }

.determinate {
    
    background-color: #074a0f;
    transition: width 0.3s;
    
}

        .center-align {
            text-align: center;
            margin-top: 5px;
        }
        /* Stil für den rechten Bereich */
        .right-section {
            position: absolute;
            right: 0;
            top: 0;
            width: 300px; /* Oder die gewünschte Breite */
            padding: 10px;
            box-sizing: border-box;
        }

.text-displayauto {
    padding: 5px;
    margin-bottom: 10px;
    box-sizing: border-box;
    text-align: center;
    border: 1px solid #e0e0e0;
    background-color: rgba(247, 247, 247, 0.5);
    border-radius: 5px;
    position: relative;
    width: auto;
    min-width: 10px; /* Mindestbreite */
    flex: 1; /* Flexibles Wachstum in der Höhe */
    font-weight: bold;
}

        .text-display {
            width: 100%;
            padding: 5px;
            margin-bottom: 10px;
            box-sizing: border-box;
            text-align: center;
            border: 1px solid #e0e0e0;
            background-color: rgba(247, 247, 247, 0.5); /* Transparenter Hintergrund */
            border-radius: 5px; /* Leicht abgerundete Ecken */
            position: relative; /* Für das ::before Pseudoelement */
            padding-left: 25px; /* Platz für das Icon */
        }

        .text-display strong {
            display: block;
            margin-bottom: 5px;
             font-weight: bold;
        }

        /* Stil für die span-Elemente innerhalb der Anzeigeelemente */
.text-display span {
    font-weight: bold; /* Fettschrift */
    font-size: 18px; /* Größere Schriftgröße */
    color: #333; /* Dunkle Schriftfarbe für bessere Sichtbarkeit */
}

        /* Stil für das Icon */
        .text-display::before {
            content: "";
            position: absolute;
            left: 25px; /* Abstand vom linken Rand */
            top: 50%;  /* Vertikal zentrieren */
            transform: translateY(-50%); /* Vertikal zentrieren */
            width: 48px;  /* Breite des Icons */
            height: 48px; /* Höhe des Icons */
            background-size: contain;
			background-repeat: no-repeat;
        }

        /* Individuelle Icons für jedes Element */
        .icon-filename::before {
            background-image: url('https://github.com/vr6syncro/Bambulab_Esp32_LEDController/blob/main/icons/File.png?raw=true');
        }

        .icon-status::before {
            background-image: url('https://github.com/vr6syncro/Bambulab_Esp32_LEDController/blob/main/icons/Status.png?raw=true');
        }

        .icon-nozzle::before {
            background-image: url('https://github.com/vr6syncro/Bambulab_Esp32_LEDController/blob/main/icons/Hotend.png?raw=true');
        }

        .icon-bed::before {
            background-image: url('https://github.com/vr6syncro/Bambulab_Esp32_LEDController/blob/main/icons/Bed.png?raw=true');
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

    var socket = new WebSocket('ws://' + location.hostname + ':81/ws');

// Handling incoming data from the ESP32
socket.onmessage = function(event) {
    var data = JSON.parse(event.data);

    if (data.hasOwnProperty('nozzleTemperature')) {
        document.getElementById('nozzleTemperature').textContent = data.nozzleTemperature + '°C';
    }

    if (data.hasOwnProperty('bedTemperature')) {
        document.getElementById('bedTemperature').textContent = data.bedTemperature + '°C';
    }

    if (data.hasOwnProperty('subtask_name')) {
        if (data.subtask_name.trim() === "") {
            document.getElementById('subtaskName').textContent = "waiting for print";
        } else {
            document.getElementById('subtaskName').textContent = data.subtask_name;
        }
    }

    if (data.hasOwnProperty('gcodeState')) {
        document.getElementById('gcodeStateID').textContent = data.gcodeState;
    }

    if (data.hasOwnProperty('layer_num') && data.hasOwnProperty('total_layer_num')) {
        let progressPercentage = (data.layer_num / data.total_layer_num) * 100;

        if (isNaN(progressPercentage)) {
            document.getElementById('progressPercentage').textContent = "Start print to view progress";
        } else {
            document.getElementById('printProgress').style.width = progressPercentage + "%";
            document.getElementById('progressPercentage').textContent = "The print is " + progressPercentage.toFixed(2) + "% complete";
        }
    }
};

});
</script>
</head>
<body>

<div class="box-1080p">
    <!-- Ihr Fortschrittsbalken-Inhalt -->
    <div class="progress-bar-container">
        <div class="progress">
            <div class="determinate" id="printProgress" style="width: 0%"></div>
        </div>
        <p class="center-align text-displayauto no-margin-bottom" id="progressPercentage">The print is 0.00% complete</p>
    </div>
</div>

        <!-- Rechte Sektion mit den gewünschten Elementen -->
        <div class="right-section">
            <p class='center-align text-display icon-filename'><strong>File Name</strong> <span id='subtaskName'>Loading...</span></p>
            <p class='center-align text-display icon-status'><strong>Status</strong> <span id='gcodeStateID'>Loading...</span></p>
            <p class='center-align text-display icon-nozzle'><strong>Nozzle Temperature</strong> <span id='nozzleTemperature'>Loading...</span></p>
            <p class='center-align text-display icon-bed'><strong>Bed Temperature</strong> <span id='bedTemperature'>Loading...</span></p>
            
        </div>
    </div>
</body>

</html>
)rawliteral";

  return html;
}

#endif