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
    if(valorRecebido == "estado_a"){
      digitalWrite(ledVermelho, HIGH);
    }
    if(valorRecebido == "0;0;0"){
      digitalWrite(ledAmarelo, HIGH);
    }
    if(valorRecebido == "1;1;1"){
      digitalWrite(ledVermelho, LOW);
    }
    if(valorRecebido == "2;2;2"){
      digitalWrite(ledAmarelo, LOW);
    }
    SerialBT.write(3);
  }
}