//File: MyMQTTTempSensorV3.ino
//
//by: sarodp@yahoo.com 22/08/2016
//=============================================
//
// additional libraries
//
// #esp8266wifi.h
// File-->Preferences-->Settings--> Additonal Boards Manger URLs: 
//   http://arduino.esp8266.com/stable/package_esp8266com_index.json
//
// #pubsubclient.h
//  $ wget https://github.com/knolleary/pubsubclient/archive/master.zip
//
//====================================================================
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

//define D0...D7
#define LEDFAULT D2
#define LEDMQTT D3
#define SWOFF HIGH
#define SWON LOW

//define Thermister ==============================================
// GND--Rntc---Rseries---+3.3V
//           |
//           A0 PIN
//----------------------------------------------
// analog pin to connect
#define THERMISTORPIN A0         
// series resistor to 3.3V 
#define RSERIES 2000    
// NTC resistance at 25 degrees C
#define RNTCNOMINAL 8000      
// NTC temp. at nominal resistance (almost always 25 C)
#define TNTCNOMINAL 25   
// how many samples to take and average, more takes longer
// but is more 'smooth'
#define NUMSAMPLES 5
// The beta coefficient of the thermistor (usually 3000-4000)
#define NTCBETA 4600

 
// WIFI const  =====================================
// Update these with values suitable for your network.
const char* ssid = "MYDLINK";
const char* password = "20572057";
const char* mqtt_broker0 = "192.168.1.6";

const char* client_name = "nodeMCU01";
const char* topicOut0 = "/z1/r1/t1";
const char* topicIN = "/z1/r1/cmd/#";
const char* topicD0 = "/z1/r1/cmd/d0";
const char* topicD1 = "/z1/r1/cmd/d0";
const char* topicD2 = "/z1/r1/cmd/d0";
const char* topicD3 = "/z1/r1/cmd/d0";

//Var's ======================================
WiFiClient espClient;
PubSubClient client(espClient);

void setup(void) {
//0.init ADC  
// analogReference(EXTERNAL);

//1.Initialize pins as an output
  pinMode(D0, OUTPUT);     
  pinMode(D1, OUTPUT);     
  pinMode(LEDFAULT, OUTPUT);     
  pinMode(LEDMQTT, OUTPUT);  

  digitalWrite(D0, SWOFF);
  digitalWrite(D1, SWOFF);
  digitalWrite(LEDFAULT, SWON);
  digitalWrite(LEDMQTT, SWOFF);

//2.init serial0
  Serial.begin(115200);
  
//3.init wifi 
  setup_wifi();

//4.init mqtt
  setupMQTTauto();
//client.setServer(mqtt_broker0, 1883);
//client.setCallback(mqtt_callback);  
}

   
void setup_wifi() {
 //0-----------
  delay(100);
  
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  //1---We start by connecting to a WiFi network
  WiFi.begin(ssid, password);

  //1a...wait until connected....
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  //2--connected ok
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("Device name:");
  Serial.println(client_name);
  Serial.print("IP address:");
  Serial.println(WiFi.localIP());
  Serial.println("");
}


void setupMQTTauto() {
 int xi = 20;
 char xbrokerIP[20];

 do {  
  //1--xbrokerIP
    xi = xi + 1;
    if (xi > 254)  {
	   xi = 1;
	  }
	sprintf(xbrokerIP, "192.168.1.%d", xi);
  //2--.set...  
	client.setServer(xbrokerIP, 1883);
    client.setCallback(mqtt_callback);  
  //*/ 
  //3a-- Attempt to connect message
  Serial.print("Attempting MQTT.connect #");
  Serial.println(xi);
  Serial.print( "Broker IP:" );
  Serial.println(xbrokerIP);
  
  //3b--.connect..
    if (!client.connect(client_name)) {
      Serial.print(">FAILED, rc=");
      Serial.println(client.state());
      Serial.println(" ");
	  Serial.println(">try again in 2 seconds");
	  }

	//4--wait 2 seconds before retrying
  delay(2000);
 } while (!client.connected());

 //99--doneOK
      Serial.println("Connected OK.");
      Serial.println("==========================================");
      digitalWrite(LEDFAULT, SWOFF);
	  
      // Once connected, publish an announcement...
      client.publish(client_name, "hello world");
      
	  // ... and resubscribe
      client.subscribe(topicIN);

}

  
void mqtt_callback(char* topic, byte* payload, unsigned int length) {
//1a--print topic
  Serial.print("<<<<<<<<<<<<<<<<< Message arrived:[");
  Serial.print(topic);
  Serial.print("]");
  
//1b--print payload  
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

//2--Switch Dx...
//2a-payload[0] =cmdOn/off LED
  String strA;
   strA = String(topic);

//2b-calc xSW
 //default = off
 int xSW = SWOFF;   
 char* xchSW = "OFF";
 
  if ((char)payload[0] == '1') {
    xSW = SWON;
    xchSW = "ON";
  }

//2c-digitalwrite...
 char charDx;
 int  xpos = 12;   // /z1/r1/cmd/dx
     charDx= topic[xpos];
 
 switch (charDx) {
    case '0':
      digitalWrite(D0, xSW);
    break;
    case '1':
      digitalWrite(D1, xSW);
    break;
    case '2':
      digitalWrite(D2, xSW);
    break;
    case '3':
      digitalWrite(D3, xSW);
    break;
 }
 
 //2d--print...
 Serial.print(">>>>>>>>>>>>> set D");
 Serial.print(charDx);
 Serial.print("=");
 Serial.println(xchSW);

 }


void mqttReconnect() {
// Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT reconnect");
    Serial.print("...");
    //--- Attempt to connect
    if (client.connect(client_name)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(client_name, "hello world");
      // ... and resubscribe
      client.subscribe(topicIN);
    } else {
      Serial.print("failed, rc=");
      Serial.println(client.state());
      Serial.println(">try again in 3 seconds");
      // Wait 3 seconds before retrying
      delay(3000);
    }
  }
}


// MAIN PROGRAM ======================================= 
void loop(void) {
 //m1---
  float xvalue;
   xvalue = readTemperature();

  //m2---
  char* xtopic;
   xtopic = "/z1/r1/t1";
   mqttPublish(xtopic, xvalue);
   doblink(D3,10,10);
 
 //m99---
  delay(1000);
}

void doblink(int xpin, int xmsec, int xcount) {
//blink xpinLED for xmsec
 for(int i = 1; i < xcount; ++i) {
   digitalWrite(xpin, SWON);
   delay(xmsec);
   digitalWrite(xpin, SWOFF);
   delay(xmsec); 
 }
}

void mqttPublish(char* xtopic, float xvalue) {
//00---------------------------------------------------------------------------------
// http://stackoverflow.com/questions/27651012/arduino-sprintf-float-not-formatting
//Use this method to convert it to a C-Style string and then use sprintf, eg:
//char *dtostrf(double val, signed char width, unsigned char prec, char *s)
//    dtostrf(xvalue, 4, 2, str_temp);
//    4 is mininum width, 2 is precision, float value is copied onto str_temp
//-----------------------------------------------------------------------------------

//1a-- MQTT connect
  if (!client.connected()) {
    mqttReconnect();
  }
  client.loop();

//1b--MQTT publish
  char xmsg[50]; 
  char str_temp[6];
    dtostrf(xvalue, 3, 1, str_temp);
    sprintf(xmsg,"T1=%sC", str_temp);
    
	Serial.print(">>>>> Publish:[");
  Serial.print(client_name);
  Serial.print("][");
  Serial.print(xtopic);
	Serial.print("]");
    Serial.println(xmsg);
    Serial.println("");
    client.publish(xtopic, xmsg);
  
}

 
float readTemperature(void) {
  uint8_t i;
  float xval;
  float steinhart;
  int samples[NUMSAMPLES];
 
  //m1-- take N samples in a row, with a slight delay
  for (i=0; i< NUMSAMPLES; i++) {
   samples[i] = analogRead(THERMISTORPIN);
   delay(10);
  }
 
  //m1---xval=average all the samples out
  xval = 0;
  for (i=0; i< NUMSAMPLES; i++) {
     xval += samples[i];
  }
     xval /= NUMSAMPLES;

  //m2---print average reading
  Serial.print("Average analog reading="); 
  Serial.println(xval);

  //m3---xval = resistance
  // convert the value to resistance
  // (xRES+SERIESRESISTOR)/XRES = 1023/xReading 
  // 1+SERIESRESISTOR/XRES = 1023/xReading 
  // SERIESRESISTOR/XRES = 1023/xReading -1
                        
   xval = 1023 / xval -1; 
   xval = RSERIES / xval;
  
  Serial.print("Thermistor resistance="); 
  Serial.println(xval);

  //m4---calc temperature
 
  steinhart = xval / RNTCNOMINAL;     // (R/Ro)
  steinhart = log(steinhart);                  // ln(R/Ro)
  steinhart /= NTCBETA;                   // 1/B * ln(R/Ro)
  steinhart += 1.0 / (TNTCNOMINAL + 273.15); // + (1/To)
  steinhart = 1.0 / steinhart;                 // Invert
  steinhart -= 273.15;                         // convert to C
 
  Serial.print("Temperature="); 
  Serial.print(steinhart,1);
  Serial.println("C");
 
  //m99---
  return steinhart;
}

