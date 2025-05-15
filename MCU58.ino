#include <ESP8266WiFi.h>
char ssid[] = "Rodolfo_Roteador";
char pass[] = "rodolfoinstrutor";
uint8_t statuspin=0;

void setup() {
Serial.begin(9600);
pinMode(2,OUTPUT);
digitalWrite(2,1);  

}

void loop() {
  if(WiFi.status() !=WL_CONNECTED)
  {
    Serial.print("\nTENTANDO  CONECTAR");
    WiFi.begin(ssid, pass);
  
  while (WiFi.status() != WL_CONNECTED)
   {
     delay(1000);
     Serial.print(".");
     digitalWrite(2,statuspin^=1);
    }
  Serial.print("\nCONECTADO A REDE: ");
  Serial.print(ssid);
  Serial.print("\nSeu IP e: ");
  Serial.println(WiFi.localIP());
  digitalWrite(2,0);
  }
  delay(1000);
}
