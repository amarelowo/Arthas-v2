#include <Arduino.h>
#include <BluetoothSerial.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif


#define PINLED 2
#define AIN1 5
#define AIN2 18
#define PWMA 19
#define STBY 21

BluetoothSerial SerialBT;
String valorRecebido;


// int ledVermelho =  26;
// int ledAmarelo = 32;

void setup() {
  Serial.begin(9600);
  SerialBT.begin("esp32-teste");
  Serial.println("Pronto para pareamento!");

  pinMode(STBY, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PINLED, OUTPUT);

  digitalWrite(STBY, HIGH);
  // pinMode(ledVermelho, OUTPUT);
  // pinMode(ledAmarelo, OUTPUT);
}

void loop() {
  int i;

  if (SerialBT.available()){
    valorRecebido = SerialBT.readString();
    Serial.println(valorRecebido);
    // SerialBT.println(valorRecebido);

    //formato do dado recebido
    //     kP     ki      kD
    // 000.000;000.000;000.000
    if(valorRecebido == "estado_a" || valorRecebido == "Cliente Conectado!" || valorRecebido == "Cliente Desconectado!"){
      // Serial.println(valorRecebido);
      SerialBT.print(valorRecebido);
    }
    else if(valorRecebido == "ok"){
      SerialBT.print(valorRecebido);
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

      // prova real de que o numero está chegando inteiro

      Serial.print(kP, 6);
      Serial.print(" | ");
      Serial.print(kI, 6);
      Serial.print(" | ");
      Serial.println(kD, 6);
      SerialBT.print(kP);

    }
  }

  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);

  for(i=0;i<255;i++){
    analogWrite(PWMA, i);
    Serial.println(i);
    delay(10);
  }
  for(i=255;i>0;i--){
    analogWrite(PWMA, i);
    Serial.println(i);
    delay(10);
  }

  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  
  for(i=0;i<255;i++){
    analogWrite(PWMA, i);
    Serial.println(i);
    delay(10);
  }
  for(i=255;i>0;i--){
    analogWrite(PWMA, i);
    Serial.println(i);
    delay(10);
  }

}
