// INITIALIZE HARDWARE
int signPin_1 = 2;
int noisePin_1 = 3;
int signPin_2 = 4;
int noisePin_2 = 5;

// MEASURMENT VARIAVLES
int sign_1 = 0;
int noise_1 = 0;
int sign_2 = 0;
int noise_2 = 0;
int sON_1 = 0;
int nON_1 = 0;
int sON_2 = 0;
int nON_2 = 0;
int signCount_1 = 0;
int noiseCount_1 = 0;
int signCount_2 = 0;
int noiseCount_2 = 0;

// TIME VARIABLES
unsigned long int total_time = 0;
unsigned long int currTime = 0;
unsigned long int prevTime = 0;
unsigned int diffTime = 0;
long int loop_index = 0;
// CPM

double cpm_1 = 0;
double cpm_2 = 0;

void setup() {
  //Serial setup
  //9600bps
  Serial.begin(9600);

  //PIN setting for Radiation Pulse
  pinMode(signPin_1, INPUT);
  digitalWrite(signPin_1, HIGH);
  pinMode(signPin_2, INPUT);
  digitalWrite(signPin_2, HIGH);

  //PIN setting for Noise Pulse
  pinMode(noisePin_1, INPUT);
  digitalWrite(noisePin_1, HIGH);
  pinMode(noisePin_2, INPUT);
  digitalWrite(noisePin_2, HIGH);

  prevTime = millis();

  // INITIALIZE SERIAL OUTOUT
  Serial.println("[time, ms] [GC1] [GC2]");
}

void loop() {
    //////////////
    // GEIGER 1 //
    //////////////
   // Raw data of Radiation Pulse: Not-detected -> High, Detected -> Low
  int sign_1 = digitalRead(signPin_1);
  int noise_1 = digitalRead(noisePin_1);
   //Radiation Pulse normally keeps low for about 100[usec]
  if (sign_1 == 0 && sON_1 == 0)
  { //Deactivate Radiation Pulse counting for a while
    sON_1 = 1;
    signCount_1++;
    Serial.println("Yes!");
  } else if (sign_1 == 1 && sON_1 == 1) {
    sON_1 = 0;
  }
  //Noise Pulse normally keeps high for about 100[usec]
  if (noise_1 == 1 && nON_1 == 0)
  { //Deactivate Noise Pulse counting for a while
    nON_1 = 1;
    noiseCount_1++;
  } else if (noise_1 == 0 && nON_1 == 1) {
    nON_1 = 0;
  }

     //////////////
    // GEIGER 2 //
    //////////////
  // Raw data of Noise Pulse: Not-detected -> Low, Detected -> High
  int sign_2 = digitalRead(signPin_2);
  int noise_2 = digitalRead(noisePin_2);
   //Radiation Pulse normally keeps low for about 100[usec]
  if (sign_2 == 0 && sON_2 == 0)
  { //Deactivate Radiation Pulse counting for a while
    sON_2 = 1;
    signCount_2++;
  } else if (sign_2 == 1 && sON_2 == 1) {
    sON_2 = 0;
  }
  //Noise Pulse normally keeps high for about 100[usec]
  if (noise_2 == 1 && nON_2 == 0)
  { //Deactivate Noise Pulse counting for a while
    nON_2 = 1;
    noiseCount_2++;
  } else if (noise_2 == 0 && nON_2 == 1) {
    nON_2 = 0;
  }

  //Output readings to serial port, after 10000 loops
  if (loop_index == 100000) //About 100 * 160-170 msec in Arduino Nano(ATmega328)
  {
    //Get current time
    currTime = millis();

    // GEIGER 1
    //No noise detected in 10000 loops
    if (noiseCount_1 == 0)
    {
      diffTime = currTime - prevTime;
      cpm_1 = ((1.0 * signCount_1) / diffTime) * 60000.0;
    }
    else {
      cpm_1 = -1;
    }

    // GEIGER 2
    //No noise detected in 10000 loops
    if (noiseCount_2 == 0)
    {
      diffTime = currTime - prevTime;
      cpm_2 = ((1.0 * signCount_2) / diffTime) * 60000.0;
    }
    else {
      cpm_2 = -1;
    }
    Serial.print(total_time);
    Serial.print(" ");
    Serial.print(cpm_1);
    Serial.print(" ");
    Serial.println(cpm_2);

    // RESET VARIABLES
    cpm_1 = 0;
    cpm_2 = 0;
    noiseCount_1 = 0;
    signCount_1 = 0;
    noiseCount_2 = 0;
    signCount_2 = 0;
    loop_index = 0;
    prevTime = millis();
    total_time = millis();
  }
  loop_index++;
}
