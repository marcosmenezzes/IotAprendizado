#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Wi-Fi
const char* ssid = "Rodolfo_Roteador";
const char* password = "rodolfoinstrutor";

// MQTT
const char* IP_do_servidor = "10.140.4.80";  // Endereço IP do Broker MQTT
const int Porta_do_servidor = 1883;           // Porta padrão do MQTT

// Pinos dos LEDs
const int led_verde = D0;  // LED Verde
const int led_amarelo = D1; // LED Amarelo
const int led_vermelho = D2; // LED Vermelho

// Tópico para comando de LEDs
const char* topico_comando = "leds/comando";

// Tópico para enviar status
const char* topico_status = "status/led";

WiFiClient espClient;
PubSubClient client(espClient);

// Função para enviar status ao Node-RED
void enviar_status(const char* cor, const char* estado) {
  String mensagem = String(cor) + " " + String(estado);
  client.publish(topico_status, mensagem.c_str());
}

// Callback ao receber mensagem MQTT
void callback(char* topic, byte* payload, unsigned int length) {
  String mensagem;
  for (int i = 0; i < length; i++) {
    mensagem += (char)payload[i];
  }

  Serial.print("Mensagem recebida no tópico ");
  Serial.print(topic);
  Serial.print(": ");
  Serial.println(mensagem);

  // Controlando os LEDs conforme o payload
  if (strcmp(topic, topico_comando) == 0) {
    if (mensagem == "1") {
      digitalWrite(led_vermelho, LOW);   // Liga LED Vermelho
      enviar_status("vermelho", "Ligado");
    } else if (mensagem == "2") {
      digitalWrite(led_vermelho, HIGH);  // Desliga LED Vermelho
      enviar_status("vermelho", "Desligado");
    } else if (mensagem == "3") {
      digitalWrite(led_amarelo, LOW);    // Liga LED Amarelo
      enviar_status("amarelo", "Ligado");
    } else if (mensagem == "4") {
      digitalWrite(led_amarelo, HIGH);   // Desliga LED Amarelo
      enviar_status("amarelo", "Desligado");
    } else if (mensagem == "5") {
      digitalWrite(led_verde, LOW);      // Liga LED Verde
      enviar_status("verde", "Ligado");
    } else if (mensagem == "6") {
      digitalWrite(led_verde, HIGH);     // Desliga LED Verde
      enviar_status("verde", "Desligado");
    }
  }
}

void setup() {
  Serial.begin(9600);

  // Configura os pinos dos LEDs como saída
  pinMode(led_verde, OUTPUT); digitalWrite(led_verde, HIGH);
  pinMode(led_amarelo, OUTPUT); digitalWrite(led_amarelo, HIGH);
  pinMode(led_vermelho, OUTPUT); digitalWrite(led_vermelho, HIGH);

  // Conecta ao Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao Wi-Fi...");
  }
  Serial.println("Wi-Fi conectado!");

  // Conecta ao broker MQTT
  client.setServer(IP_do_servidor, Porta_do_servidor);
  client.setCallback(callback);

  while (!client.connect("ESP8266Client")) {
    Serial.println("Tentando conectar ao MQTT...");
    delay(1000);
  }
  Serial.println("Conectado ao MQTT!");

  // Inscreve-se no tópico de comando dos LEDs
  client.subscribe(topico_comando);
}

void loop() {
  client.loop();  // Mantém a conexão com o broker MQTT
}
