/*
 *  This sketch displays the difference between two dates on 3 x 7 segment led screens
 *
 */

#include <ESP8266WiFi.h>

#include <Wire.h>
#include <TimeLib.h>
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"

Adafruit_LEDBackpack matrix = Adafruit_LEDBackpack();

// *******************************************************
//     Setup
// *******************************************************

const char* ssid     = "wifi_ssid";
const char* password = "wifi_password";
String compare_date = "2016-06-18";
int minutes_between_refresh = 30;

// *******************************************************
// *******************************************************

const char* host = "www.codenamefish.co.uk";
int sleep_minutes = 1;


void displayCharacter(int offset, int character, int number){

  offset = offset + 4 - character;
  
  if (number == 0){
    matrix.displaybuffer[0] |= 0x0001 << offset;
    matrix.displaybuffer[1] |= 0x0001 << offset;
    matrix.displaybuffer[2] |= 0x0001 << offset;
    matrix.displaybuffer[3] |= 0x0001 << offset;
    matrix.displaybuffer[4] |= 0x0001 << offset;
    matrix.displaybuffer[5] |= 0x0001 << offset;
  }

  if (number == 1){
    matrix.displaybuffer[4] |= 0x0001 << offset;
    matrix.displaybuffer[5] |= 0x0001 << offset;
  }

  if (number == 2){
    matrix.displaybuffer[0] |= 0x0001 << offset;
    matrix.displaybuffer[1] |= 0x0001 << offset;
    matrix.displaybuffer[3] |= 0x0001 << offset;
    matrix.displaybuffer[4] |= 0x0001 << offset;
    matrix.displaybuffer[6] |= 0x0001 << offset;
  }

  if (number == 3){
    matrix.displaybuffer[0] |= 0x0001 << offset;
    matrix.displaybuffer[3] |= 0x0001 << offset;
    matrix.displaybuffer[4] |= 0x0001 << offset;
    matrix.displaybuffer[5] |= 0x0001 << offset;
    matrix.displaybuffer[6] |= 0x0001 << offset;
  }

  if (number == 4){
    matrix.displaybuffer[2] |= 0x0001 << offset;
    matrix.displaybuffer[4] |= 0x0001 << offset;
    matrix.displaybuffer[5] |= 0x0001 << offset;
    matrix.displaybuffer[6] |= 0x0001 << offset;
  }

  if (number == 5){
    matrix.displaybuffer[0] |= 0x0001 << offset;
    matrix.displaybuffer[2] |= 0x0001 << offset;
    matrix.displaybuffer[3] |= 0x0001 << offset;
    matrix.displaybuffer[5] |= 0x0001 << offset;
    matrix.displaybuffer[6] |= 0x0001 << offset;
  }

  if (number == 6){
    matrix.displaybuffer[0] |= 0x0001 << offset;
    matrix.displaybuffer[1] |= 0x0001 << offset;
    matrix.displaybuffer[2] |= 0x0001 << offset;
    matrix.displaybuffer[3] |= 0x0001 << offset;
    matrix.displaybuffer[5] |= 0x0001 << offset;
    matrix.displaybuffer[6] |= 0x0001 << offset;
  }

  if (number == 7){
    matrix.displaybuffer[3] |= 0x0001 << offset;
    matrix.displaybuffer[4] |= 0x0001 << offset;
    matrix.displaybuffer[5] |= 0x0001 << offset;
  }

  if (number == 8){
    matrix.displaybuffer[0] |= 0x0001 << offset;
    matrix.displaybuffer[1] |= 0x0001 << offset;
    matrix.displaybuffer[2] |= 0x0001 << offset;
    matrix.displaybuffer[3] |= 0x0001 << offset;
    matrix.displaybuffer[4] |= 0x0001 << offset;
    matrix.displaybuffer[5] |= 0x0001 << offset;
    matrix.displaybuffer[6] |= 0x0001 << offset;
  }

  if (number == 9){
    matrix.displaybuffer[0] |= 0x0001 << offset;
    matrix.displaybuffer[2] |= 0x0001 << offset;
    matrix.displaybuffer[3] |= 0x0001 << offset;
    matrix.displaybuffer[4] |= 0x0001 << offset;
    matrix.displaybuffer[5] |= 0x0001 << offset;
    matrix.displaybuffer[6] |= 0x0001 << offset;
  }

}


void displayNumber(int screen, int number){

  int offset = (screen-1) * 4;

  displayCharacter(offset, 1, (number%10000)/1000);
  displayCharacter(offset, 2, (number%1000)/100);
  displayCharacter(offset, 3, (number%100)/10);
  displayCharacter(offset, 4, number%10);
    
}



void clearScreenBuffer(){

  matrix.displaybuffer[0] = 0x0000;
  matrix.displaybuffer[1] = 0x0000;
  matrix.displaybuffer[2] = 0x0000;
  matrix.displaybuffer[3] = 0x0000;
  matrix.displaybuffer[4] = 0x0000;
  matrix.displaybuffer[5] = 0x0000;
  matrix.displaybuffer[6] = 0x0000;
  matrix.displaybuffer[7] = 0x0000;
  
}

void writeToScreen(){
  matrix.writeDisplay();
}



void setup() {
  Serial.begin(115200);
  delay(10);
  
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  matrix.begin(0x70);  // pass in the address

  displayNumber(1, 0);
  displayNumber(2, 0);
  displayNumber(3, 0);

  writeToScreen();
  
}

int value = 0;




void loop() {

  clearScreenBuffer();

  int counter = 0;
  String date = "";

  delay(5000);

  Serial.print("connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  String url = "/compare_dates";
  url += "?date=";
  url += compare_date;
  url += "&timezone=";
  url += "EST";
  
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "User-Agent: runscope/0.1" + 
               "Connection: close\r\n\r\n");
  delay(5000);

  String line;
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    line = client.readStringUntil('\r');
    Serial.print(line);

    counter++;

    //if this is the line we want, save it into the date variable
    if (counter == 10){
      date = line;
    }
    
  }

  //if we have a date
  if (date != ""){
  
    String string_year = date.substring(1, 3);
    String string_month = date.substring(4, 6);
    String string_day = date.substring(7, 9);

    int int_year = string_year.toInt();
    int int_month = string_month.toInt();
    int int_day = string_day.toInt();

    //display our results
    displayNumber(1, int_year);
    displayNumber(2, int_month);
    displayNumber(3, int_day);
    writeToScreen();

    //once we have 1 good reading, we can revert to our main sleep time
    sleep_minutes = minutes_between_refresh;

  }

  //wait until we make the request again
  delay(60000 * sleep_minutes);
}


