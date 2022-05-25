int parola_corecta[4] = {1, 2, 3, 4};
#include <IRremote.h>
#include <LiquidCrystal.h>

const int echoPin = 2; //pin senzor ultrasonic
const int trigPin = 3; //pin senzor ultrasonic
const int RECV_PIN = 4; //pin reciever IR
const int rs = 7, en = 8, d4 = 9, d5 = 10, d6 = 11, d7 = 12; //pini lcd
const int buzzer = 5; //pin buzzer

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
IRrecv irrecv(RECV_PIN);
decode_results results;
unsigned long key_value = 0;
long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement

void setup() {
  Serial.begin(9600);
  //led RGB
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  //reciever IR
  irrecv.enableIRIn();
  irrecv.blink13(true);
  //senzor ultrasonic
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  //LCD
  lcd.begin(16, 2);
}
bool deschis = false, corect = false, blocat = false;
int parola[4];
int i = 0;


void loop() {
  delay(500);
  int input = 0;
  //masoara distanta de la capac
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  //daca distanta e mai mare de 10 cm inseamna ca capacul a fost deschis
  if(distance > 10){
     deschis = true;
  }
  if(distance < 10){
    if(corect == true){
      i=0;
      parola[0]=0;
      parola[1]=0;
      parola[2]=0;
      parola[3]=0;
    }
    if(deschis == true){
      lcd.clear();
      i=0;
    }
    deschis = false;
    corect = false;
  }
  if(deschis == false && blocat == false){
    lcd.setCursor(5, 0);
    lcd.print("Parola:   ");
    //aprinde led rosu
    digitalWrite(A0, HIGH);
    digitalWrite(A1, LOW);
    //cauta input de la telecomanda
    if (irrecv.decode(&results)){
 
        if (results.value == 0XFFFFFFFF)
          results.value = key_value;

        switch(results.value){
          case 0xFF6897:
          input = 0;
          break ;  
          case 0xFF30CF:
          input = 1;
          break ;
          case 0xFF18E7:
          input = 2;
          break ;
          case 0xFF7A85:
          input = 3;
          break ;
          case 0xFF10EF:
          input = 4;
          break ;
          case 0xFF38C7:
          input = 5;
          break ;
          case 0xFF5AA5:
          input = 6;
          break ;
          case 0xFF42BD:
          input = 7;
          break ;
          case 0xFF4AB5:
          input = 8;
          break ;
          case 0xFF52AD:
          input = 9;
          break ;      
        }
        key_value = results.value;
        irrecv.resume(); 
  }
  Serial.print(input);
    //daca exista input il adauga la parola
    if(input && i < 5){
      parola[i] = input;
      lcd.setCursor(6+i, 1);
      lcd.print(input);
      i++;
    }
    //daca sunt destule cifre pentru a forma o parola verifica daca e corect
    if(i == 4){
      Serial.print(parola[0]);
      Serial.print(parola[1]);
      Serial.print(parola[2]);
      Serial.print(parola[3]);
      if(parola_corecta[0] == parola[0] && parola_corecta[1] == parola[1] && parola_corecta[2] == parola[2] && parola_corecta[3] == parola[3]){
        corect = true;
        lcd.clear();
        lcd.setCursor(5,0);
        lcd.print("corect!");
        digitalWrite(A0, LOW);
        digitalWrite(A1, HIGH);
        delay(5000);
      }
      else{
        i = 0;
        lcd.clear();
        lcd.setCursor(4,0);
        lcd.print("incorect");
        delay(1000);
        lcd.clear();
      }
    }
  }
  if(deschis == false && blocat == true){
    lcd.setCursor(0, 0);
    lcd.print("     BLOCAT!");
  }
  if(deschis == true && corect == false){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("  Deschis fara");
      lcd.setCursor(0,1);
      lcd.print(" parola corecta!");
      blocat = true;
      tone(buzzer, 1000);
      digitalWrite(A0, LOW);
      digitalWrite(A1, LOW);
      delay(500);
      digitalWrite(A0, HIGH);
      digitalWrite(A1, LOW);
      noTone(buzzer);
    }
}
