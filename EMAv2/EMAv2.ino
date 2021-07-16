#include <RH_ASK.h>
#ifdef RH_HAVE_HARDWARE_SPI
#include <SPI.h> // Not actually used but needed to compile
#endif

//Packet Variables
static byte SMP = 0;
static char SID[4];
static char RID[4];
static char ED[44];
static char HK[10];

//Checking Variables
static char RID_check[4];
static char HK_check[10];

//Conditional Variables
static boolean isReceivingUserContact = false;
static boolean hasUserContact = false;
static boolean isReceivingRHMessages = false;
static boolean hasSerialMessage = true;

byte Rx = 3, Tx = 4;
//RH_ASK driver
RH_ASK driver(2000, 5, 6); 

void setup(){  
    pinMode(Tx, OUTPUT);
    pinMode(Rx, OUTPUT);
#ifdef RH_HAVE_SERIAL
    Serial.begin(9600);   // Debugging only
#endif
    if (!driver.init())
#ifdef RH_HAVE_SERIAL
         Serial.println("init failed");
#else
  ;
#endif
}  
 
void loop(){

  while(!hasUserContact){
    digitalWrite(Rx, LOW);
    digitalWrite(Tx, LOW);
    char serialData;
    byte SIDCounter = 0;
    
    if(Serial.availableForWrite() >= 63){
      Serial.write(0x01);
      isReceivingUserContact = true;
    }
    while(isReceivingUserContact){
      while(SIDCounter <= 3){
        if(Serial.available()){
          serialData = Serial.read();
          SID[SIDCounter] = serialData;
          SIDCounter++;
        }
        if(SIDCounter >= 3){
          hasUserContact = true;
          isReceivingUserContact = false;
          isReceivingRHMessages = true;
          hasSerialMessage = false;
        }  
      }
    }
  }

  Serial.print("SID : ");
  Serial.print(SID);

  while(isReceivingRHMessages){
    digitalWrite(Rx, HIGH);
    
    uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
    uint8_t buflen = sizeof(buf);

    if (driver.recv(buf, &buflen)){ // Non-blocking
      int i;
      // Message with a good checksum received, dump it.
      for(i = 0; i<buflen; i++){
        Serial.print((char)buf[i]);
      }
    }
  }
   
  delay(500);  
}  

void hashKeyGenerator(){
  //insert hash key algorithm here    
}
