#include <LiquidCrystal.h>

#define trigPin1 9
#define trigPin2 7
#define echoPin1 10
#define echoPin2 6
#define buzzerPin1 8
#define buzzerPin2 13
#define ledPin1 A0
#define ledPin2 A1

const int colunasDisponiveis = 16;
const int limitedDistance = 15;
const int numReadings = 3;

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int blinkInterval = 1000;
int vaga1 = 0;
int vaga2 = 0;

void setup() {
  String linha1 = "Bem vindo ao";
  String linha2 = "nosso";
  String linha3 = "Estacionamento";
  String linha4 = "inteligente";
  
  lcd.begin(16, 2);
  Serial.begin(9600);
  
  pinMode(trigPin1, OUTPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(echoPin2, INPUT);
  pinMode(buzzerPin1, OUTPUT);
  pinMode(buzzerPin2, OUTPUT);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);

  lcd.setCursor(0, 0);
  CentralizaTexto(linha1);
  lcd.setCursor(0, 1);
  CentralizaTexto(linha2);
  delay(2000);
  lcd.clear();
  
  lcd.setCursor(0, 0);
  CentralizaTexto(linha3);
  lcd.setCursor(0, 1);
  CentralizaTexto(linha4);
  delay(2500);
  lcd.clear();
}

void loop() {
  float distance1, distance2, previousDistance1, previousDistance2;
  
  previousDistance1 = getFilteredDistance(trigPin1, echoPin1);
  delay(200);
  distance1 = getFilteredDistance(trigPin1, echoPin1);

  previousDistance2 = getFilteredDistance(trigPin2, echoPin2);
  delay(200);
  distance2 = getFilteredDistance(trigPin2, echoPin2);

  if (distance1 < limitedDistance)
  {
    if (!isDifference(distance1, previousDistance1) && distance1 < previousDistance1 + 0.5)
    {
      activateLED(ledPin1);
      if (distance1 < 15 && distance1 > 10)
        digitalWrite(buzzerPin1, 1000);
      else if (distance1 <= 10)
        digitalWrite(buzzerPin1, 5000);

        delay(200);
        noTone(buzzerPin1);
        deactivateLED(ledPin1);

    }

    else
    {
      deactivateLED(ledPin1);
      noTone(buzzerPin1);
    }
    vaga1 = 1;
  }
  
  else
  {
    deactivateLED(ledPin1);
    noTone(buzzerPin1);
    vaga1 = 0;
  }

  if (distance2 < limitedDistance)
  {
    if (!isDifference(distance2, previousDistance2) && distance2 < previousDistance2 + 0.5)
    {
      activateLED(ledPin2);
      if (distance2 < 15 && distance2 > 10)
        digitalWrite(buzzerPin2, 1000);
      else if (distance1 <= 10)
        digitalWrite(buzzerPin2, 5000);

        delay(200);
        noTone(buzzerPin2);
        deactivateLED(ledPin2);
    }

    else
    {
      deactivateLED(ledPin2);
      noTone(buzzerPin2);
    }
    vaga2 = 1;
  }
  
  else
  {
    deactivateLED(ledPin2);
    noTone(buzzerPin2);
    vaga2 = 0;
  }
          
  printaVagasLivres(vaga1, vaga2);
}

void playChangingBuzzer(int buzzerPin, float distance) 
{
  int baseFrequency = 500;
  int maxInterval = 2000;
  int interval = map(distance, 0, 30, 0, maxInterval);
  int frequency = baseFrequency + map(distance, 0, 30, 100, 0);
  
  tone(buzzerPin, frequency);
  delay(10);
  tone(buzzerPin, frequency);
  delay(100);
}

float getDistance(int trigPin, int echoPin) 
{
  
  long duration;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH);
  
  return (duration * 0.0343) / 2.0;
}

void activateLED(int pin) {
  digitalWrite(pin, HIGH);
}

void deactivateLED(int pin) {
  digitalWrite(pin, LOW);
}

void printaVagasLivres(int vaga1, int vaga2)
{
  lcd.clear();
  String vagaA = "Vaga A";
  String vagaB = "Vaga B";
  
  String semVagas1 = "No momento nao";
  String semVagas2 = "ha vagas livres";
  
  if (!vaga1 || !vaga2)
  {
    lcd.setCursor(0, 0);
  	lcd.print("Vagas Livres:");
    
    lcd.setCursor(0, 1);
    
    if(!vaga1 && !vaga2)
      CentralizaTexto("Vaga A | Vaga B");
    
    else if(!vaga1)
    	CentralizaTexto(vagaA);
      
    else if(!vaga2)
     	CentralizaTexto(vagaB);  	
  }
  
  else
  {
    lcd.setCursor(0, 0);
    CentralizaTexto(semVagas1);
    lcd.setCursor(0, 1);
    CentralizaTexto(semVagas2);
  }
}
  
void CentralizaTexto(String texto)
{
  int tamanhoDoTexto = texto.length();
  int espacosAntesDoTexto = (colunasDisponiveis - tamanhoDoTexto) / 2;
  
  for (int i = 0; i < espacosAntesDoTexto; i++)
  	lcd.print(" ");
  
  lcd.print(texto);
}

bool isDifference(float distance, float previousDistance) {
  float diff = abs(distance - previousDistance);
  
  return (diff <= 0.6);
}

float getFilteredDistance(int trigPin, int echoPin) {
  float total = 0;
  for (int i = 0; i < numReadings; i++) 
  {
    total += getDistance(trigPin, echoPin);
    delay(10);
  }

  return total / numReadings;
}