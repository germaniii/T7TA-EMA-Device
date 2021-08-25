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
static boolean isWaiting = false;
static boolean isSendingRHMessage = false;

static byte msg_len;

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
    
    if(Serial.availableForWrite() >= 60){
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
          hasSerialMessage = false;
          isWaiting = true;
        }  
      }
    }
  }

  Serial.print("SID : ");
  Serial.print(SID);

  while(isWaiting){   // Receiver Mode
    digitalWrite(Rx, HIGH); // Turn on Receiver Module
    uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
    uint8_t buflen = sizeof(buf);

    //if Message from Outside
    if(driver.available()){
      isReceivingRHMessages = true;
      boolean isForMe = false;
      while(isReceivingRHMessages){
          int i;
        if (driver.recv(buf, &buflen)){ // Non-blocking
          for(i = 1; i <= 4; i++){
            if(buf[i] == '0'){
              isForMe = true; // Checks if incoming message is for you, else throw
              continue;
            }
            else {
              isForMe = false;
              break;
            }
          }

          if(isForMe == false){
            for(i = 1; i <= 4; i++){
              if(buf[i] == SID[i-1]){
                isForMe = true; // Checks if incoming message is for you, else throw
                continue;
              }
              else {
                isForMe = false;
                break;
              }
            }
          }

          if(isForMe){
            //...
            //Code to verify received message with HK
            //...
            // Message with a good checksum received, dump it.
            for(i = 0; i<buflen; i++){
              Serial.print((char)buf[i]);
            }
          }else 
            isReceivingRHMessages = false;
        }
      }
      isReceivingRHMessages = false;
    }

    // if Message from user Phone serial
    if(Serial.available() >= 60){
      int i;    // Read all from the buffer
      for(i = 0; i < RH_ASK_MAX_MESSAGE_LEN; i++){
        buf[i] = Serial.read();
      }
      
      isSendingRHMessage = true;
      digitalWrite(Tx, HIGH); // Turn on Transmitter Module
      digitalWrite(Rx, LOW); // Turn off Receiver Module
      
      while(isSendingRHMessage){
        driver.send((uint8_t *)buf, buflen);
        driver.waitPacketSent();
        isSendingRHMessage = false;
        delay(200);
      }
      
      delay(100); 
      digitalWrite(Tx, LOW); // Turn off Transceiver Module
      digitalWrite(Rx, HIGH); // Turn on Receiver Module
        
    }
  }

}  

void hashKeyGenerator(){
  //insert hash key algorithm here    
}
