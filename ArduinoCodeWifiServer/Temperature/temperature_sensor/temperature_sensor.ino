/*
 * temperature.pde
 * -----------------
 * Takes the sensor (LM335) input and converts it
 * to Kelvin degrees, then celsius.
 * Subtract 2.5 celsius degrees that seem 
 * to be the sensor error perhaps due to the heat 
 * generating by the current running through it.
 * 2.5 degrees were calculated by using a mercury 
 * thermometer next to the sensor.
*/
/*Declare the Variables for the Program*/

float temp_in_celsius = 0;
float temp_in_kelvin=0;
float temp_in_fahrenheit=0;

void setup()
{
  Serial.begin(9600); 
}
void loop()
{
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
  delay(1000); 
}
