//These values are in the datasheet
#define RT0 10000   // Ω
#define B 3977      // K
//--------------------------------------

#define VCC 5    //Supply voltage
#define R 10000  //R=10KΩ
 
//Variables
float RT, VR, ln, TX, T0, VRT;
int power;
 
void setup() {
  Serial.begin(9600);
  T0 = 25 + 273.35;                 //Temperature T0 from datasheet, conversion from Celsius to kelvin
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, INPUT);
  pinMode(6, OUTPUT);
  
}

void setColor(int redValue, int greenValue,  int blueValue) {
  analogWrite(8, redValue);
  analogWrite(9,  greenValue);
  analogWrite(10, blueValue);
}

void loop() {
  power = digitalRead(11);
  VRT = analogRead(A0);              //Acquisition analog value of VRT
  VRT = (5.00 / 1024.00) * VRT;      //Conversion to voltage
  VR = VCC - VRT;
  RT = VRT / (VR / R);               //Resistance of RT
 
  ln = log(RT / RT0);
  TX = (1 / ((ln / B) + (1 / T0))); //Temperature from thermistor
 
  TX = TX - 273.35;                 //Conversion to Celsius 
 
  Serial.print("Temperature:");
  Serial.print("\t");
  Serial.print(TX);
  Serial.print("C\t");
  Serial.print("Power: ");
  Serial.print(power);
  Serial.print("\t");
  delay(500);
  if(power == 0){   //Power Off i.e. IDLE
    setColor(0, 0, 0);
    digitalWrite(7, HIGH);
    Serial.println("State - IDLE");
  }
  if (power == 1){   //Power on
    setColor(255, 255, 255);
    digitalWrite(7, HIGH);
  
    if(TX<25.00){   //Heating
      setColor(255,255,0);
      digitalWrite(7,LOW);
      Serial.println("State - Heating");
    }
    if(TX>30.00){  //Overheating
      setColor(255,0,0);
      tone(6, 262, 250);
      digitalWrite(7,HIGH);
      Serial.println("State - Overheating");
    }
    if(TX>=26.00 && TX<28){  //Stablizing
      setColor(0,0,255);
      digitalWrite(7,LOW);
      Serial.println("State - Stablizing");
    }
    if(TX>28 && TX<30){  //Target Reached
      setColor(0,255,0);
      digitalWrite(7,HIGH);
      Serial.println("State - Target Reached");
    }
  } 
}