const int array_size = 30;
String actuators[array_size];
String sensors[array_size];
String ser_recv = "";

void setup() 
{
    for (int i=0; i < array_size; i++)
    {
        actuators[i] = "";
        sensors[i] = "";
    }
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
        ser_recv = Serial3.readStringUntil(terminator);
        Serial3.write(ser_recv);
        Serial.print(ser_recv);
    }
    
	  // used the echoed character to switch the onboard LED
  	if (actuator_cmd[0] == 'A')
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
