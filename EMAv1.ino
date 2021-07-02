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
static boolean isReceivingSerialMessages = false;
static boolean hasSerialMessage = true;

//Counter Variables
static short SIDCounter = 0;

void setup(){  
  Serial.begin(9600);  
}  
 
void loop(){
  char serialData;
  
  if(Serial.available()){
    serialData = Serial.read();

    /* 
     *  RECEIVE and process Serial Data from User Android Phone on this part 
     */
     
    if(isReceivingUserContact)
      assignSID(serialData);

    if(isReceivingSerialMessages);  // insert code to pass to SoftwareSerial here
      
  }else{
    
    /* 
     *  SEND Control Codes on this part of the code
     *  '1 - 8' can be used as a control operator
     */
     
    if(!hasUserContact){                            // Control Code 1, This will ask for the User SID
      if(Serial.availableForWrite() >= 63){
        Serial.write(0x01);
        isReceivingUserContact = true;
      }
      
    }else if(!hasSerialMessage){                    // Control Code 2, Messages from MessageOut_Table will be sent here
      if(Serial.availableForWrite() >= 63){
        Serial.write(0x02)
        hasSerialMessage = true;
      }
                                                    // Control Code 3, This will ask the Android Phone to ready the MessagesIn_Table
    }else{ 
      Serial.print("SID : ");
      Serial.print(SID);
    }
    delay(500);  
  }
}  

/*------------------------
 *FUNCTIONS FOR MODULARITY
  ------------------------*/
 
void assignSID(char serialData){
  while(SIDCounter <= 3){
    SID[SIDCounter] = serialData;
    SIDCounter++;
    if(Serial.available())
      serialData = Serial.read();
  }
  if(SIDCounter >= 3){
    hasUserContact = true;
    isReceivingUserContact = false;
    isReceivingSerialMessages = true;
    hasSerialMessaage = false;
  }
}

void hashKeyGenerator(){
  //insert hash key algorithm here    
}
