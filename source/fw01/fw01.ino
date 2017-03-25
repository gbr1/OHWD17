#include <ESP8266WiFi.h>

//led definitions
int l1 = 13;
int l2 = 12;
int l3 = 14;
int l4 = 16;

//charge pin
int cpin = 5;

//wifi ssid and password
#define wifiname "NALUGOHWD17"
#define wifipswd "password"

//string to compose http request response
String wk = "";

//server
WiFiServer server(80);

//time
long time1 = 0; //rising
long time2 = 0; //fallling
long timeR = 0; //reading
long timeL = 0; //blinking

//water level
int level = 0;


void setup() {
  //protect ADC from previous charge
  pinMode(cpin,OUTPUT);
  setTozero();

  //opening serial connection to PC
  Serial.begin(115200);

  //writing team
  Serial.println("\n\nteam NaLUG");
  Serial.println("capitano: \tStallman\nmembri: \tTorvalds\n\tGnu\n\tTux");

  //setup led
  pinMode(l1,OUTPUT);
  pinMode(l2,OUTPUT);
  pinMode(l3,OUTPUT);
  pinMode(l4,OUTPUT);

  //turn off led
  digitalWrite(l1,LOW);
  digitalWrite(l2,LOW);
  digitalWrite(l3,LOW);
  digitalWrite(l4,LOW);
  
  ledcycle();

  //open wifi
  WiFi.disconnect();
  WiFi.mode(WIFI_AP);
  WiFi.softAP(wifiname,wifipswd);
  Serial.println("\n\nWifi opened - IP address: ");
  Serial.println(WiFi.softAPIP());

  ledcycle();

  //strat server
  server.begin();
  Serial.println("HTTP server started");

  
  timeR=millis();
  timeL=millis();
}

void loop() {
  handle_root(); //client check

  //2s measure
  if (millis()-timeR>2000){
    readWater();
    Serial.println(level);
    timeR=millis();
  }
  led();    //led refresh
}

//function tu handle client request
void handle_root(){
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  wk="HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html><head>";
  wk+="<head><title>Open Hardware Day 2017 - challenge</title>";
  //refresh 2s
  wk+="<META HTTP-EQUIV=\"refresh\" CONTENT=\"2\"></head>";

  //team
  wk+="<body><div align=\"center\"><table><tr><th>team NALUG</th></tr>";
  wk+="<tr style=\"color:Red\"><td>Stallman</td></tr>";
  wk+="<tr style=\"color:Blue\"><td>Torvalds</td></tr>";
  wk+="<tr style=\"color:Blue\"><td>Gnu</td></tr>";
  wk+="<tr style=\"color:Blue\"><td>Tux</td></tr></table></div>";

  //level
  wk+="</br></br><div align=\"center\">Level: ";
  if (level==0){
    wk+="empty";
  }
  if (level==1){
    wk+="1st stage";
  }
  if (level==2){
    wk+="2nd stage";
  }
  if (level==3){
    wk+="3rd stage";
  }
  if (level==4){
    wk+="full";
  }
  wk+="</br></br></div>";


  //grafical using a table
  wk+="<div align=\"center\"><table style=\"border: 0.5px solid Blue\">";
  for (int i=0; i<(4-level); i++){
    wk+="<tr><td></br></td></tr>";
  }
  for (int i=0; i<level; i++){
    wk+="<tr bgcolor=\"#BFCFDE\"><td></br></td></tr>";
  }
  wk+="</table></div>";
  
  wk+="</body></html>";
  
  client.print(wk);
  delay(100);
}

//boot led cycle
void ledcycle(){
  digitalWrite(l1,HIGH);
  digitalWrite(l2,LOW);
  digitalWrite(l3,LOW);
  digitalWrite(l4,LOW);
  delay(500);
  digitalWrite(l1,LOW);
  digitalWrite(l2,HIGH);
  digitalWrite(l3,LOW);
  digitalWrite(l4,LOW);
  delay(500);
  digitalWrite(l1,LOW);
  digitalWrite(l2,LOW);
  digitalWrite(l3,HIGH);
  digitalWrite(l4,LOW);
  delay(500),
  digitalWrite(l1,LOW);
  digitalWrite(l2,LOW);
  digitalWrite(l3,LOW);
  digitalWrite(l4,HIGH);
  delay(500);
  digitalWrite(l1,LOW);
  digitalWrite(l2,LOW);
  digitalWrite(l3,LOW);
  digitalWrite(l4,LOW);
}

//function to update level value - water level measurement
void readWater(){
  setTozero();
  time1=0;
  time2=0;
  digitalWrite(cpin,HIGH);
  time1=micros();
  while(analogRead(A0)<995);
  time1=micros()-time1;
  digitalWrite(cpin,LOW);
  time2=micros();
  while(analogRead(A0)>5);
  time2=micros()-time2;
  /*
  Serial.print("\ncharge:\t");
  Serial.print(time1);
  Serial.print("\tdischarge:\t");
  Serial.println(time2);
  */
  if (time2>550000){
    level=0;
  }
  else{
    if (time2>510000){
      level=1;
    }
    else{
      if (time2>475000){
        level=2;
      }
      else{
        if (time2>430000){
          level=3;
        }
        else{
          level=4;        
        }
      }
    }
  }
}

void setTozero(){
  digitalWrite(cpin,LOW);
  while(analogRead(A0)>5);
}

void led(){
  if (level==0){
    if ((millis()-timeL)>1000){
      digitalWrite(l1,!digitalRead(l1));
      digitalWrite(l2,LOW);
      digitalWrite(l3,LOW);
      digitalWrite(l4,LOW);
      timeL=millis();
    }
  }
  if (level==1){
    digitalWrite(l1,HIGH);
    digitalWrite(l2,LOW);
    digitalWrite(l3,LOW);
    digitalWrite(l4,LOW);
  }
  if (level==2){
    digitalWrite(l1,LOW);
    digitalWrite(l2,HIGH);
    digitalWrite(l3,LOW);
    digitalWrite(l4,LOW);
  }
  if (level==3){
    digitalWrite(l1,LOW);
    digitalWrite(l2,LOW);
    digitalWrite(l3,HIGH);
    digitalWrite(l4,LOW);
  }
  if (level==4){
    if ((millis()-timeL)>500){
      digitalWrite(l1,LOW);
      digitalWrite(l2,LOW);
      digitalWrite(l3,LOW);
      digitalWrite(l4,!digitalRead(l4));
      timeL=millis();
    }
  }
}

