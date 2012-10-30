/*
* A Web page used to control the pin's
* The Wireless configuration parameters were borrowed from Arduino.cc in order to 
* get the Wifi Board to comunicate with the router.
*/


#include <WiServer.h>
#include <string.h>
#include <ctype.h>

#define WIRELESS_MODE_INFRA   1
#define WIRELESS_MODE_ADHOC   2

#define ledPin1 5
#define ledPin2 6
#define ledPin3 7


#define bit9600Delay 84  
#define halfBit9600Delay 42
#define bit4800Delay 188 
#define halfBit4800Delay 94 

byte rx = 8;
byte tx = 9;
byte SWval;

// Wireless configuration parameters ----------------------------------------
unsigned char local_ip[] = {192,168,1,102};	// IP address of WiShield
unsigned char gateway_ip[] = {192,168,1,1};	// router or gateway IP address
unsigned char subnet_mask[] = {255,255,255,0};	// subnet mask for the local network
const prog_char ssid[] PROGMEM = {"SmartHome"};		// max 32 bytes

unsigned char security_type = 0;	// 0 - open; 1 - WEP; 2 - WPA; 3 - WPA2

// WPA/WPA2 passphrase
const prog_char security_passphrase[] PROGMEM = {"DeadBeef"};	// max 64 characters

// WEP 128-bit keys
// sample HEX keys
prog_uchar wep_keys[] PROGMEM = {	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d,	// Key 0
									0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00,	// Key 1
									0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00,	// Key 2
									0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00	// Key 3
								};

// setup the wireless mode
// infrastructure - connect to AP
// adhoc - connect to another WiFi device
unsigned char wireless_mode = WIRELESS_MODE_INFRA;

unsigned char ssid_len;
unsigned char security_passphrase_len;

// End of wireless configuration parameters ----------------------------------------

boolean states[3]; //Led states
char stateCounter, tmpStrCat[64], stateBuff[4], numAsCharBuff[2], ledChange; //Variable declarations

/*
Check the conversion to display on the page so we can see if the url is on or off
*/
void boolToString (boolean tests, char returnBuff[4]){
  returnBuff[0] = '\0';
  if (tests){
    strcat(returnBuff, "On");}
  else{
    strcat(returnBuff, "Off");}
}
/*
State print counter
*/
void printStates(){
        for (stateCounter = 0 ; stateCounter < 3; stateCounter++){
            boolToString(states[stateCounter], stateBuff);
            int count = stateCounter;
           
           Serial.print("turn on ");
           Serial.print(count);
           Serial.print(": ");
           Serial.println(stateBuff);} 
          }

void writeStates()
{
        //set led states
        digitalWrite(ledPin1, states[0]);
        digitalWrite(ledPin2, states[1]);
        digitalWrite(ledPin3, states[2]);
}
void SWprint(int data)
{
    byte mask;
  //startbit
  digitalWrite(tx,LOW);
  delayMicroseconds(bit9600Delay);
  for (mask = 0x01; mask>0; mask <<= 1) {
    if (data & mask){ // choose bit
     digitalWrite(tx,HIGH); // send 1
    }
    else{
     digitalWrite(tx,LOW); // send 0
    }
    delayMicroseconds(bit9600Delay);
  }
  //stop bit
  digitalWrite(tx, HIGH);
  delayMicroseconds(bit9600Delay);
}

int SWread()
{
  byte val = 0;
  while (digitalRead(rx));
  //wait for start bit
  if (digitalRead(rx) == LOW) {
    delayMicroseconds(halfBit9600Delay);
    for (int offset = 0; offset < 8; offset++) {
     delayMicroseconds(bit9600Delay);
     val |= digitalRead(rx) << offset;
    }
    //wait for stop bit + extra
    delayMicroseconds(bit9600Delay); 
    delayMicroseconds(bit9600Delay);
    return val;
  }
}
// Here the page gets served
boolean sendPage(char* URL) {
  
    printStates();
    writeStates();
    
  //Using to check if the URL needs to change
  if (URL[1] == '?' && URL[2] == 'L' && URL[3] == 'E' && URL[4] == 'D') //url has a leading /
  {
    ledChange = (int)(URL[5] - 48); //get the led to change.
    
    for (stateCounter = 0 ; stateCounter < 3; stateCounter++)
    {
      if (ledChange == stateCounter)
      {
        states[stateCounter] = !states[stateCounter];
          //  Serial.print("Have changed ");
            Serial.println(ledChange);
      }
    }
    
    //after having change state, return the user to the index page.
    WiServer.print("<HTML><HEAD><meta http-equiv='REFRESH' content='0;url=/'></HEAD></HTML>");
    return true;
  }
  
  if (strcmp(URL, "/") == false) //why is this not true?
   {
     
      WiServer.print("<html><head><title>Led switch</title></head>");
    
      WiServer.print("<body><center>Please select the led state:<center>\n<center>");
      for (stateCounter = 0; stateCounter < 3; stateCounter++) //for each led
      {
        numAsCharBuff[0] = (char)(stateCounter + 49); //as this is displayed use 1 - 3 rather than 0 - 2
        numAsCharBuff[1] = '\0'; //strcat expects a string (array of chars) rather than a single character.
                                 //This string is a character plus string terminator.
        
        tmpStrCat[0] = '\0'; //initialise string
        strcat(tmpStrCat, "<a href=?LED"); //start the string
        tmpStrCat[12] = (char)(stateCounter + 48); //add the led number
        tmpStrCat[13] = '\0'; //terminate the string properly for later.
    
        strcat(tmpStrCat, ">Led ");
        strcat(tmpStrCat, numAsCharBuff);
        strcat(tmpStrCat, ": ");
        
        boolToString(states[stateCounter], stateBuff);
        strcat(tmpStrCat, stateBuff);
        strcat(tmpStrCat, "</a> "); //we now have something in the range of <a href=?LED0>Led 0: Off</a>
    
        WiServer.print(tmpStrCat);
      }
        
     //Declare the variables to find the temperature
  float temp_in_celsius = 0;
  float temp_in_kelvin=0;
  float temp_in_fahrenheit=0;
  
 //Reads the input and converts it to Kelvin degrees
  temp_in_kelvin = analogRead(0) * 0.004882812 * 100;
  //Converts Kelvin to Celsius minus 2.5 degrees error
  temp_in_celsius = temp_in_kelvin - 2.5 - 273.15; 
  temp_in_fahrenheit = ((temp_in_kelvin - 2.5) * 9 / 5) - 459.67;
  //Temperature
        float h = temp_in_kelvin;
        float t = temp_in_celsius;
        float f = temp_in_fahrenheit;
        float laser = analogRead(1);
        String txt;
        
        //Check to see if the Laser is tripped or not 
        if(analogRead(1)<810){
          txt = "Laser Tripped";
        }else{
          txt = "Laser Not Tripped";
        }  
        // check if returns if not a number then it failed
        if (isnan(t) || isnan(h)) {
        	WiServer.print("<html>");  //HTML Code
         	WiServer.print("Failed to read from Sensor Input");
        	WiServer.println("        </html>");
        } else {
        	WiServer.print("<html>");
        	WiServer.print("<center><H2>Room 1 Temperature Statistics: ID: 1</H2><br><br><br>");

        WiServer.print("Temperature Kelvin: ");
        WiServer.print(h);
        WiServer.print(" T\t");
        WiServer.print("Temperature Celsius ");
        WiServer.print(t);
        WiServer.println(" *C");
        WiServer.print("Temperature Fehrenhite ");
        WiServer.print(f);
        WiServer.println(" *F");
        WiServer.println("        ");
        WiServer.print("</center>");
        WiServer.print("<center><H3>Laser TripWire Detection in Room 1</H3><br><br><br>");
        WiServer.print("Detection: ");
        WiServer.print(txt);
        WiServer.println("        ");
        WiServer.print(laser);
        WiServer.print("</html></center>");        // URL was recognized
        return true;
    }
    // URL not found
    return false;
    
        WiServer.print("</html> ");
        return true;
   }
}

void setup() {
  // Initialize WiServer and have it use the sendMyPage function to serve pages
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(3, OUTPUT);

  // Enable Serial output and ask WiServer to generate log messages (optional)
 // Serial.begin(57600);
  
  WiServer.enableVerboseMode(true);
  Serial.begin(9600);
  WiServer.init(sendPage);
  states[0] = false;
  states[1] = false;
  states[2] = false;
  pinMode(rx,INPUT);
  pinMode(tx,OUTPUT);
  digitalWrite(tx,HIGH);
  digitalWrite(13,HIGH); //turn on debugging LED
  SWprint('h');  //debugging hello
  SWprint('i');
  SWprint(10); //carriage return
}

void loop(){
  if(analogRead(1)<810){
          digitalWrite(3, HIGH);
        }else{
          digitalWrite(3, LOW);
        }  
  // Run WiServer
  WiServer.server_task();
  
    SWval = SWread(); 
    SWprint(toupper(SWval));
  delay(10);
}
