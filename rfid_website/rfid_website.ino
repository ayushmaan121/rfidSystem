#include <ESP8266WiFi.h>
#include "HTTPSRedirect.h"
const char* ssid = "No";
const char* password = "connected2";
const int httpPort = 80;
int count = 0,a=0,i,len,num=0;                                          // count = 0
char input[12];
WiFiClient client;
const char* host = "dicuiet.000webhostapp.com";
void setup() {
  Serial.begin(9600);
  Serial.println("Connecting to wifi: ");
  Serial.println(ssid);
  Serial.flush();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" IP address: ");
  Serial.println(WiFi.localIP()); 
  Serial.print(String("Connecting to "));
  Serial.println(host);
  bool flag = false;
  for (int i=0; i<5; i++){
    int retval = client.connect(host, httpPort);
    if (retval == 1) {
       flag = true;
       break;
    }
    else
      Serial.println("Connection failed. Retrying...");
  }
  Serial.println("Connection Status: " + String(client.connected()));
  Serial.flush(); 
  if (!flag){
    Serial.print("Could not connect to server: ");
    Serial.println(host);
    Serial.println("Exiting...");
    Serial.flush();
    return;
  }
  // Data will still be pushed even certification don't match.
}
void loop(){  
  if(Serial.available())
   {
      count = 0;
      num = 0;
      while(Serial.available() && count < 12)
      {
         input[count] = Serial.read();
         count++;
         delay(5);  
      }
     if(a%2==0)
     {
      Serial.println("tag id to be sent:");
      Serial.println(input);
      len = strlen(input);
      for(i=0;i<len;i++){
        if (input[i]<=57 && input[i]>=48){num = num*10+(input[i]-'0');}
        }
      Serial.println(num);
      input[0]='\0';          
      sendToServer(num);
     }
      a++;
   }
}
void sendToServer(int tag){
  //char* username="ayushmaan";
  Serial.print("connecting to ");
  Serial.println(host);
  while(!client.connect(host, httpPort)) {
    Serial.print(".");
   }
  String url = "/rfid/insert.php?tag=" + String(tag);
  Serial.println("URL Requested:");
  Serial.println(url);
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(500);  
  while(client.available()){
    String disp = client.readStringUntil('\r');
    Serial.print(disp);
  }
  Serial.println();
  Serial.println("closing connection");
}
