/*
  Todd Rogers, Computer Engineering Senior Project Fall 20- University of Utah
 This is the code which runs the MCU on the powerstrip. 
 
 This example code is in the public domain.
 */
 #include <SoftwareSerial.h>
 
  SoftwareSerial mySerial(2, 3);
  String str = "";
  boolean stringComplete;
  int currentCount;
  int numActiveSocket = 0;
  
  //Sockets 0-3
  int socketD0Pin = 9;
  int socketD1Pin = 10;
  int socketD2Pin = 11;
  int socketD3Pin = 12;
  
  //Are sockets 1-4 on or off?
  boolean socketD0OnOff;
  boolean socketD1OnOff;
  boolean socketD2OnOff;
  boolean socketD3OnOff;
  
  //current stats of each socket
  int socket0Current;
  int socket0CurrentHigh = 0;
  int socket0CurrentLow = 10000;
  int socket0Power;
  
  int socket1Current;
  int socket1CurrentHigh = 0;
  int socket1CurrentLow = 10000;
  int socket1Power;
  
  int socket2Current;
  int socket2CurrentHigh = 0;
  int socket2CurrentLow = 10000;
  int socket2Power;
  
  int socket3Current;
  int socket3CurrentHigh = 0;
  int socket3CurrentLow = 10000;
  int socket3Power;
  
  //current sensor pins
  int socketA0Pin = A0;
  int socketA1Pin = A1;
  int socketA2Pin = A2;
  int socketA3Pin = A3; //something wrong with current 4 reading
  
void setup() {

  Serial.begin(9600);
  mySerial.begin(9600);
  
  pinMode(socketA0Pin, INPUT);
  pinMode(socketA1Pin, INPUT);
  pinMode(socketA2Pin, INPUT);
  pinMode(socketA3Pin, INPUT);
  
  pinMode(socketD0Pin, OUTPUT);
  pinMode(socketD1Pin, OUTPUT);
  pinMode(socketD2Pin, OUTPUT);
  pinMode(socketD3Pin, OUTPUT);
  str.reserve(200);
  
  
}

void loop() {
 
  //Change to this if you want a serial connection only from the usb interface
      while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    str += (String)inChar;
    //mySerial.print(inChar);
    // add it to the inputString:
        if (inChar == '\n') {
        
      stringComplete = true;
      break;
    }
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it
  }
  
  /*
    while (mySerial.available()) {
    // get the new byte:
    char inChar = (char)mySerial.read();
    str += (String)inChar;
    //mySerial.print(inChar);
    // add it to the inputString:
        if (inChar == '\n') {
        
      stringComplete = true;
      break;
    }
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it
  } */
  
  //testing/debugging
if(stringComplete)
Serial.print(str);

//Process the mySerial command
if(stringComplete)
mySerialCommand(str);
  
//Get the number of active sockets
numActiveSocket=0;
if(socketD0OnOff)
numActiveSocket++;
if(socketD1OnOff)
numActiveSocket++;
if(socketD2OnOff)
numActiveSocket++;
if(socketD3OnOff)
numActiveSocket++;

  
  //find current 0
  socket0Current = analogRead(socketA0Pin);
  
  if(socket0Current > socket0CurrentHigh)
  socket0CurrentHigh = socket0Current;
  if(socket0Current < socket0CurrentLow)
  socket0CurrentLow = socket0Current;
  
    //find current 1
  socket1Current = analogRead(socketA1Pin);
  
  if(socket1Current > socket1CurrentHigh)
  socket1CurrentHigh = socket1Current;
  if(socket1Current < socket1CurrentLow)
  socket1CurrentLow = socket1Current;
  
    //find current 2
  socket2Current = analogRead(socketA2Pin);
  
  if(socket2Current > socket2CurrentHigh)
  socket2CurrentHigh = socket2Current;
  if(socket2Current < socket2CurrentLow)
  socket2CurrentLow = socket2Current;
  
    //find current 3
  socket3Current = analogRead(socketA3Pin);
  
  if(socket3Current > socket3CurrentHigh)
  socket3CurrentHigh = socket3Current;
  if(socket3Current < socket3CurrentLow)
  socket3CurrentLow = socket3Current;
  
  //reset current high/low and find real socket current
  //35 = 23W
  if(currentCount > 500) //adjust this number to adjust the granularity of the current calculation
  {
  currentCount = 0;
  
  socket0Power = socket0CurrentHigh - socket0CurrentLow - 11; // -11 because of natural offset
  socket1Power = socket1CurrentHigh - socket1CurrentLow - 11;
  socket2Power = socket2CurrentHigh - socket2CurrentLow - 11;
  socket3Power = socket3CurrentHigh - socket3CurrentLow - 11;
  
  //reset current ranges
  socket0CurrentHigh = 0;
  socket0CurrentLow = 10000;
  socket1CurrentHigh = 0;
  socket1CurrentLow = 10000;
  socket2CurrentHigh = 0;
  socket2CurrentLow = 10000;
  socket3CurrentHigh = 0;
  socket3CurrentLow = 10000;
  
  }
  
  currentCount++;
  
 
     // print the string when a newline arrives:
  if (stringComplete) {
    stringComplete = false;
    mySerial.print(str);
    str = "";
  }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
 /* List of Commands you can enter:
 turn on <socketnum>        //turn on socket
 turn off <socketnum>       //turn off socket
 power <socketnum>          //returns power of socket
 current <socketnum> high   //returns highpoint of current waveform
 current <socketnum> low    //returns lowpoint of current waveform
 socket <socketnum>         //returns on/off
 allpower                   //returns all powers on a new line each
 active sockets             //returns the number of powered sockets
 */ 
//Process mySerial command string
void mySerialCommand(String str)
{
 if(str.equals("turn on 0\n"))
  {
        mySerial.println("Turning on Relay 0\n");
        Serial.println("Turning on Relay 0\n");
        digitalWrite(socketD0Pin, 1);
        socketD0OnOff = true;
  }
 
 if(str.equals("turn off 0\n"))
 {
       mySerial.println("Turning off Relay 0\n");
       Serial.println("Turning off Relay 0\n");
       digitalWrite(socketD0Pin, 0);
       socketD0OnOff = false;
 }
 
   if(str.equals("turn on 1\n"))
  {
        mySerial.println("Turning on Relay 1\n");
        Serial.println("Turning on Relay 1\n");
        digitalWrite(socketD1Pin, 1);
        socketD1OnOff = true;
  }
 
 if(str.equals("turn off 1\n"))
 {
       mySerial.println("Turning off Relay 1\n");
       Serial.println("Turning on Relay 1\n");
       digitalWrite(socketD1Pin, 0);
       socketD1OnOff = false;
 }
 
   if(str.equals("turn on 2\n"))
  {
        mySerial.println("Turning on Relay 2\n");
        Serial.println("Turning on Relay 2\n");
        digitalWrite(socketD2Pin, 1);
        socketD2OnOff = true;
  }
 
 if(str.equals("turn off 2\n"))
 {
       mySerial.println("Turning off Relay 2\n");
       Serial.println("Turning on Relay 2\n");
       digitalWrite(socketD2Pin, 0);
       socketD2OnOff = false;
 }
 
   if(str.equals("turn on 3\n"))
  {
        mySerial.println("Turning on Relay 3\n");
        Serial.println("Turning on Relay 3\n");
        digitalWrite(socketD3Pin, 1);
        socketD3OnOff = true;
  }
 
 if(str.equals("turn off 3\n"))
 {
       mySerial.println("Turning off Relay 3\n");
       Serial.println("Turning on Relay 3\n");
       digitalWrite(socketD3Pin, 0);
       socketD3OnOff = false;
 }
 //////////////////////////////////////////////////////////////////////////////////////////////////////////////
 //If they requested current information... socket 0
  if(str.equals("power 0\n"))
 {
   mySerial.print("\ncurrent 0: ");
   mySerial.print(socket0Power);
   mySerial.println();
   Serial.print("\ncurrent 0: ");
   Serial.print(socket0Power);
   Serial.println();
 }
 if(str.equals("current 0 high\n"))
 {
   mySerial.print("\nThe High Current for Socket 0 is: ");
   mySerial.print(socket0CurrentHigh);
   mySerial.println();
   Serial.print("\nThe High Current for Socket 0 is: ");
   Serial.print(socket0CurrentHigh);
   Serial.println();
 }
  if(str.equals("current 0 low\n"))
 {
   mySerial.print("\nThe Low Current for Socket 0 is: ");
   mySerial.print(socket0CurrentLow);
   mySerial.println();
   Serial.print("\nThe Low Current for Socket 0 is: ");
   Serial.print(socket0CurrentLow);
   Serial.println();
 }
 //socket1
   if(str.equals("power 1\n"))
 {
   mySerial.print("\ncurrent 1: ");
   mySerial.print(socket1Power);
   mySerial.println();
   Serial.print("\ncurrent 1: ");
   Serial.print(socket1Power);
   Serial.println();
 }
 if(str.equals("current 1 high\n"))
 {
   mySerial.print("\nThe High Current for Socket 1 is: ");
   mySerial.print(socket1CurrentHigh);
   mySerial.println();
   Serial.print("\nThe High Current for Socket 1 is: ");
   Serial.print(socket1CurrentHigh);
   Serial.println();
 }
  if(str.equals("current 1 low\n"))
 {
   mySerial.print("\nThe Low Current for Socket 1 is: ");
   mySerial.print(socket1CurrentLow);
   mySerial.println();
   Serial.print("\nThe Low Current for Socket 1 is: ");
   Serial.print(socket1CurrentLow);
   Serial.println();
 }
 //socket2
   if(str.equals("power 2\n"))
 {
   mySerial.print("\ncurrent 2: ");
   mySerial.print(socket2Power);
   mySerial.println();
   Serial.print("\ncurrent 2: ");
   Serial.print(socket2Power);
   Serial.println();
 }
 if(str.equals("current 2 high\n"))
 {
   mySerial.print("\nThe High Current for Socket 2 is: ");
   mySerial.print(socket2CurrentHigh);
   mySerial.println();
   Serial.print("\nThe High Current for Socket 2 is: ");
   Serial.print(socket2CurrentHigh);
   Serial.println();
 }
  if(str.equals("current 2 low\n"))
 {
   mySerial.print("\nThe Low Current for Socket 2 is: ");
   mySerial.print(socket2CurrentLow);
   mySerial.println();
   Serial.print("\nThe Low Current for Socket 2 is: ");
   Serial.print(socket2CurrentLow);
   Serial.println();
 }
 //socket3
   if(str.equals("power 3\n"))
 {
   mySerial.print("\ncurrent 3: ");
   mySerial.print(socket3Power);
   mySerial.println();
   Serial.print("\ncurrent 3: ");
   Serial.print(socket3Power);
   Serial.println();
 }
 if(str.equals("current 3 high\n"))
 {
   mySerial.print("\nThe High Current for Socket 3 is: ");
   mySerial.print(socket3CurrentHigh);
   mySerial.println();
   Serial.print("\nThe High Current for Socket 3 is: ");
   Serial.print(socket3CurrentHigh);
   Serial.println();
 }
  if(str.equals("current 3 low\n"))
 {
   mySerial.print("\nThe Low Current for Socket 3 is: ");
   mySerial.print(socket3CurrentLow);
   mySerial.println();
   Serial.print("\nThe Low Current for Socket 3 is: ");
   Serial.print(socket3CurrentLow);
   Serial.println();
 } 
   if(str.equals("allpower\n")) //prints all currents
 {
   mySerial.println(socket0Power);
   mySerial.println(socket1Power);
   mySerial.println(socket2Power);
   mySerial.println(socket3Power);
   Serial.println(socket0Power);
   Serial.println(socket1Power);
   Serial.println(socket2Power);
   Serial.println(socket3Power);
 } 
 
 //////////////////////////////////////////////////////////////////////////////////////////////////////////////
 //if they ask for the socket states(on/off) number of sockets on.. 
   if(str.equals("active sockets\n"))
 {
   mySerial.print("\nThe number of powered sockets is ");
   mySerial.print(numActiveSocket);
   mySerial.println();
   Serial.print("\nThe number of powered sockets is ");
   Serial.print(numActiveSocket);
   Serial.println();
 } 
 
    if(str.equals("socket 0\n"))
 {
   if(socketD0OnOff){
   mySerial.print("\nsocket 0 : on");
   mySerial.println();
   Serial.print("\nsocket 0 : on");
   Serial.println();
   }
   else
   {
   mySerial.print("\nsocket 0 : off");
   mySerial.println();
   Serial.print("\nsocket 0 : off");
   Serial.println();
   }
 } 
 
    if(str.equals("socket 1\n"))
 {
   if(socketD1OnOff){
   mySerial.print("\nsocket 1 : on");
   mySerial.println();
   Serial.print("\nsocket 1 : on");
   Serial.println();
   }
   else
   {
   mySerial.print("\nsocket 1 : off");
   mySerial.println();
   Serial.print("\nsocket 1 : off");
   Serial.println();
   }
 } 
 
     if(str.equals("socket 2\n"))
 {
   if(socketD2OnOff){
   mySerial.print("\nsocket 2 : on");
   mySerial.println();
   Serial.print("\nsocket 2 : on");
   Serial.println();
   }
   else
   {
   mySerial.print("\nsocket 2 : off");
   mySerial.println();
   Serial.print("\nsocket 2 : off");
   Serial.println();
   }
 }
 
      if(str.equals("socket 3\n"))
 {
   if(socketD3OnOff){
   mySerial.print("\nsocket 3 : on");
   mySerial.println();
   Serial.print("\nsocket 3 : on");
   Serial.println();
   }
   else
   {
   mySerial.print("\nsocket 3 : off");
   mySerial.println();
   Serial.print("\nsocket 3 : off");
   Serial.println();
   }
 }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Process Serial command string *old*
void SerialCommand(String str)
{
 if(str.equals("turn on 0\n"))
  {
        Serial.println("Turning on Relay 0\n");
        digitalWrite(socketD0Pin, 1);
        socketD0OnOff = true;
  }
 
 if(str.equals("turn off 0\n"))
 {
       Serial.println("Turning off Relay 0\n");
       digitalWrite(socketD0Pin, 0);
       socketD0OnOff = false;
 }
 
   if(str.equals("turn on 1\n"))
  {
        Serial.println("Turning on Relay 1\n");
        digitalWrite(socketD1Pin, 1);
        socketD1OnOff = true;
  }
 
 if(str.equals("turn off 1\n"))
 {
       Serial.println("Turning off Relay 1\n");
       digitalWrite(socketD1Pin, 0);
       socketD1OnOff = false;
 }
 
   if(str.equals("turn on 2\n"))
  {
        Serial.println("Turning on Relay 2\n");
        digitalWrite(socketD2Pin, 1);
        socketD2OnOff = true;
  }
 
 if(str.equals("turn off 2\n"))
 {
       Serial.println("Turning off Relay 2\n");
       digitalWrite(socketD2Pin, 0);
       socketD2OnOff = false;
 }
 
   if(str.equals("turn on 3\n"))
  {
        Serial.println("Turning on Relay 3\n");
        digitalWrite(socketD3Pin, 1);
        socketD3OnOff = true;
  }
 
 if(str.equals("turn off 3\n"))
 {
       Serial.println("Turning off Relay 3\n");
       digitalWrite(socketD3Pin, 0);
       socketD3OnOff = false;
 }
 
 //if they ask for current readings... socket 0
  if(str.equals("current 0\n"))
 {
   Serial.print("\ncurrent 0: ");
   Serial.print(socket0Power);
   Serial.println();
 }
 if(str.equals("current 0 high\n"))
 {
   Serial.print("\nThe High Current for Socket 0 is: ");
   Serial.print(socket0CurrentHigh);
   Serial.println();
 }
  if(str.equals("current 0 low\n"))
 {
   Serial.print("\nThe Low Current for Socket 0 is: ");
   Serial.print(socket0CurrentLow);
   Serial.println();
 }
 //socket1
   if(str.equals("current 1\n"))
 {
   Serial.print("\ncurrent 1: ");
   Serial.print(socket1Power);
   Serial.println();
 }
 if(str.equals("current 1 high\n"))
 {
   Serial.print("\nThe High Current for Socket 1 is: ");
   Serial.print(socket1CurrentHigh);
   Serial.println();
 }
  if(str.equals("current 1 low\n"))
 {
   Serial.print("\nThe Low Current for Socket 1 is: ");
   Serial.print(socket1CurrentLow);
   Serial.println();
 }
 //socket2
   if(str.equals("current 2\n"))
 {
   Serial.print("\ncurrent 2: ");
   Serial.print(socket2Power);
   Serial.println();
 }
 if(str.equals("current 2 high\n"))
 {
   Serial.print("\nThe High Current for Socket 2 is: ");
   Serial.print(socket2CurrentHigh);
   Serial.println();
 }
  if(str.equals("current 2 low\n"))
 {
   Serial.print("\nThe Low Current for Socket 2 is: ");
   Serial.print(socket2CurrentLow);
   Serial.println();
 }
 //socket3
   if(str.equals("current 3\n"))
 {
   Serial.print("\ncurrent3 3:");
   Serial.print(socket3Power);
   Serial.println();
 }
 if(str.equals("current 3 high\n"))
 {
   Serial.print("\nThe High Current for Socket 3 is: ");
   Serial.print(socket3CurrentHigh);
   Serial.println();
 }
  if(str.equals("current 3 low\n"))
 {
   Serial.print("\nThe Low Current for Socket 3 is: ");
   Serial.print(socket3CurrentLow);
   Serial.println();
 } 
}
