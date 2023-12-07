#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Iuri Galaxy S20 FE"; // Nome da rede WiFi
const char* password = "eszk3793"; // Senha da rede WiFi

/* CONFIGURAÇÕES DO MQTT*/
const char* mqttServer = "broker.hivemq.com"; // Endereço do Broker MQTT
const int mqttPort = 1883; // Porta TCP do Broker MQTT
const char* mqttUser = "iurinott"; // Usuário MQTT
const char* mqttPassword = "TicoTeco#"; // Senha MQTT

int tempo = 0;
int soma = 0, qtde = 0;

LiquidCrystal_I2C lcd(0x27, 20, 4);

#define pinoAnalogico 32
#define ledVermelho 12
#define ledAmarelo 14
#define ledVerde 27
#define buzzer 23

int segundos = 0;

WiFiClient espClient; // Cliente de Rede WiFi
PubSubClient clientMqtt(espClient); // Cria uma instancia de um cliente MQTT

void callback(char* topic, byte* payload, unsigned int length) {
    String strTopic = String(topic);
    String strPayload = "";

    for (int i = 0; i < length; i++) {
        strPayload += (char)payload[i];
    }

    Serial.println("payload:"+strPayload);

    Serial.print("Uma mensagem chegou no tópico: ");
    Serial.println(topic);

//    if (payload[0] == '0'){
//        Serial.println("\nDesligando luz");
//        digitalWrite(led, LOW);
//    }
//
//    if (payload[0] == '1'){
//        Serial.println("\nLigando luz");
//        digitalWrite(led, HIGH);
//    }
    if( strTopic == "1yG4Lcv0/led") {
          if(strPayload == "1") {
              digitalWrite(ledVerde,LOW);
              digitalWrite(ledAmarelo,LOW);
              digitalWrite(ledVermelho,HIGH);
              Serial.println("Luz Vermelha!");
          } else if (strPayload == "0") {
              digitalWrite(ledVerde,LOW);
              digitalWrite(ledAmarelo,HIGH);
              digitalWrite(ledVermelho,LOW);
              Serial.println("Luz Amarela!");
          }else {
            digitalWrite(ledVerde,HIGH);
              digitalWrite(ledAmarelo,LOW);
              digitalWrite(ledVermelho,LOW);
              Serial.println("Luz Verde!");
          }
    }
    Serial.println();
    Serial.println(" -----------------------");

}

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password); // Configura o WiFi

  while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print("Tentando se conectar na rede: ");
        Serial.println(ssid);
  }

  Serial.println("Conectado na Rede WiFi.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  clientMqtt.setServer(mqttServer, mqttPort);
  clientMqtt.setCallback(callback);

  /* Verifica se o cliente está conectado ao Broker */
  while (!clientMqtt.connected()) {     
      Serial.println("Conectando ao Broker MQTT...");
      
      String clientId = "ESP32Client_" + String(random(0xffff), HEX);
      Serial.println("clientId = " + clientId);
      
      /* Conecta o cliente ao Broker MQTT.
         Configurações de credenciais e Last Will podem ser configuradas aqui*/
      if (clientMqtt.connect(clientId.c_str(), mqttUser, mqttPassword )) {
        Serial.println("O cliente " + clientId + " foi conectado com sucesso");
      } else {
        // Estado do cliente MQTT. Quando a conexão falhar pode ser usado para obter informações sobre a falha
        int clientState = clientMqtt.state();
        
        Serial.print("Falha ao se conectar. ");
        Serial.println("Estado do cliente: " + (String) clientState);
        
        delay(2000);
      }
  }

  Serial.print("Tentando enviar a mensagem");
  
  clientMqtt.publish("1yG4Lcv0/esp32", "Hello from ESP32");
  clientMqtt.subscribe("1yG4Lcv0/led");

  // Define os pinos do sensor como entrada
  pinMode(pinoAnalogico, INPUT);
  pinMode(ledVermelho,OUTPUT);    
  pinMode(ledAmarelo,OUTPUT);
  pinMode(ledVerde,OUTPUT);
  pinMode(buzzer,OUTPUT);
  
  lcd.init();
  lcd.backlight();
}


void loop() {
  
  /* loop: função que deve ser chamada regularmente para permitir que o cliente
  processe as mensagens recebidas e mantenha sua conexão com o Broker.*/
  clientMqtt.loop();
  
  // Lê o pino analógico do sensor
  int leitura = analogRead(pinoAnalogico);
  int res = map(leitura, 0, 4095, 0, 100);
  int flag = true;

 


  
  // Apresenta a leitura analógica no monitor serial
  // Serial.print("leitura do sensor: ");
  // Serial.println(leitura);
 
  
 
  // Repete a leitura do sensor a cada 1 segundo

  // lcd.clear();
  // lcd.setCursor(0,0); // Coluna, Linha
  // lcd.print("Leitura:");
  // lcd.print(leitura);

  // lcd.setCursor(0,3);
  // lcd.print("Tempo decorrido:");
  // lcd.print(segundos++);
  // delay(100);

  // String payload = (String) leitura;

  soma = soma + leitura;
  qtde++;
  
  int tempoAtual = millis();
  if(tempoAtual - tempo > 1000) {
    int media = soma / qtde;
    String payload = (String) media;
    clientMqtt.publish("1yG4Lcv0/sensor", payload.c_str());
    tempo = tempoAtual;
    soma = 0;
    qtde = 0;

    lcd.clear();
    lcd.setCursor(0,0); // Coluna, Linha
    lcd.print("Leitura:");
    lcd.print(leitura);

    lcd.setCursor(0,3);
    lcd.print("Tempo decorrido:");
    lcd.print(segundos++);

    Serial.print("leitura do sensor: ");
    Serial.println(leitura);

  }

  // clientMqtt.publish("1yG4Lcv0/sensor", payload.c_str());

  delay(100);
}
