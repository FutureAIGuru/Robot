char inByte = 0;

void setup() 
{
    Serial.begin(115200);
    while (!Serial);
    Serial3.begin(115200);
  	pinMode(13, OUTPUT);
    digitalWrite(13, HIGH);
}

void loop() 
{
    if (Serial3.available()) 
    {
        inByte = Serial3.read();
        Serial3.write(inByte);
        // Serial.print("[");
        Serial.print(inByte);
        // Serial.print("]");
    }
	  // used the echoed character to switch the onboard LED
  	if (inByte == '0')
    {
        //Serial.println("");
        //Serial.println("LED OFF");
        digitalWrite(13, LOW);
  	}
	  if (inByte == '1')
	  {
        //Serial.println("");
        //Serial.println("LED ON");
        digitalWrite(13, HIGH);
	  }
}
