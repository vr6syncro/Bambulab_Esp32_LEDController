#ifndef WELCOME_H
#define WELCOME_H


void printWelcomeMessage() {
  Serial.println();
  Serial.println("==================================================");
  Serial.println("       Welcome to Bambulab_ESP32_LEDController       ");
  Serial.println("==================================================");
  Serial.println();
  Serial.println("The device is just starting up, it may take a while");
  Serial.println("until all functions are available.");
  Serial.println();
  Serial.println("If you have questions, come to the Discord:");
  Serial.println("http://discord.vr6syncro.de");
  Serial.println();
  Serial.println("==================================================");
  Serial.println();
}

#endif