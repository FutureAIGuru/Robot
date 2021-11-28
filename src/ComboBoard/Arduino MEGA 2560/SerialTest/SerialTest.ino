int inByte = 0;

void setup() 
{
	Serial.begin(115200);   // com to PC
	while(!Serial);
    Serial3.begin(115200);  // com to ESP8266
  	pinMode(13, OUTPUT);
    digitalWrite(13, HIGH);
}

void loop() 
{
    // read from port 0, send to port 3:
    if (Serial.available()) 
    {
        inByte = Serial.read();
        Serial3.write(inByte);
    }
    // read from port 3, send to port 0:
    if (Serial3.available()) 
	{
        inByte = Serial3.read();
        Serial.write(inByte);
    }
	  // used the echoed character to switch the onboard LED
	if (inByte == '0')
    {
        digitalWrite(13, LOW);
	}
	if (inByte == '1')
	{
        digitalWrite(13, HIGH);
	}
}
