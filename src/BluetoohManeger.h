#include <BluetoothSerial.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

const char *pin = "8266";
String data;

void setupBT(){
    SerialBT.begin("Arthas_vai_comer_seu_bumbum");
    SerialBT.setPin(pin);
    
}

void receiveBTData(){
    
}