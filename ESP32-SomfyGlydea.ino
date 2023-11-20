#include <WiFi.h>

#include <HTTPUpdateServer.h>

#include <EEPROM.h>
#define EEPROM_SIZE 1

#include <ESP32-Secrets.h>

#define GPIO_OPEN 2
#define GPIO_CLOSE 3
#define GPIO_MYPOSITION 18

#define CURTAIN_UNKNOWN 0
#define CURTAIN_CLOSE 1
#define CURTAIN_OPEN 2
#define CURTAIN_MYPOSITION 3
byte curtain_status = 0;

// -------------------------------------------------------------------

#define SERVERPORT 80
WebServer server(SERVERPORT);
HTTPUpdateServer httpUpdater;

#define WEBPAGESIZE 2048
char webpage[WEBPAGESIZE];

// -------------------------------------------------------------------
void setup() {
  Serial.begin(115200);

  EEPROM.begin(EEPROM_SIZE);
  curtain_status = EEPROM.read(0);

  pinMode(GPIO_CLOSE, INPUT);
  pinMode(GPIO_OPEN, INPUT);
  pinMode(GPIO_MYPOSITION, INPUT);

  Serial.print("Connecting to ");
  Serial.println(WIFI_NAME);
  WiFi.begin(WIFI_NAME, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print(" done!\nSignal strength: ");
  Serial.println(String(WiFi.RSSI()));
  Serial.print("Webserver started at http://");
  Serial.println(WiFi.localIP());

  server.on("/", handlewebpage_root);
  server.on("/status", handlewebpage_status);
  server.on("/open", handlewebpage_open);
  server.on("/close", handlewebpage_close);
  server.on("/myposition", handlewebpage_myposition);
  server.begin();
  httpUpdater.setup(&server, ota_path, ota_username, ota_password);
}

// -------------------------------------------------------------------
void loop() {

  server.handleClient();

  delay(5);
}

// -------------------------------------------------------------------
void handlewebpage_root() {

  SendHTML((curtain_status == CURTAIN_OPEN), (curtain_status == CURTAIN_CLOSE), (curtain_status == CURTAIN_MYPOSITION));
}

// -------------------------------------------------------------------
void handlewebpage_open() {

  curtain_status = CURTAIN_OPEN;
  EEPROM.write(0, curtain_status);
  EEPROM.commit();

  pinMode(GPIO_OPEN, OUTPUT);
  pinMode(GPIO_MYPOSITION, INPUT);
  pinMode(GPIO_CLOSE, INPUT);
  digitalWrite(GPIO_OPEN, LOW);
  delay(200);
  pinMode(GPIO_OPEN, INPUT);

  SendHTML((curtain_status == CURTAIN_OPEN), (curtain_status == CURTAIN_CLOSE), (curtain_status == CURTAIN_MYPOSITION));
}

// -------------------------------------------------------------------
void handlewebpage_close() {

  curtain_status = CURTAIN_CLOSE;
  EEPROM.write(0, curtain_status);
  EEPROM.commit();

  pinMode(GPIO_OPEN, INPUT);
  pinMode(GPIO_MYPOSITION, INPUT);
  pinMode(GPIO_CLOSE, OUTPUT);
  digitalWrite(GPIO_CLOSE, LOW);
  delay(200);
  pinMode(GPIO_CLOSE, INPUT);

  SendHTML((curtain_status == CURTAIN_OPEN), (curtain_status == CURTAIN_CLOSE), (curtain_status == CURTAIN_MYPOSITION));
}

// -------------------------------------------------------------------
void handlewebpage_myposition() {

  curtain_status = CURTAIN_MYPOSITION;
  EEPROM.write(0, curtain_status);
  EEPROM.commit();

  pinMode(GPIO_OPEN, INPUT);
  pinMode(GPIO_MYPOSITION, OUTPUT);
  pinMode(GPIO_CLOSE, INPUT);
  digitalWrite(GPIO_MYPOSITION, LOW);
  delay(200);
  pinMode(GPIO_MYPOSITION, INPUT);

  SendHTML((curtain_status == CURTAIN_OPEN), (curtain_status == CURTAIN_CLOSE), (curtain_status == CURTAIN_MYPOSITION));
}

// -------------------------------------------------------------------
void handlewebpage_status() {

  if (curtain_status == CURTAIN_OPEN) { snprintf(webpage, WEBPAGESIZE, "<!DOCTYPE HTML><html>Open</html>"); }
  if (curtain_status == CURTAIN_CLOSE) { snprintf(webpage, WEBPAGESIZE, "<!DOCTYPE HTML><html>Close</html>"); }
  if (curtain_status == CURTAIN_MYPOSITION) { snprintf(webpage, WEBPAGESIZE, "<!DOCTYPE HTML><html>MyPosition</html>"); }

  server.send(200, "text/html", webpage);
}

// -------------------------------------------------------------------
void SendHTML(uint8_t open, uint8_t close, uint8_t myposition) {

  snprintf(webpage, WEBPAGESIZE, " \
  <!DOCTYPE html> <html> \
  <head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\"> \
  <title>Curtain Control</title> \
  <style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;} \
  body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;} \
  .button {display: block;width: 150px;background-color: #3498db;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;} \
  .button-1 {background-color: #3498db;} \
  .button-1:active {background-color: #2980b9;} \
  .button-0 {background-color: #34495e;} \
  .button-0:active {background-color: #2c3e50;} \
  p {font-size: 14px;color: #888;margin-bottom: 10px;} \
  </style> \
  </head> \
  <body> \
  <h1><a href=""/"">Curtain Web Server</a></h1> \
  <a class=\"button button-%d\" href=\"/open\">Open</a> \
  <a class=\"button button-%d\" href=\"/close\">Close</a> \
  <a class=\"button button-%d\" href=\"/myposition\">My Position</a> \
  </body></html>\n", open, close, myposition);
  server.send(200, "text/html", webpage);
}
