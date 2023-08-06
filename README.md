<img src="ak_rund.png" alt="Logo" width="200"/>

# Bambulab Esp32 LEDController #

###### [Improved version of the Esp8622 Controller Version](https://github.com/vr6syncro/BambulabLedController_Neopixel "Improved version of the Esp8622 Controller Version") ######

## Getting started

Currently there is only the Web Flasher. The source code will be made available later.

[Click here to get to the flasher.](https://vr6syncro.de/programmer/esp32.html "Click here to get to the flasher.")

The ESP32 is flashed with the firmware.

The ESP32 is now building a W-Lan access point with which we connect. The current password is: `password`

After that we have to navigate to the Wifimanager Config page with a browser.
The IP is: `192.168.4.2`

There you enter your W-Lan registration data.

Optionally, the Bambulab printer can be set up now or you can set it up later via the serial console or via the website.

If the EPS32 is connected to the WiFi, you should be able to find and reach it in your network.
The IP of the device is also output via the console if a programming app is installed.

As soon as you enter the IP of the EPS32 in your browser, you get to the configuration page and set up your printer or change data. The whole thing can also be done in the console with the `config` command.

If the printer is not already connected, it reboots once after saving the data and it should connect to the MQTT.

When the connection is established, you can add `/info` to the ESP32 IP to get to the info page  
  
  ## Hardware Infos
  ESP32 Modul, tested with NodeMCU ESP32 but should work with others.  
  I will switch to wemos esp32 in future.  
    
    Pin 16 is connected with WS2812B Data Pin.


### For more infos and help see my discord at: [http://discord.vr6syncro.de](http://discord.vr6syncro.de)


## License
This project is licensed under the "Attribution-NonCommercial-ShareAlike 4.0 International" License. See the [LICENSE](LICENSE) file for more details.

