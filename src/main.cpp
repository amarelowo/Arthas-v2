#include <Arduino.h>
#include <BluetoothSerial.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

// Pin
#define PINLED 2

// Pino motor 1
#define AIN1 5
#define AIN2 18
#define PWMA 19

// Pino stby ponte h
#define STBY 21

// Pino Encoder
#define C1 26
#define C2 27
byte Encoder_C1Last;
int duracao;
boolean Direcao;






BluetoothSerial SerialBT;
String data;


// int ledVermelho =  26;
// int ledAmarelo = 32;


void updateEncoder(){
  int Lstate = digitalRead(C1);
  if ((Encoder_C1Last == LOW) && Lstate == HIGH){
    int val = digitalRead(C2);
    if (val == LOW && Direcao){
      Direcao = false; //Reverse
    }
    else if (val == HIGH && !Direcao){
      Direcao = true;  //Forward
    }
  }
  Encoder_C1Last = Lstate;
  if (!Direcao)  duracao++;
  else  duracao--;
}

void setup() {
  Serial.begin(9600);
  SerialBT.begin("esp32-teste");
  Serial.println("Pronto para pareamento!");

  pinMode(STBY, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PINLED, OUTPUT);


  pinMode(C1, INPUT);
  pinMode(C2, INPUT_PULLUP);

  // digitalWrite(C1, HIGH);
  // digitalWrite(C2, HIGH);

  attachInterrupt(C1,updateEncoder, CHANGE);
  // attachInterrupt(digitalPinToInterrupt(C2),updateEncoder, CHANGE);

  digitalWrite(STBY, HIGH);
  // pinMode(ledVermelho, OUTPUT);
  // pinMode(ledAmarelo, OUTPUT);
}

void loop() {
  int i;

  if (SerialBT.available()){
    data = SerialBT.readString();
    Serial.println(data);
    
    if(data == "a"){
      for(i=0;i<5;i++){
      SerialBT.print("Estado de calibração iniciando");
      }
    }
    else if(data == "b"){
      SerialBT.print("Iniciando percurso");
    }
    else if(data == "c"){
      SerialBT.print("Modo Standby");
    }
    else if(data == "Cliente Conectado!" || data ==  "Cliente Desconectado!"){
      SerialBT.print(data);
    }
    else{
      //formato do dado recebido
      //           kP     ki      kD     Vel
      //        d;000.000;000.000;000.000;000
      //Carac.     2-9     10-17   18-25   26-29

      String rawKp = data.substring(2,9);
      double kP = rawKp.toDouble();

      String rawKi = data.substring(10,17);
      double kI = rawKi.toDouble();

      String rawKd = data.substring(18,25);
      double kD = rawKd.toDouble();

      String rawSpeed = data.substring(26,29);
      int speed = rawSpeed.toInt();

      // Prova pra ver se o numero ta virando numero de fato
      Serial.print("Soma: ");
      Serial.println(kP+kI+kD+speed);

      // Printa na serial do bt
      SerialBT.print("Kp: ");
      SerialBT.print(kP);
      SerialBT.print(" | Ki: ");
      SerialBT.print(kI);
      SerialBT.print(" | Kd: ");
      SerialBT.print(kD);
      SerialBT.print(" | Vel: ");
      SerialBT.print(speed);
      
    }
  }

  // digitalWrite(AIN1, HIGH);
  // digitalWrite(AIN2, LOW);

  // for(i=0;i<255;i++){
  //   analogWrite(PWMA, i);
  //   Serial.println(duracao);
  //   delay(30);
  // }
  // for(i=255;i>0;i--){
  //   analogWrite(PWMA, i);
  //   Serial.println(duracao);
  //   delay(30);
  // }
  // digitalWrite(AIN1, LOW);
  // digitalWrite(AIN2, HIGH);

  // for(i=0;i<255;i++){
  //   analogWrite(PWMA, i);
  //   Serial.println(duracao);
  //   delay(30);
  // }
  // for(i=255;i>0;i--){
  //   analogWrite(PWMA, i);
  //   Serial.println(duracao);
  //   delay(30);
  // }
  
  // delay(100);



}
