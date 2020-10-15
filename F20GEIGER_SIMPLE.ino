// INITIALIZE HARDWARE
int signPin = 2;
int noisePin = 3;
// MEASURMENT VARIAVLES
int sign = 0;
int noise = 0;
int sON = 0;
int nON = 0;
int signCount = 0;
int noiseCount = 0;
// TIME VARIABLES
unsigned long int total_time = 0;
unsigned long int currTime = 0;
unsigned long int prevTime = 0;
unsigned int diffTime = 0;
int loop_index = 0;
// CPM
double cpm = 0;

void setup() {
  //Serial setup
  //9600bps
  Serial.begin(9600);

  //PIN setting for Radiation Pulse
  pinMode(signPin, INPUT);
  digitalWrite(signPin, HIGH);

  //PIN setting for Noise Pulse
  pinMode(noisePin, INPUT);
  digitalWrite(noisePin, HIGH);

  prevTime = millis();
}

void loop() {
   // Raw data of Radiation Pulse: Not-detected -> High, Detected -> Low
  int sign = digitalRead(signPin);

  // Raw data of Noise Pulse: Not-detected -> Low, Detected -> High
  int noise = digitalRead(noisePin);

   //Radiation Pulse normally keeps low for about 100[usec]
  if (sign == 0 && sON == 0)
  { //Deactivate Radiation Pulse counting for a while
    sON = 1;
    signCount++;
    Serial.println("Yes!");
  } else if (sign == 1 && sON == 1) {
    sON = 0;
  }

  //Noise Pulse normally keeps high for about 100[usec]
  if (noise == 1 && nON == 0)
  { //Deactivate Noise Pulse counting for a while
    nON = 1;
    noiseCount++;
    
  } else if (noise == 0 && nON == 1) {
    nON = 0;
  }

  //Output readings to serial port, after 10000 loops
  if (loop_index == 10000) //About 160-170 msec in Arduino Nano(ATmega328)
  {
    //Get current time
    currTime = millis();

    //No noise detected in 10000 loops
    if (noiseCount == 0)
    {
      diffTime = currTime - prevTime;
      cpm = ((1.0 * noiseCount) / diffTime) * 60000.0;
    }
    else {
      cpm = -1;
    }
    Serial.print(currTime);
    Serial.print(" ");
    Serial.println(cpm);

    // RESER VARIABLES
    cpm = 0;
    noiseCount = 0;
    signCount = 0;
    loop_index = 0;
    prevTime = millis();
    total_time = millis();
  }
  loop_index++;
}
