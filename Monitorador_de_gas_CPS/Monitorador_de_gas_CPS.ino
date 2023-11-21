#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

#define pinoAnalogico 13


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
  
  // Apresenta a leitura analógica no monitor serial
  Serial.print("leitura do sensor: ");
  Serial.println(leitura_analogica);
 
  // Verifica se existe gás tóxico presente no ambiente
  
 
// Repete a leitura do sensor a cada 1 segundo
delay(1000);
    
    lcd.setCursor(2,0); // Coluna, Linha
    lcd.print(leitura_analogica);
    
    lcd.setCursor(9,0);
    lcd.print(segundos++);
    delay(1000);
  }

  if(modo == 3) {
    lcd.setCursor(5, 0);
    lcd.print("Display");
    lcd.display();
    delay(1000);
    lcd.noDisplay();
    delay(1000);
  }

  if(modo == 4) {
    lcd.scrollDisplayLeft();
    //lcd.scrollDisplayRight();
    delay(400);
  }

  if(modo == 5) {
    String texto = "ABC";
    
    lcd.setCursor(15, 1);
    
    for(int i = 0; i < texto.length(); i++) {
      lcd.write(texto.charAt(i));
    }
  }
  
}