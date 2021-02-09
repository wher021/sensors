
#include "WiFiEsp.h"
#include <ArduinoJson.h>

#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"

// set up software serial to allow serial communication to our TX and RX pins
SoftwareSerial Serial1(6, 7);
#endif

// Set  baud rate of so we can monitor output from esp.
// CHANGE THIS TO MATCH YOUR SETTINGS
char ssid[] = "";
char pass[] = "";
int status = WL_IDLE_STATUS;

int reqCount = 0;

char server[] = "192.168.1.229";

// Initialize the Ethernet client object
void setup()
{
  // Open up communications for arduino serial and esp serial at same rate
  Serial.begin(9600);
  Serial1.begin(9600);

  // Initialize the esp module
  WiFi.init(&Serial1);

  // Start connecting to wifi network and wait for connection to complete
  while (status != WL_CONNECTED)
  {
    Serial.print("Conecting to wifi network: ");
    Serial.println(ssid);

    status = WiFi.begin(ssid, pass);
  }

  // Once we are connected log the IP address of the ESP module
  Serial.print("IP Address of ESP8266 Module is: ");
  Serial.println(WiFi.localIP());
  Serial.println("You're connected to the network");
}

void SendMessage()
{
  WiFiEspClient client;
  client.stop();
  if (client.connect(server, 4444)) {
    Serial.println("Connected to server");
    client.println("GET /api/CamServer/pir?order_id=1 HTTP/1.1");
    client.println("content-type: application/json");

    client.print("Host: ");
    client.println(server);
    client.println("Connection: close");
    client.println();

    // if there are incoming bytes available
    // from the server, read them and print them
    while (client.available()) {
      char c = client.read();
      Serial.write(c);
    }
  }
}


void loop()
{
  SendMessage();
  delay(3000);
}
