// Pac-Man Simulator for ESPresso Lite

#include <ESP8266WiFi.h>
#include <MicroGear.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include "PacMan.h"
#include "Game.h"

const char* ssid     = "trueCS";
const char* password = "cs123456";

#define APPID   "anun"
#define KEY     "AeQJGmkm2Wy4Va5"
#define SECRET  "S96ImTDdQy94oYjMjYgXM2Va5"
#define ALIAS   "piepacman"

ESPert espert;
Game* game = NULL;

WiFiClient client;
AuthClient *authclient;

// {12, 13, 14, 2, 0, A0}
// (left, right, up, down, a, b)

MicroGear microgear(client);

/* If a new message arrives, do this */
void onMsghandler(char *topic, uint8_t* msg, unsigned int msglen) {
    Serial.print("Incoming message --> ");
    char *m = (char *)msg;
    m[msglen] = '\0';
    int s = atoi(m);
    int delaytime = 10;
    if(s==4){
        game->setPressButton(s);
        Serial.print(s);
    }
    
    if(s==5){
        game->setPressButton(s);
        Serial.print(s);
    }
    
    if(s==0){
        game->setPressButton(s);
        Serial.print(s);
    }
    
    if(s==1){
        game->setPressButton(s);
        Serial.print(s);
    }
    
    if(s==2){
        game->setPressButton(s);
        Serial.print(s);
    }
    
    if(s==3){
        game->setPressButton(s);
        Serial.print(s);
    }
    Serial.println();
}

void onFoundgear(char *attribute, uint8_t* msg, unsigned int msglen) {
    Serial.print("Found new member --> ");
    for (int i=0; i<msglen; i++)
        Serial.print((char)msg[i]);
    Serial.println();  
}

void onLostgear(char *attribute, uint8_t* msg, unsigned int msglen) {
    Serial.print("Lost member --> ");
    for (int i=0; i<msglen; i++)
        Serial.print((char)msg[i]);
    Serial.println();
}

/* When a microgear is connected, do this */
void onConnected(char *attribute, uint8_t* msg, unsigned int msglen) {
    Serial.println("Connected to NETPIE...");
    /* Set the alias of this microgear ALIAS */
    microgear.setName(ALIAS);
}

void setup() {

    Serial.begin(115200);
    Serial.println("Starting...");
    
    if (WiFi.begin(ssid, password)) {
        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            Serial.print(".");
        }
    }

    Serial.println("WiFi connected");  
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
//    WiFiManager wifiManager;
//    wifiManager.setTimeout(180);
//    
//    if(!wifiManager.autoConnect("PiePacMan")) {
//      Serial.println("Failed to connect and hit timeout");
//      delay(3000);
//      ESP.reset();
//      delay(5000);
//    }else{
      /* Add Event listeners */
  
      /* Call onMsghandler() when new message arraives */
      microgear.on(MESSAGE,onMsghandler);
  
      /* Call onFoundgear() when new gear appear */
      microgear.on(PRESENT,onFoundgear);
  
      /* Call onLostgear() when some gear goes offline */
      microgear.on(ABSENT,onLostgear);
  
      /* Call onConnected() when NETPIE connection is established */
      microgear.on(CONNECTED,onConnected);
  
      /* Initial with KEY, SECRET and also set the ALIAS here */
      microgear.init(KEY,SECRET,ALIAS);
  
      /* connect to NETPIE to a specific APPID */
      microgear.connect(APPID);
      
  //    espert.oled.clear();
  //    espert.oled.println("Connecting NETPIE...");
      Serial.println("Connecting NETPIE...");
      
      espert.init();
      game = new pacMan::PacMan();
      game->init(&espert);
//    }
}

void loop() {
    game->update();
    game->render();
    /* To check if the microgear is still connected */
    if (microgear.connected()) {
        /* Call this method regularly otherwise the connection may be lost */
        microgear.loop();
    }
    else {
        Serial.println("connection lost, reconnect...");
        microgear.connect(APPID);
    }
}
