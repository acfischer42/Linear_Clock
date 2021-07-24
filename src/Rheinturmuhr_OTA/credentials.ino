/** Load WLAN credentials from EEPROM */
void loadCredentials() {
  EEPROM.begin(512);
  EEPROM.get(0, ssid);
  EEPROM.get(0+sizeof(ssid), password);
  EEPROM.get(0+sizeof(ssid)+sizeof(password),wTimezone);
  EEPROM.get(0+sizeof(ssid)+sizeof(password)+sizeof(wTimezone),dstactive);
  EEPROM.get(0+sizeof(ssid)+sizeof(password)+sizeof(wTimezone)+sizeof(dstactive),geolocactive);
  EEPROM.get(0+sizeof(ssid)+sizeof(password)+sizeof(wTimezone)+sizeof(dstactive)+sizeof(geolocactive),TimeServer);
   char ok[2+1];
   EEPROM.get(0+sizeof(ssid)+sizeof(password)+sizeof(wTimezone)+sizeof(dstactive)+sizeof(geolocactive)+sizeof(TimeServer), ok);
  //EEPROM.get(0+sizeof(ssid)+sizeof(password)+sizeof(wTimezone)+sizeof(dstactive), ok);
  //EEPROM.get(0+sizeof(ssid)+sizeof(password)+sizeof(wTimezone), ok);
  //EEPROM.get(0+sizeof(ssid)+sizeof(password), ok);
  EEPROM.end();
  if (String(ok) != String("OK")) {
    ssid[0] = 0;
    password[0] = 0;
    wTimezone[0] = 0;
    dstactive[0] = 0;
    geolocactive[0] = 0;
    TimeServer[0] = 0;   
  }
  Serial.println("Recovered credentials:");
  Serial.println(ssid);
  Serial.println(wTimezone);
  Serial.println(dstactive);
  Serial.println(geolocactive);
  Serial.println(strlen(password)>0?"********":"<no password>");
}

/** Store WLAN credentials to EEPROM */
void saveCredentials() {
  Serial.println("Saving credentials:");
  Serial.println(ssid);
  Serial.println(password);
  Serial.println(sizeof(password));
  Serial.println(wTimezone);
  EEPROM.begin(512);
  EEPROM.put(0, ssid);
  EEPROM.put(0+sizeof(ssid), password);
  //EEPROM.put(0+sizeof(ssid)+sizeof(password),wTimezone);
  //EEPROM.put(0+sizeof(ssid)+sizeof(password)+sizeof(wTimezone),dstactive);
  char ok[2+1] = "OK";
  //EEPROM.put(0+sizeof(ssid)+sizeof(password)+sizeof(wTimezone)+sizeof(dstactive), ok);
  //EEPROM.put(0+sizeof(ssid)+sizeof(password)+sizeof(wTimezone), ok);
  EEPROM.put(0+sizeof(ssid)+sizeof(password)+sizeof(wTimezone)+sizeof(dstactive)+sizeof(geolocactive)+sizeof(TimeServer), ok);
  EEPROM.commit();
  EEPROM.end();
}

void saveTimezone() {
  Serial.println("Saving Time Settings:");
  
  Serial.println(wTimezone);
  Serial.println(dstactive);
  Serial.println(geolocactive);
  
    EEPROM.begin(512);
  //EEPROM.put(0, ssid);
  //EEPROM.put(0+sizeof(ssid), password);
  EEPROM.put(0+sizeof(ssid)+sizeof(password),wTimezone);
  EEPROM.put(0+sizeof(ssid)+sizeof(password)+sizeof(wTimezone),dstactive);
  EEPROM.put(0+sizeof(ssid)+sizeof(password)+sizeof(wTimezone)+sizeof(dstactive),geolocactive);
  EEPROM.put(0+sizeof(ssid)+sizeof(password)+sizeof(wTimezone)+sizeof(dstactive)+sizeof(geolocactive), TimeServer);
  char ok[2+1] = "OK";
  EEPROM.put(0+sizeof(ssid)+sizeof(password)+sizeof(wTimezone)+sizeof(dstactive)+sizeof(geolocactive)+sizeof(TimeServer), ok);
  //EEPROM.put(0+sizeof(ssid)+sizeof(password)+sizeof(wTimezone), ok);
  //EEPROM.put(0+sizeof(ssid)+sizeof(password), ok);
  EEPROM.commit();
  EEPROM.end();
}

//remove dstactive for testing
//convert dstactive to string?
