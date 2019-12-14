/** Handle root or redirect to captive portal */
void handleRoot() {
  //if (captivePortal()) { // If captive portal redirect instead of displaying the page.
  //  return;
  //}
  //iphoneportal();
 // server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
 // server.sendHeader("Pragma", "no-cache");
 // server.sendHeader("Expires", "-1");
 // server.setContentLength(CONTENT_LENGTH_UNKNOWN);
 // server.send(200, "text/html", ""); // Empty content inhibits Content-length header so we have to close the socket ourselves.
  server.sendContent(
    "<html><head></head><body>"
    "<h1>Success</h1>"
  );
  if (server.client().localIP() == apIP) {
    server.sendContent(String("<p>You are connected through the soft AP: ") + softAP_ssid + "</p>");
  } else {
    server.sendContent(String("<p>You are connected through the wifi network: ") + ssid + "</p>");
  }
  server.sendContent(
    "<p>You may want to <a href='/time'>1. configure Time</a>.</p>"
    "<p>You may want to <a href='/wifi'>2. configure Wifi</a>.</p>"
    "</body></html>"
  );
//  server.client().stop(); // Stop is needed because we sent no content length
}

/** Redirect to captive portal if we got a request for another domain. Return true in that case so the page handler do not try to handle the request again. */
boolean captivePortal() {
  //if (!isIp(server.hostHeader()) && server.hostHeader() != (String(myHostname)+".local")) {
    //Serial.println("Request redirected to captive portal here");
    //server.sendHeader("Location", String("http://") + toStringIp(server.client().localIP()), true);
    //server.setContentLength(CONTENT_LENGTH_UNKNOWN);
    //server.send ( 302, "text/plain", ""); // Empty content inhibits Content-length header so we have to close the socket ourselves.
    //server.client().stop(); // Stop is needed because we sent no content length
    iphoneportal();
    //return true;
  //}
  return false;
}

void iphoneportal() {
Serial.println("response for iphone");
server.setContentLength(CONTENT_LENGTH_UNKNOWN);
server.sendContent(
//"<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2//EN">"
"<HTML>"
"<HEAD>"
"    <TITLE>hereiam</TITLE>"
"</HEAD>"
"<BODY>"
"hereiam"
"</BODY>"
"</HTML>");
}

/** Wifi config page handler */
void handleWifi() {
  //server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  //server.sendHeader("Pragma", "no-cache");
  //server.sendHeader("Expires", "-1");
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  //server.send(200, "text/html", ""); // Empty content inhibits Content-length header so we have to close the socket ourselves.
  server.sendContent(
    "<html><head></head><body>"
    "<h1>config</h1>"
  );

  /*timeserver config
  server.sendContent(
    "<form method='POST' action='timesave'><h4>Time Settings:</h4>"
    );
  server.sendContent(String() + "<label><input type='text' value='" + wTimezone + "' name='tz'/>TimeZone UTC=0 </label>");
  server.sendContent(String() + "<br /><label><input type='text' value='" + TimeServer + "' name='tserv'/>ntp Server Name or IP address</label>");
  server.sendContent(   
    "<br /><label><input type='Checkbox' placeholder='Daylight Savings Time' name='dst' value='on'/>DST Checked = tz +1 </label>"
    "<br /><input type='submit' value='Save Time Settings'/></form>");
   End timeserver config */
  
  if (server.client().localIP() == apIP) {
    server.sendContent(String("<p>You are connected through the soft AP: ") + softAP_ssid + "</p>");
  } else {
    server.sendContent(String("<p>You are connected through the wifi network: ") + ssid + "</p>");
  }
  server.sendContent(
    "\r\n<br />"
    "<table><tr><th align='left'>SoftAP config</th></tr>"
  );
  server.sendContent(String() + "<tr><td>SSID " + String(softAP_ssid) + "</td></tr>");
  server.sendContent(String() + "<tr><td>IP " + toStringIp(WiFi.softAPIP()) + "</td></tr>");
  server.sendContent(
    "</table>"
    "\r\n<br />"
    "<table><tr><th align='left'>WLAN config</th></tr>"
  );
  server.sendContent(String() + "<tr><td>SSID " + String(ssid) + "</td></tr>");
  server.sendContent(String() + "<tr><td>IP " + toStringIp(WiFi.localIP()) + "</td></tr>");
  server.sendContent(
    "</table>"
    "\r\n<br />"
    "<table><tr><th align='left'>WLAN list (refresh if any missing)</th></tr>"
  );
  Serial.println("scan start");
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n > 0) {
    for (int i = 0; i < n; i++) {
      server.sendContent(String() + "\r\n<tr><td>SSID " + WiFi.SSID(i) + String((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":" *") + " (" + WiFi.RSSI(i) + ")</td></tr>");
    }
  } else {
    server.sendContent(String() + "<tr><td>No WLAN found</td></tr>");
  }
  server.sendContent(
    "</table>"
    "<form method='POST' action='wifisave'><h4>Connect to network:</h4>"
    "<input type='text' placeholder='network' name='n'/>"
    "<br /><input type='password' placeholder='password' name='p'/>"
    "<br /><input type='submit' value='Connect/Disconnect'/></form>"   
    "</table>" );
  

  
  // begin end page
  server.sendContent("<br /><p><a href='/time'>Configure Time Server</a>.</p>");
  server.sendContent("<br /><p><a href='/'>return to home</a>.</p>"
    "</body></html>"
  );
  server.client().stop(); // Stop is needed because we sent no content length
}

void handletime() {
  //server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  //server.sendHeader("Pragma", "no-cache");
  //server.sendHeader("Expires", "-1");
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  //server.send(200, "text/html", ""); // Empty content inhibits Content-length header so we have to close the socket ourselves.
  server.sendContent(
    "<html><head></head><body>"
    "<h1>config</h1>"
  );

  //timeserver config
  server.sendContent(
    "<form method='POST' action='timesave'><h4>Time Settings:</h4>"
    );
  server.sendContent(String() + "<label><input type='text' value='" + wTimezone + "' name='tz'/> TimeZone UTC=0 www.timeanddate.com </label>");
  server.sendContent(String() + "<br /><label><input type='text' value='" + TimeServer + "' name='tserv'/> ntp Server Name or IP address (us.pool.ntp.org) </label>");
  server.sendContent(   
    "<br /><label><input type='Checkbox' placeholder='Daylight Savings Time' name='dst' value='on'/>DST Checked = tz +1 </label>"
    "<br /><input type='submit' value='Save Time Settings'/></form>");
  // End timeserver config
  
  
  // begin end page
  server.sendContent("<br /><p><a href='/wifi'>Configure Wifi</a>.</p>");
  server.sendContent("<br /><p><a href='/'>return to home</a>.</p>"
    "</body></html>"
  );
  server.client().stop(); // Stop is needed because we sent no content length
}


void handleTimeSave() {
  Serial.println("Time save");
  server.arg("dst").toCharArray(dstactive, sizeof(dstactive) - 1);
  server.arg("tserv").toCharArray(TimeServer, sizeof(TimeServer) - 1);
  server.arg("tz").toCharArray(wTimezone, sizeof(wTimezone) - 1);
  server.sendHeader("Location", "wifi", true);
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send ( 302, "text/plain", "");  // Empty content inhibits Content-length header so we have to close the socket ourselves.
  server.client().stop(); // Stop is needed because we sent no content length
  saveTimezone();
  timesetup();
  //connect = strlen(ssid) > 0; // Request WLAN connect with new credentials if there is a SSID
}

/** Handle the WLAN save form and redirect to WLAN config page again */
void handleWifiSave() {
  Serial.println("wifi save");
  server.arg("n").toCharArray(ssid, sizeof(ssid) - 1);
  server.arg("p").toCharArray(password, sizeof(password) - 1);
  server.sendHeader("Location", "wifi", true);
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send ( 302, "text/plain", "");  // Empty content inhibits Content-length header so we have to close the socket ourselves.
  server.client().stop(); // Stop is needed because we sent no content length
  saveCredentials();
  connect = strlen(ssid) > 0; // Request WLAN connect with new credentials if there is a SSID
}



void handleNotFound() {
  
  //if (captivePortal()) { // If caprive portal redirect instead of displaying the error page.
  //  Serial.println("handlenotfound but captive");
  //  return;
  //}
  iphoneportal();
 Serial.println("go to handleroot");
  //handleRoot(); 
  return;
  
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += ( server.method() == HTTP_GET ) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for ( uint8_t i = 0; i < server.args(); i++ ) {
    message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
  }
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send ( 404, "text/plain", message );
}
