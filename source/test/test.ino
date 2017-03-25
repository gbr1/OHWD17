#include <ESP8266WiFi.h>           //header per set istruzioni WiFi

//istruzioni eseguite una sola volta ad avvio
void setup() {
  WiFi.mode(WIFI_OFF);		       //chiude la connsessione ad airport e il soft airport
  pinMode(LED_BUILTIN, OUTPUT);    //il led blu sulla scheda è OUTPUT - non usare in gara
}

//istruzioni da ripetere all'infinito
void loop() {
  digitalWrite(LED_BUILTIN, LOW);  //led spento	
  delay(1000);					   //aspetto 1s
  digitalWrite(LED_BUILTIN, HIGH); //led acceso
  delay(1000);					   //aspetto 1s
}