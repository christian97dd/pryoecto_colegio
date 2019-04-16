/*

#####################################################################################

 ******Pines para el Arduino*********
 *** Reset = Pin 5 **
 *** SDA = Pin 10 ***
 *** MOSI = Pin 11 ***
 *** MISO = Pin 12 ***
 *** SCK = Pin 13 ***
 *** GND = Ground ***
 *** 3.3 = 3.3v ***

 ******************************************

*/

 

 

// Librerías

#include <SPI.h>
#include <RFID.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

// Definición del RFID

RFID rfid(10,5);

byte tarjeta[5] = {0xC5,0x5A,0x91,0x28,0x26};

// Coloca aquí las tarjetas permitidas

// Declaración del LCD y del tipo

LiquidCrystal_I2C lcd(0x27,20,4);

byte serNum[5];
byte data[5];
// Definición de los pines - LED, Buzzer y Servomotor
int LED_access = 2;
int LED_intruder = 3;
int speaker_pin = 8;
int servoPin = 9;
// Definición del servomotor
Servo doorLock;

void setup(){
  doorLock.attach(servoPin);
  Serial.begin(9600); // Inicialización de comunicación serial
  lcd.init(); // Initialización del LCD
  lcd.backlight();
  lcd.clear();// Limpia el display LCD
  SPI.begin(); // inicialización de la comunicación SPI
  rfid.init(); // inicialización del módulo RFID
  delay(1000);
  pinMode(LED_access,OUTPUT);
  pinMode(LED_intruder,OUTPUT);
  pinMode(speaker_pin,OUTPUT);
  pinMode(servoPin,OUTPUT);
}

void loop(){
  lcd.clear();
  lcd.noBacklight();

  // Aquí puedes crear una variable para cada usuario
  // NAME_card o KEY_card
  boolean tarjeta_card = true; // Esta es la tarjeta

  if (rfid.isCard()){ // tarjeta válida encontrada
    if (rfid.readCardSerial()){ // lee la tarjeta
      delay(1000);
      data[0] = rfid.serNum[0]; // almacena el número de serie
      data[1] = rfid.serNum[1];
      data[2] = rfid.serNum[2];
      data[3] = rfid.serNum[3];
      data[4] = rfid.serNum[4];
    }
  //rfid.halt(); // Módulo RFID puesto en espera

  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("   ID Encontrada:");
  //Serial.print("Tarjeta Encontrada - Código:");

  lcd.setCursor(0,1);
  lcd.print("     ");
  if(rfid.serNum[0] < 16){
    lcd.print("0");
  }

  lcd.print(rfid.serNum[0],HEX);
  if(rfid.serNum[1] < 16){
    lcd.print("0");
  }

  lcd.print(rfid.serNum[1],HEX);
  if(rfid.serNum[2] < 16){
    lcd.print("0");
  }

  lcd.print(rfid.serNum[2],HEX);
  if(rfid.serNum[3] < 16){
    lcd.print("0");
  }

  lcd.print(rfid.serNum[3],HEX);
  if(rfid.serNum[4] < 16){
    lcd.print("0");
  }

  lcd.print(rfid.serNum[4],HEX);
  for(int i=0; i<5; i++){
    if(data[i] != tarjeta[i]) tarjeta_card = false; // Si no es una tarjeta válida, coloca falso a este usuario
    // Aquí puedes comprobar los otros usuarios permitidos, solo coloca líneas como las de arriba con el nombre del usuario
  }

  Serial.println();
  if (tarjeta_card){ // Si una tarjeta válida fue encontrada
    lcd.setCursor(0,2);
    lcd.print("  Acceso Permitido!");
    lcd.setCursor(0,3);
    lcd.print("     Bienvenido!");
    digitalWrite(speaker_pin,HIGH);
    delay(500);
    digitalWrite(speaker_pin,LOW);
    delay(500);
  }
  else{ // si la tarjeta no es reconocida
    lcd.setCursor(0,2);
    lcd.print(" ID no reconocida!");
    lcd.setCursor(0,3);
    lcd.print("   Acceso denegado!");
    //Serial.println("Tarjeta no reconocida!  contacte con la administración!"); // imprime un mensaje

    digitalWrite(LED_intruder, HIGH); // enciende el LED rojo
    digitalWrite(speaker_pin,HIGH);
    delay(1500);
    digitalWrite(speaker_pin,LOW);
    digitalWrite(LED_intruder, LOW); // apaga el LED rojo
  }

  if (tarjeta_card){// Añade otro usuario usando una logica o condición
    // Mensaje de bienvenida y permiso de acceso
    //Serial.println("Acceso Permitido!    Bienvenido!"); // Imprime un mensaje
    digitalWrite(LED_access,HIGH); // enciende el LED verde
    doorLock.write(90);
    delay(10000); // espera
    doorLock.write(0);
    digitalWrite(LED_access,LOW); // apaga el LED verde
  }

  Serial.println();
  delay(500);
  rfid.halt();
  lcd.noBacklight();
  }

}