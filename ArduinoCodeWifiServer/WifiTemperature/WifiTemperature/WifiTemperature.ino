//Program to check the Temperature of the Room in 3 different Readings.

#include <WiServer.h>

#define WIRELESS_MODE_INFRA	1
#define WIRELESS_MODE_ADHOC	2

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
//---------------------------------------------------------------------------


// This is our page serving function that generates web pages
boolean sendMyPage(char* URL) {
  
  float temp_in_celsius = 0;
  float temp_in_kelvin=0;
  float temp_in_fahrenheit=0;
  
 //Reads the input and converts it to Kelvin degrees
  temp_in_kelvin = analogRead(0) * 0.004882812 * 100;
  //Converts Kelvin to Celsius minus 2.5 degrees error
  temp_in_celsius = temp_in_kelvin - 2.5 - 273.15; 
  temp_in_fahrenheit = ((temp_in_kelvin - 2.5) * 9 / 5) - 459.67;
  //Print the temperature in Celsius to the serial port
  Serial.print("Celsius: ");
  Serial.println(temp_in_celsius);                  
  //Print the temperature in Fahrenheit to the serial port
  Serial.print("Fahrenheit: ");
  Serial.println(temp_in_fahrenheit);
  Serial.print("Kelvin:  ");
  Serial.println(temp_in_kelvin);
  Serial.println();
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
        // check if returns are valid, if they are NaN (not a number) then something went wrong!
        if (isnan(t) || isnan(h)) {
        	WiServer.print("<html>");  //Here is the code for the html page
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
}

void setup() {
  // Initialize WiServer and have it use the sendMyPage function to serve pages
  WiServer.init(sendMyPage);
  pinMode(3, OUTPUT);

  // Enable Serial output and ask WiServer to generate log messages (optional)
  Serial.begin(57600);
  
  WiServer.enableVerboseMode(true);

}

void loop(){
if(analogRead(1)<810){
          digitalWrite(3, HIGH);
        }else{
          digitalWrite(3, LOW);
        }  
  // Run WiServer
  WiServer.server_task();
  delay(10);
}
