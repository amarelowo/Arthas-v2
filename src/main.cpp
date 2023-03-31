#include <Arduino.h>
#include <BluetoothSerial.h>
#include <SparkFun_TB6612.h>
#include <QTRSensors.h>
#include <GeneralFunctions.h>
#include <PID.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

int maxSpeed = 180;

void setup() {
  Serial.begin(9600);
  SerialBT.begin("ArthasBT");

  // seta os pinos dos sensores laterais AINDA ESTA SEM O DE CURVA
  setupSensoresLaterais();
}

void loop() {
  // Aguarda comandos do BT
  if (SerialBT.available()){
    String data = SerialBT.readString();
    Serial.println(data);
    
    if(data == "a"){
      SerialBT.print("Estado de calibração iniciando");
      calibrar();
    }
    else if(data == "b"){
      SerialBT.print("Iniciando percurso");
      // COMECA O PERCURSO COM A VELOCIDADE ESTABELECIDA NO APP OU COM A PADRAO QUE É 180
      runningTrack(maxSpeed);
    }
    else if(data == "c"){
      SerialBT.print("Modo Standby");
      motor1.drive(0);
      motor2.drive(0);
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

      // Atualiza os parametros de pid e a velocidade
      pid.atualizarPID(kP,kI,kD);
      maxSpeed = speed; 

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
}
