#include <ESP8266WiFi.h>      // Biblioteca para conectar o ESP8266 à rede Wi-Fi
#include <PubSubClient.h>     // Biblioteca para comunicação MQTT

// Credenciais da rede Wi-Fi
char ssid[] = "Rodolfo_Roteador";     // Nome da rede Wi-Fi
char pass[] = "rodolfoinstrutor";     // Senha da rede Wi-Fi

// Configuração do servidor MQTT (Broker)
const char* IP_do_servidor = "10.140.4.42"; // IP do broker MQTT
int Porta_do_servidor = 1883;               // Porta padrão do protocolo MQTT

// Objetos necessários para a comunicação Wi-Fi e MQTT
WiFiClient cliente_wifi;                   // Cliente Wi-Fi
PubSubClient MQTT(cliente_wifi);           // Cliente MQTT que usa o cliente Wi-Fi

void setup() 
{
  // Configura o pino 2 (geralmente o LED do ESP8266) como saída e o desliga (nível alto)
  pinMode(2, OUTPUT);
  digitalWrite(2, 1);

  Serial.begin(9600);                      // Inicializa a comunicação serial para debug

  // Conecta o ESP8266 à rede Wi-Fi
  WiFi.begin(ssid, pass);                  // Inicia a conexão Wi-Fi
  while(WiFi.status() != WL_CONNECTED)     // Aguarda até conectar
  {
    delay(1000);
  }

  // Configura o servidor MQTT
  MQTT.setServer(IP_do_servidor, Porta_do_servidor); // Define o IP e a porta do broker MQTT
  MQTT.setCallback(resposta);            // Define a função de callback que será chamada ao receber mensagens

  // Conecta ao broker MQTT
  while(!MQTT.connect("RodolfoID"))      // Tenta se conectar com o ID "RodolfoID"
  {
    delay(1000);                         // Aguarda antes de tentar novamente
  }

  // Inscreve-se no tópico "Led1_rodolfo" para receber comandos de controle do LED
  MQTT.subscribe("Led1_rodolfo");

  Serial.println("Mqtt conectado");      // Mensagem indicando conexão bem-sucedida
}

void loop() 
{
  delay(3000);                            // Aguarda 3 segundos entre iterações
  MQTT.loop();                            // Mantém a conexão com o broker ativa (keep-alive)
  MQTT.publish("topico_teste", "OLA");   // Publica a mensagem "OLA" no tópico "topico_teste"
}

// Função chamada sempre que uma mensagem é recebida em um dos tópicos inscritos
void resposta(char* topico, byte* conteudo, unsigned int comprimento) 
{
  String minha_string;                    // String para armazenar a mensagem recebida

  // Converte o conteúdo da mensagem de byte para String
  for(int i = 0; i < comprimento; i++)
  {
    minha_string = minha_string + (char)conteudo[i];
  }

  // Verifica o conteúdo da mensagem e aciona o LED de acordo
  if(minha_string == "on")                // Se a mensagem for "on"
  {
    digitalWrite(2, 0);                   // Liga o LED (nível baixo)
  }
  else if(minha_string == "off")          // Se a mensagem for "off"
  {
    digitalWrite(2, 1);                   // Desliga o LED (nível alto)
  }
}
