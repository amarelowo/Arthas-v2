#include <Arduino.h>
#include <BluetoothSerial.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;
String valorRecebido;


int ledVermelho =  26;
int ledAmarelo = 32;

void setup() {
  Serial.begin(9600);
  SerialBT.begin("esp32-teste");
  Serial.println("Pronto para pareamento!");


  pinMode(ledVermelho, OUTPUT);
  pinMode(ledAmarelo, OUTPUT);
}

void loop() {
  if (SerialBT.available()){
    valorRecebido = SerialBT.readString();
    Serial.println(valorRecebido);
    // SerialBT.println(valorRecebido);

    //formato do dado recebido
    //     kP     ki      kD
    // 000.000;000.000;000.000
    if(valorRecebido == "estado_a"){
      Serial.println(valorRecebido);
      SerialBT.println(valorRecebido);
    }
    else{
      String stgkP = valorRecebido.substring(0,7);
      double kP = stgkP.toDouble();
      String stgkI = valorRecebido.substring(8,15);
      double kI = stgkI.toDouble();
      String stgkD = valorRecebido.substring(16,23);
      double kD = stgkD.toDouble();
      if((kP + kI + kD) < 100){
        Serial.println("parece que deu certo essa bucetona aqui");
      }
      Serial.print(kP);
      Serial.print(" | ");
      Serial.print(kI);
      Serial.print(" | ");
      Serial.println(kD);
      SerialBT.println("PID recebido vlw men");

    }
  }
}