/*
DME pulse Count’
 */

// digital pin 2 has a pushbutton attached to it. Give it a name:

int dme0 = 2;
int dme1 = 3;
int dme2 = 4;

volatile unsigned int pulse0 = 0;
volatile unsigned int pulse1 = 0;
volatile unsigned int pulse2 = 0;
volatile byte bits = 0;
volatile unsigned long pulseTime = 0;
volatile unsigned long timeDiff =0;
volatile unsigned int twelveUS = 0;
volatile unsigned int thirtySixUS = 0;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  // make the DME PULSE pins inputs:  
  pinMode(dme0, INPUT);
  pinMode(dme1, INPUT);
  pinMode(dme2, INPUT);
  attachInterrupt(digitalPinToInterrupt(dme0), pulse, RISING);
}


// the loop routine runs over and over again forever:
void loop() {
  Serial.print(pulse0);
  pulse0=0;
  Serial.write(0x2c);
  Serial.print(pulse1);
  pulse1=0;
  Serial.write(0x2c);  
  Serial.print(pulse2);
  pulse2=0;  

  // work out percentage of 12 vs 36us
  int Percent12 = (int)((float)twelveUS/(twelveUS+thirtySixUS))*100;
  int Percent36 = (int)((float)thirtySixUS/(twelveUS+thirtySixUS))*100;
  twelveUS=0;
  thirtySixUS=0;
  
  Serial.write(0x2c);
  Serial.print(Percent12);
  Serial.write(0x2c);
  Serial.print(Percent36);
  Serial.write(0x0a);
  Serial.write(0x0d);

  delay(1000);        // delay in ms between reads for stability
}


void pulse() {
  bits = PIND;
  timeDiff = micros()-pulseTime;
  if ( timeDiff < 20 ){
    twelveUS ++;
  } else if ( timeDiff < 40 ) {
    thirtySixUS ++;
  }
  pulseTime = micros();
  pulse0 ++;
  if ( bits & 0b00001000 ){
    pulse1++;
  }
  if ( bits & 0b00010000 ){
    pulse2++;
  }
}

