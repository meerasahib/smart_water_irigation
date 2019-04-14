#include <DHT.h>          
#define DHTPIN 2     // what digital pin we're connected to

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

DHT dht(DHTPIN, DHTTYPE);

#include <SoftwareSerial.h>
int relayP=5;
int relayV=6;
int rain=4;
int water=3;
int s=0;
float tempC;
float tempF;

int reading;
float referenceVoltage;

int tempPin = 0; //Analog pin connected to LM35


float humidity;
SoftwareSerial gprsSerial(7,8);
// Specify data and clock connections and instantiate SHT1x object

void setup()
{
   //analogReference(INTERNAL);
 //referenceVoltage = 1.1; //Set to 5, 3.3, 2.56 or 1.1 depending on analogReference Setting
 
gprsSerial.begin(9600); // GPRS shield baud rate
pinMode(relayP,OUTPUT);
pinMode(relayV,OUTPUT);
digitalWrite(5,HIGH);
digitalWrite(6,HIGH);
pinMode(rain,INPUT);
pinMode(water,INPUT);
Serial.begin(38400); // Open serial connection to report values to host

 dht.begin();
delay(2000);
}
void loop()
{
int rain1=digitalRead(rain);
int water1=digitalRead(water);
//reading = 0;
//  
//  for(int i = 0; i < 20; i++) { // Average 10 readings for accurate reading
//     reading += analogRead(tempPin); 
//     delay(20);
//  }
//delay(1500);

  // A lot of examples divide the sensor reading by 1024. This is incorrect and should be 1023. There are 1024 values including 0 so this should be 1023.
  tempC =  0.48828*analogRead(A0);
  
// Read values from the sensor



  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  int humidity = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
if (tempC >27 &&humidity<72 && rain1==HIGH)
{
//  Serial.println(" NO RAIN ");
//SendTextMessage();
digitalWrite(relayV,LOW);
delay(200);
}
else
{
digitalWrite(relayV,HIGH);
}
if(water1==HIGH)
{
  //Serial.println("LOW WATER");
digitalWrite(relayP,LOW);
}
else
{
digitalWrite(relayP,HIGH);
}
if (water1==HIGH && rain1 == HIGH)
{
if(s<90)
//Serial.println(" NO RAIN & LOW WATER");
//Serial.println(humidity);
SendTextMessage();
delay(5000);
s++;
//Serial.println(s);
}
else
{
s=s-s;
}
Serial.println("tem=");
Serial.println(tempC);
Serial.println("humidity=");
Serial.println(humidity);
Serial.println("rain=");
Serial.println(rain1);
Serial.println("water=");
Serial.println(water1);
}
void SendTextMessage()
{
  humidity = dht.readHumidity();
  gprsSerial.begin(9600);
  gprsSerial.print("\r");
  delay(1000); 
gprsSerial.print("AT+CMGF=1\r"); // Set the shield to SMS mode
delay(1000);
// send sms message, the phone number needs to include the country code e.g. if a U.S. phone number such as (540) 898-5543 then the string must be:
// +15408985543
//gprsSerial.println("AT+CMGS = \"+919080552981\"\r");
gprsSerial.println("AT+CMGS = \"+966572804143\"\r");
 
delay(1000);
gprsSerial.println(" NO RAIN & LOW WATER"); //the content of the message
gprsSerial.println(" HUMIDITY =");
gprsSerial.println(humidity);
gprsSerial.println("TEMPERATURE =");
gprsSerial.println(tempC);

delay(1000);
gprsSerial.write(0x1A);//the ASCII code of the ctrl+z is 26 (required according to the datasheet)
delay(1000);
//gprsSerial.println();
// Serial.println("Text Sent.");
}
