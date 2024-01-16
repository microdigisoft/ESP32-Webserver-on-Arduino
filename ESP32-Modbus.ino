#include <SoftwareSerial.h>
// RS485 setup with ESp32
#define RE 32  // Connect RE terminal with 32 of ESP
#define DE 33    // Connect DE terminal with 33 of ESP            
const byte ModReadBuffer[] = {0x01, 0x03, 0x00, 0x1e, 0x00, 0x01, 0xe4, 0x0c};
byte BufferValue[8];
SoftwareSerial mod(26, 27); // RX=26 , TX =27

void setup() {
  Serial.begin(115200);
  mod.begin(9600);// modbus configuration
  pinMode(RE, OUTPUT);
  pinMode(DE, OUTPUT);
}


void loop() {
  byte val1;
  while (mod.write(ModReadBuffer, sizeof(ModReadBuffer)) == 8) {
    val1 = ModbusData();
    delay(5000);
  }
}
byte ModbusData(){
  byte i;
  digitalWrite(DE,HIGH);
  digitalWrite(RE,HIGH);
  delay(10);
  if(mod.write(ModReadBuffer,sizeof(ModReadBuffer))==8){
    digitalWrite(DE,LOW);
    digitalWrite(RE,LOW);
    for(i=0;i<4;i++){
    //Serial.print(mod.read(),HEX);
    BufferValue[i] = mod.read();
    }
    if(BufferValue[1]!=255 && BufferValue[2]!=255 && BufferValue[2]!=255){
    Serial.println("Serial Data received On:"); 
    Serial.print("Modbus Buffer[1]="); 
    Serial.println(BufferValue[1],DEC);
    Serial.print("Modbus Buffer[2]="); 
    Serial.println(BufferValue[2],DEC);
    Serial.print("Modbus Buffer[3]="); 
    Serial.println(BufferValue[3],DEC);
     Serial.println("");}
   // }
  }
  return BufferValue[8];
}
 
