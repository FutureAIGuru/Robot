// Experiment aimed at transporting bytes from Serial to Serial3 and vice versa.

#define BUFF_SIZE   65

char MEGA2ESP[BUFF_SIZE];
char ESP2MEGA[BUFF_SIZE];
int numMEGA2ESP = 0;
int numESP2MEGA = 0;

void ClearBuffer(char *target)
{
  for (int i = 0; i < BUFF_SIZE; i++)
  {
    target[i] = 0x00;
  }
}

void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial3.begin(115200);
}

void loop() 
{
  ClearBuffer(MEGA2ESP);
  ClearBuffer(ESP2MEGA);

  numMEGA2ESP = Serial.available();
  numESP2MEGA = Serial3.available();

  if (numMEGA2ESP > 0) Serial.readBytes(MEGA2ESP, numMEGA2ESP);
  if (numESP2MEGA > 0) Serial3.readBytes(ESP2MEGA, numESP2MEGA);

  MEGA2ESP[numMEGA2ESP] = 0x00;
  ESP2MEGA[numESP2MEGA] = 0x00;

  Serial3.print(MEGA2ESP);
  Serial.print(ESP2MEGA);
}
