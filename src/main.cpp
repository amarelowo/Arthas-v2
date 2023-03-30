#include <Arduino.h>
#include <BluetoothSerial.h>
#include <SparkFun_TB6612.h>
#include <QTRSensors.h>
#include <BluetoohManeger.h>
#include <GeneralFunctions.h>

void setup() {
  Serial.begin(9600);

  setupBT();
  setupSensoresLaterais();


}

void loop() {
  // Aguarda comandos do BT
  if (SerialBT.available()){
    data = SerialBT.readString();
    Serial.println(data);
    
    if(data == "a"){
      SerialBT.print("Estado de calibração iniciando");
      calibrar();
    }
    else if(data == "b"){
      SerialBT.print("Iniciando percurso");
      motor1.drive(100);
      motor2.drive(255);
      // runningTrack();
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
}
