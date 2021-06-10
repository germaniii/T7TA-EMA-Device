//Packet Variables
static byte SMP;
static char SID[4];
static char RID[4];
static char ED[44];
static char HK[10];

//Conditional Variables
static boolean isReceivingUserContact = false;
static boolean hasUserContact = false;

//Counter Variables
static short SIDCounter = 0;

void setup(){  
  Serial.begin(9600);  
}  
 
void loop(){
  char c;
  
  if(Serial.available()){
    c = Serial.read();
    
    if(isReceivingUserContact)
      assignSID(c);
    
  }else{
    if(!hasUserContact){
      // '~' will be used as a control operator
      Serial.print('~');
      isReceivingUserContact = true;
    }else{
      // for debugging purposes
      Serial.print("SID ");
      Serial.print(SID);
    }
    delay(500);  
  }
}  


void assignSID(char c){
  while(SIDCounter <= 3){
    SID[SIDCounter] = c;
    SIDCounter++;
    if(Serial.available())
      c = Serial.read();
  }
  if(SIDCounter >= 3){
    hasUserContact = true;
    isReceivingUserContact = false;
  }
}
