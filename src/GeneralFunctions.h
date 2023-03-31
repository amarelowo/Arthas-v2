#include <Arduino.h>
#include <SparkFun_TB6612.h>
#include <QTRSensors.h>
#include <PID.h>


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
const uint8_t SensorCount = 8;
uint16_t sensorValues[SensorCount];

// PID
PID pid(0.098,0.00,0.160, 3500, 1);


// Variaveis
bool noSensor = false;
int leiturasBorda = 0;
double tempoAntes;
double tempo;
double tempoParada;


void calibrar(){  
  qtr.setTypeAnalog();
  qtr.setSensorPins((const uint8_t[]){34,35,32, 33,25,26,27,14}, SensorCount);
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

bool verifica_chegada()//A gente já chegou? Função aplicada ao fim do laço loop que parará o seguidor caso encontre o indicativo de término de percurso
{
  bool verificador;

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
    verificador = true;
  }
  else{verificador = false;}


  return verificador;

}

void runningTrack(int velocidadeMaxima){
  while(!verifica_chegada()){

    // manda a leitura do QTR pro processamento do pid
    double correcao = pid.process(qtr.readLineWhite(sensorValues));

    // Printa os valores de P, I, D e a soma "PID" na serial
    Serial.println(pid.pidString());

    motor2.drive(constrain(velocidadeMaxima - correcao, 0, velocidadeMaxima));
    motor1.drive(constrain(velocidadeMaxima + correcao, 0, velocidadeMaxima));


  
  }
}