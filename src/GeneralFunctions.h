#include <Arduino.h>
#include <SparkFun_TB6612.h>
#include <QTRSensors.h>



//Ponte H
#define AIN1 21
#define BIN1 19
#define AIN2 22
#define BIN2 18
#define PWMA 23
#define PWMB 5
#define STBY 15

// Pino sensores laterais
#define SENSORFIM 13

// LED
#define LED_BUILTIN 2

// Motores
Motor motor2 = Motor(AIN1, AIN2, PWMA, 1, STBY);
Motor motor1 = Motor(BIN1, BIN2, PWMB, 1, STBY);

// Sensor de linha
QTRSensors qtr;
const uint8_t SensorCount = 2;
uint16_t sensorValues[SensorCount];

// Variaveis
bool noSensor = false;
int leiturasBorda = 0;
double tempoAntes;
double tempo;
double tempoParada;


void calibrar(){  
  qtr.setTypeAnalog();
  qtr.setSensorPins((const uint8_t[]){32, 33}, SensorCount);
  qtr.setTimeout(1000);

  delay(1000);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  // Calibração durará cerca de 10 segundos
  for (uint16_t i = 0; i < 200; i++)
  {
    qtr.calibrate();
  }
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
}

void setupSensoresLaterais(){
    pinMode(SENSORFIM, INPUT);
}

void verifica_chegada()//A gente já chegou? Função aplicada ao fim do laço loop que parará o seguidor caso encontre o indicativo de término de percurso
{
  int chegada = digitalRead(SENSORFIM);
  if(chegada == 1){
    if(noSensor == false){
      noSensor = true;
      leiturasBorda++;
    }
  }
  if(chegada == 0){
    if(noSensor == true){
      noSensor = false;
    }
  }

  tempoAntes = millis();
  
  if(leiturasBorda == 10){
    tempo = millis(); 
    leiturasBorda++;  
  }

  if(leiturasBorda >= 11 && (tempoAntes - tempo) >= 500)
  {
    motor1.drive(0);
    motor2.drive(0);
    delay(100000);
  }
}