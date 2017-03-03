#include <SPI.h>
#include <Ethernet.h>
#include "ESP8266WiFi.h"

// WiFi parameters
const char* ssid = "Remigio2-4ghz";
const char* password = "vitopina";

// ethernet
//byte mac[] = { 0xD3, 0x4D, 0xB3, 0x3F, 0xF3, 0x3D };
byte ip[] = { 192, 168, 0, 70 };
byte gateway[] = { 192, 168, 0, 1 };
byte subnet[] = { 255, 255, 255, 0 };

// serial connection
int serialBaud = 9600;
int serialCfg = SERIAL_8N1; // Default settings controller has 8 data, 1 stop and no parity.

// socket parameters
int serverPort = 8888;

// start TCP servers
EthernetServer server(serverPort);


void setup() {
  delay(1250);
  Serial1.begin(serialBaud, serialCfg); // Open Serial1 communications
  Serial.begin(serialBaud, serialCfg); // Open Serial communications
  //Ethernet.begin(mac, ip, gateway, subnet); // Start the Ethernet connection
  //Ethernet.begin(mac);

  WiFi.config(ip,gateway,subnet);
  WiFi.begin(ssid, password);

  int i = 0;
  while ((WiFi.status() != WL_CONNECTED ) && (i < 50) ) {
  delay(1000);
  i++;
  Serial1.print("\nAttempting connection - seconds: ");
  Serial1.print(i,DEC);
  if (i == 49) Serial1.print("WiFi connection failed\n");
  }
  Serial1.println("");
  Serial1.println("WiFi connected");
  // Print the IP address
  Serial1.println(WiFi.localIP());

  server.begin(); // Begin listening for TCP connections
//  Serial1.println();
//  delay(250);
//  Serial1.println("Initializing..");
//  delay(2250);
//  Serial1.print("IP address: ");
//  Serial1.println(Ethernet.localIP());
  Serial1.println();

  Serial1.println("Booted system successfully!");
}

void loop() {
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    String clientMsg = "";
    //String daftCode = "";
    while (client.connected()) {

      // Transmit
      while (client.available()) {
        char c = client.read();
        /*Serial1.write(c);
        Serial.write(c);*/
        clientMsg += c; // Store received chars up to newline
        if (c == '\n') {
          Serial1.print(clientMsg); // Then send the message through Serial1
          Serial.print(clientMsg); // Then send the message through Serial
          clientMsg = "";
          Serial1.flush();
          Serial.flush();
        }
      }

      // Receive
      int incomingByte1 = 0; // For incoming serial data
      while (Serial1.available() > 0) { // If data has been received from the serial connection
        incomingByte1 = Serial1.read();
        client.print(char(incomingByte1)); // Print the char data back to the client
        if (char(incomingByte1) == '\n')
          client.flush();
      }

      // Arduino console
      int incomingByte = 0; // For incoming serial data
      while (Serial.available() > 0) { // If data has been received from the serial connection
        incomingByte = Serial.read();
        client.print(char(incomingByte)); // Print the char data back to the client
        if (char(incomingByte) == '\n')
          client.flush();
      }
    }
  }
}
