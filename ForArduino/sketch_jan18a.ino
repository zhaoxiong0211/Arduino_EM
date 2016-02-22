int analogPin = 0;
int analogPin2 = 1;
int MAXLOOP = 17;
int MAINSVPP = 340;
int cur_offset = 301;
float CURRENTNORM = 15.5;

int count = 0;
int count_avgwatt = 0;
float vol_rawData[17];
float cur_rawData[17];
float voltagedata[17];
float ampdata[17];
float wattdata[17];
float watt_result = 0.0;

const int analogOutPin = 9; // Analog output pin that the LED is attached to
int outputValue = 0; // Analog output pin that the LED is attached to

void setup() {
  // put your setup code here, to run once:
 Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
 if (count < MAXLOOP){
// put your main code here, to run repeatedly:
  // read the raw data coming in on analog pin 0:
  int lightLevel = analogRead(analogPin);
  int lightLevel2 = analogRead(analogPin2);
  // Convert the raw data value (0 - 1023) to voltage (0.0V - 5.0V):
  float voltage = lightLevel2;
  float current = lightLevel;
  vol_rawData[count] = voltage;
  cur_rawData[count] = current;
  count = count + 1;
  if (count == MAXLOOP){
    float maxV = getMax(vol_rawData);
    float minV = getMin(vol_rawData);
    float avgV = 0.5 * (maxV + minV);
    float vpp = maxV - minV;
    float avgwatt = 0;
    Serial.println("Start here");
    for (int i=0; i<MAXLOOP; i++){
      voltagedata[i] = (vol_rawData[i] - avgV) * MAINSVPP / vpp;
      ampdata[i] = (cur_rawData[i] - cur_offset) / CURRENTNORM;
      if (ampdata[i] < 0){
        ampdata[i] = -ampdata[i];
      }
      if (voltagedata[i] < 0){
        voltagedata[i] = -voltagedata[i];
      }
      wattdata[i] = voltagedata[i] * ampdata[i];
      avgwatt = avgwatt + wattdata[i];
      
//      Serial.print(voltagedata[i]);
//      Serial.print(',');
//      Serial.println(ampdata[i]);
    }
    avgwatt = avgwatt / MAXLOOP;
    count = 0;
    if (count_avgwatt < 5){
      watt_result = watt_result + avgwatt / 5;
      count_avgwatt = count_avgwatt + 1;
    }
    else
    {
      outputValue = map(watt_result, 0, 200, 0, 51);
      // change the analog out value:
      analogWrite(analogOutPin, outputValue);
      count_avgwatt = 0;
      Serial.print("watt_result: ");
      Serial.println(watt_result);
      watt_result = 0.0;
    }
    delay(2000);
  }
 }
}

float getMax(float array[]) 
{
  float max_tem = 0.0;
  for (int i=0; i<MAXLOOP; i++)
  {
    if (max_tem < array[i])
    {
      max_tem = array[i];
    }
  }
  return max_tem;
}


float getMin(float array[]) 
{
  float min_tem = 1024.00;
  for (int i=0; i<MAXLOOP; i++)
  {
    if (min_tem > array[i])
    {
      min_tem = array[i];
    }
  }
  return min_tem;
}

