 
//started with http://hjberndt.de/soft/rtcws2812.html
//todo: page to set timezone and activate DST or not
//cleanup code - outsource stuff to subfile

//delete wifi credentials from eeprom?
//save time zone and DST to eeprom


//#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <Adafruit_NeoPixel.h>
//#include <Time.h>
#include <TimeLib.h>
#include <WiFiUdp.h>

// wifi setup
//#include "arduino_secrets.h"
//const char* ssid = SECRET_SSID;
//const char* password =  SECRET_PASS;

char wTimezone[32]="";
char dstactive[5];
/* Don't set this wifi credentials. They are configurated at runtime and stored on EEPROM */
char ssid[32] = "";
char password[32] = "";
char TimeServer[32] = "";
//#include <PubSubClient.h>

//Time via Geolocation
#include "IPGeolocation.h"
String Key = "80ee49f43bb248be977b0c01e87934db";
int lasttimecall;
byte ipsecond; 
byte ipminute;
byte iphour;



// NTP time 
// NTP Servers:
//static const char ntpServerName[] = "us.pool.ntp.org";
 char ntpServerName[32] = "192.168.188.25";
//static const char ntpServerName[] = "time-a.timefreq.bldrdoc.gov";
//static const char ntpServerName[] = "time-b.timefreq.bldrdoc.gov";
//static const char ntpServerName[] = "time-c.timefreq.bldrdoc.gov";
int timeZone;
// int timeZone = 2;     // Central European Time
//const int timeZone = -5;  // Eastern Standard Time (USA)
//const int timeZone = -4;  // Eastern Daylight Time (USA)
//const int timeZone = -8;  // Pacific Standard Time (USA)
//const int timeZone = -7;  // Pacific Daylight Time (USA)


WiFiUDP Udp;
unsigned int localPort = 8888;  // local port to listen for UDP packets

//time_t getNtpTime();
void digitalClockDisplay();
void printDigits(int digits);
void sendNTPpacket(IPAddress &address);

// end NTP time 

// stuff for Captive Portal
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <ESP8266mDNS.h>
#include <EEPROM.h>

/*
 * This example serves a "hello world" on a WLAN and a SoftAP at the same time.
 * The SoftAP allow you to configure WLAN parameters at run time. They are not setup in the sketch but saved on EEPROM.
 * 
 * Connect your computer or cell phone to wifi network ESP_ap with password 12345678. A popup may appear and it allow you to go to WLAN config. If it does not then navigate to http://192.168.4.1/wifi and config it there.
 * Then wait for the module to connect to your wifi and take note of the WLAN IP it got. Then you can disconnect from ESP_ap and return to your regular WLAN.
 * 
 * Now the ESP8266 is in your network. You can reach it through http://192.168.x.x/ (the IP you took note of) or maybe at http://esp8266.local too.
 * 
 * This is a captive portal because through the softAP it will redirect any http request to http://192.168.4.1/
 */

/* Set these to your desired softAP credentials. They are not configurable at runtime */
const char *softAP_ssid = "IOT_ap";
const char *softAP_password = "12345678";

/* hostname for mDNS. Should work at least on windows. Try http://esp8266.local */
const char *myHostname = "clock";



// DNS server
const byte DNS_PORT = 53;
DNSServer dnsServer;

// Web server
ESP8266WebServer server(80);

/* Soft AP network parameters */
IPAddress apIP(192, 168, 4, 1);
IPAddress netMsk(255, 255, 255, 0);


/** Should I connect to WLAN asap? */
boolean connect;

/** Last time I tried to connect to WLAN */
long lastConnectTry = 0;

/** Current WLAN status */
int status = WL_IDLE_STATUS;

// End Captive Portal

// MQTT things
//const char* mqttServer = "postman.cloudmqtt.com";
//const int mqttPort = 14723;
//const char* mqttUser = "zpxhfoma";
//const char* mqttPassword = "sdLaq9UTw168";

//WiFiClient espClient;M
//PubSubClient client(espClient);

// Neopixel setup
#define PIN           D4
#define NUMPIXELS     60 //46

#define ONsec        Color(15,15,15) // 150 mA max for all
#define ONmin     Color(0,0,31) 
#define ONhr     Color(0,15,0)
#define OFF       Color(1,0,0)
#define MARK      Color(15,0,15)
#define ANsec        pixels.ONsec
#define ANmin        pixels.ONmin
#define ANhr        pixels.ONhr
#define AUS       pixels.OFF
#define MARKER    pixels.MARK
#define LED(x,y)  pixels.setPixelColor(x,y)

//LEDS SEC MIN HOUR
byte oneSecond[] = {0,1,2,3,4,5,6,7,8};             //09
byte tenSecond[] = {10,11,12,13,14};                //15
byte oneMinute[] = {16,17,18,19,20,21,22,23,24};    //25
byte tenMinute[] = {26,27,28,29,30};                //31
byte oneHour[]   = {32,33,34,35,36,37,38,39,40};    //41
byte tenHour[]   = {42,43};                         //44
byte Marker[]    = {9,15,25,31,41,44,45,46};      
byte TopMarker[] = {44,46};
byte TopMarkermid[] = {45};

Adafruit_NeoPixel pixels = 
Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

/* ----------------------------------------------------------------end init -------------------------------------------*/


void setup() {
// setup Wifi
//{ Serial.begin(115200);
// WiFi.begin(ssid, password);
//  while (WiFi.status() != WL_CONNECTED) {
//    delay(500);
//    Serial.println("Connecting to WiFi..");
//  }
//  Serial.println("Connected to the WiFi network");

 //setup wifi from Captive Portal
 delay(1000);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Linear Clock V2.0 2019 A.Fischer");
  Serial.println();
  Serial.print("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAPConfig(apIP, apIP, netMsk);
  WiFi.softAP(softAP_ssid, softAP_password);
  delay(500); // Without delay I've seen the IP address blank
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());

  /* Setup the DNS server redirecting all the domains to the apIP */  
  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.start(DNS_PORT, "*", apIP);

  /* Setup web pages: root, wifi config pages, SO captive portal detectors and not found. */
  server.on("/", handleRoot);
  server.on("/wifi", handleWifi);
  server.on("/time", handletime);
  server.on("/wifisave", handleWifiSave);
  server.on("/timesave", handleTimeSave);
  server.on("/generate_204", handleRoot);  //Android captive portal. Maybe not needed. Might be handled by notFound handler.
  server.on("/fwlink", handleRoot);  //Microsoft captive portal. Maybe not needed. Might be handled by notFound handler.
  server.onNotFound ( handleNotFound );
  
  server.begin(); // Web server start
  Serial.println("HTTP server started");
  
  
  loadCredentials(); // Load WLAN credentials from eeprom
  
 
  connect = strlen(ssid) > 0; // Request WLAN connect if there is a SSID


//Neopixel setup 
 pixels.begin(); // Initialize NeoPixel library.
 for(int i=0;i<sizeof(Marker);i++)LED(Marker[i],MARKER);
 //setTime(23,59,00, 1,1,2016); 


 
}


//timesetup
void timesetup() {
//delay(1000);
//timeZone = wTimezone[0]-48;
strcpy(ntpServerName, TimeServer);  
if (String(dstactive) == "on") {
      timeZone=timeZone+1;
     }


Serial.print("IP number assigned by DHCP is ");
  Serial.println(WiFi.localIP());
  Serial.println("Starting UDP");
  Udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(Udp.localPort());
  Serial.println("waiting for sync");
  setSyncProvider(getNtpTime);
  setSyncInterval(300);
// end timesetup  

 //ipgeolocation_query();

 
}


void loop() 
{
 //captive Portal
  if (connect) {
    Serial.println ( "Connect requested" );
    connect = false;
    connectWifi();
    lastConnectTry = millis();
  }
  {
    int s = WiFi.status();
    if (s == 0 && millis() > (lastConnectTry + 60000) ) {
      /* If WLAN disconnected and idle try to connect */
      /* Don't set retry time too low as retry interfere the softAP operation */
      connect = true;
    }
    if (status != s) { // WLAN status change
      Serial.print ( "Status: " );
      Serial.println ( s );
      status = s;
      if (s == WL_CONNECTED) {
        /* Just connected to WLAN */
        Serial.println ( "" );
        Serial.print ( "Connected to " );
        Serial.println ( ssid );
        Serial.print ( "IP address: " );
        Serial.println ( WiFi.localIP() );
        WiFi.softAPdisconnect (true);
        Serial.println ( "Turned AP off since already connected to wifi" );
        
        //timeZone = wTimezone[0]-48;
        //timesetup();
        ipgeolocation_query();
        //if connected then turn softAP off
      
      
       Serial.print ( "IP address: " );
        Serial.println ( WiFi.localIP() );
        // Setup MDNS responder
        if (!MDNS.begin(myHostname)) {
          Serial.println("Error setting up MDNS responder!");
        } else {
          Serial.println("mDNS responder started");
          // Add service to MDNS-SD
          MDNS.addService("http", "tcp", 80);
        }
      } else if (s == WL_NO_SSID_AVAIL) {
        WiFi.disconnect();
      }
    }
  
  }
  // Do work:
 // 
  //DNS
  dnsServer.processNextRequest();
  //HTTP
  server.handleClient();

 // end captive Portal 
  
  
  
  static byte mup,s,so=0,mo=0;
 s=second();
 mup=minute();
 //Serial.begin(115200);
 if(so!=s)
 {so=s;
  Rheinturmuhr();
  }
 else delay(20); 

 if (minute() > (lasttimecall + 480) ) {
ipgeolocation_query();
lasttimecall=minute();
//timesetup();
}

//Serial.println(wTimezone);
//Serial.println(dst);
 
}

void ipgeolocation_query() {
Serial.println("IPgeoloc");
IPGeolocation location(Key);
  IPGeo IPG;
  location.updateStatus(&IPG);
  //Serial.println(IPG.city);
  //Serial.println(IPG.country);
  //Serial.println(IPG.country_code);
  //Serial.println(IPG.tz);
  //Serial.println(IPG.offset);
  Serial.println(location.getResponse());
  timeZone=IPG.offset;
   Serial.println(IPG.Iptime);
  Serial.println(timeZone);
ipsecond = IPG.sIptime.substring(6, 8).toInt();
ipminute = IPG.sIptime.substring(3, 5).toInt();
iphour = IPG.sIptime.substring(0, 2).toInt();
//            Serial.print("Seconds Byte: ");
//            Serial.println(ipsecond);
//             Serial.println(ipminute);
//              Serial.println(iphour);
              setTime(iphour, ipminute,ipsecond, 1,1,2019);
  
}

void Rheinturmuhr()
{int i,s,m,h;
 int s10,s1; //TENSecond ONESecond 
 int m10,m1; //TENMinute ONESecond
 int h10,h1; //Hour ..
 s=second();m=minute();h=hour();
 s1 = s % 10; s10 = s / 10;
 m1 = m % 10; m10 = m / 10;       
 h1 = h % 10; h10 = h / 10;
 
 for(i=0;i<sizeof(oneSecond);i++)
 (s1 <=i?LED(oneSecond[i],AUS):LED(oneSecond[i],ANsec));
 for(i=0;i<sizeof(tenSecond);i++)
 (s10<=i?LED(tenSecond[i],AUS):LED(tenSecond[i],ANsec));
 for(i=0;i<sizeof(oneMinute);i++)
 (m1 <=i?LED(oneMinute[i],AUS):LED(oneMinute[i],ANmin));
 for(i=0;i<sizeof(tenMinute);i++)
 (m10<=i?LED(tenMinute[i],AUS):LED(tenMinute[i],ANmin));
 for(i=0;i<sizeof(oneHour);i++)
 (h1 <=i?LED(oneHour[i],AUS):LED(oneHour[i],ANhr)); 
 for(i=0;i<sizeof(tenHour);i++)
 (h10<=i?LED(tenHour[i],AUS):LED(tenHour[i],ANhr));
 // AF: Blink top lights with the seconds
 for(i=0;i<sizeof(TopMarker);i++)
 (s1 % 2?LED(TopMarker[i],AUS):LED(TopMarker[i],ANsec));
 for(i=0;i<sizeof(TopMarkermid);i++)
 (s1 % 2?LED(TopMarkermid[i],ANsec):LED(TopMarkermid[i],AUS));
 //if (i % 2) { // do something odd }
   
 
 pixels.show(); // showtime
 digitalClockDisplay();
 //Serial.println(wTimezone);
 //Serial.println(dstactive);
 //Serial.println(ssid);
 //Serial.println(password);
}


/* Standard Stuff - don't change */
/*-------- NTP code ----------*/

const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

time_t getNtpTime()
{
  IPAddress ntpServerIP; // NTP server's ip address

  while (Udp.parsePacket() > 0) ; // discard any previously received packets
  Serial.println("Transmit NTP Request");
  // get a random server from the pool
  WiFi.hostByName(ntpServerName, ntpServerIP);
  Serial.print(ntpServerName);
  Serial.print(": ");
  Serial.println(ntpServerIP);
  sendNTPpacket(ntpServerIP);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      Serial.println("Receive NTP Response");
      Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
      unsigned long secsSince1900;
      // convert four bytes starting at location 40 to a long integer
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
    }
  }
  Serial.println("No NTP Response :-(");
  return 0; // return 0 if unable to get the time
}

// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress &address)
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}

void digitalClockDisplay()
{
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(".");
  Serial.print(month());
  Serial.print(".");
  Serial.print(year());
  Serial.println();
}

void printDigits(int digits)
{
  // utility for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if (digits < 10)
    Serial.print('0');
  Serial.print(digits);
}
