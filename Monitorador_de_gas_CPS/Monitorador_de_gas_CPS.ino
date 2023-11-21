#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

#define pinoAnalogico 13
#define ledVermelho 12
#define ledAmarelo 14
#define ledVerde 27


int modo = 1;

int segundos = 0;

byte smile[8] = {
  B00000,
  B10001,
  B00000,
  B00000,
  B10001,
  B01110,
  B00000,
  B00000
};

void setup() {
  Serial.begin(9600);

  // Define os pinos do sensor como entrada
  pinMode(pinoAnalogico, INPUT);
  pinMode(ledVermelho,OUTPUT);    
  pinMode(ledAmarelo,OUTPUT);
  pinMode(ledVerde,OUTPUT);

  lcd.init();
  lcd.backlight();

  lcd.createChar(0, smile);

  if(modo == 0) {
    lcd.setCursor(5, 0);
    lcd.print("C");
    //lcd.cursor();
    //lcd.blink();
  }

  if(modo == 2){
    lcd.home();
    lcd.write(224);
    lcd.write(B11111100);
    lcd.write(126);
    lcd.write(B00100110);
  }

  if(modo == 4) {
    lcd.home();
    lcd.print("Ciber Fisico");
  }

  if(modo == 5) {
    lcd.rightToLeft();
  }

  if(modo == 6) {
    lcd.home();
    lcd.write(0);
  }
}

void loop() {
  
  if(modo == 1) {

    // Lê o pino analógico do sensor
  int leitura_analogica = analogRead(pinoAnalogico);
  int flag = true;

 
  
  
  // Apresenta a leitura analógica no monitor serial
  Serial.print("leitura do sensor: ");
  Serial.println(leitura_analogica);
 
  // Verifica se existe gás tóxico presente no ambiente
  
 
// Repete a leitura do sensor a cada 1 segundo
delay(1000);
    
    lcd.setCursor(1,0); // Coluna, Linha
    lcd.print("Leitura:");
    lcd.print(leitura_analogica);
    
    lcd.setCursor(2,3);
    lcd.print("Tempo decorrido:");
    lcd.print(segundos++);
    delay(1000);

 for (int i = -1;i<segundos;i++){
    if(leitura_analogica<100){
      digitalWrite(ledVerde,HIGH);
      digitalWrite(ledAmarelo,LOW);
      digitalWrite(ledVermelho,LOW);
    }else if (leitura_analogica>100 && leitura_analogica<300){
     digitalWrite(ledVerde,LOW);
     digitalWrite(ledAmarelo,HIGH);
     digitalWrite(ledVermelho,LOW);
    }else{
      digitalWrite(ledVerde,LOW);
      digitalWrite(ledAmarelo,LOW);
      digitalWrite(ledVermelho,HIGH);
    }
  }

  }
}