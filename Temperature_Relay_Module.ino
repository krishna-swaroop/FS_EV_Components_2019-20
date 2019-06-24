/* The following code was written on 23-05-2019 for the design of Temperature Relay Module to be used in the shutdown circuit. 
   We used an analog multiplexer to connect the temperature sensors to the microcontroller. The multiplexed signal was stabilised using
   an op-amp with negative feedback. At the time of writing, the team is still in design stage. So an Arduino has been used to prototype 
   the circuits. We plan to use a specialised microcontroller for the same later on.
*/
// Inspired Karters FS, BITS Pilani
// Designed by D. Krishna Swaroop, 2017A3PS0315P


// Setting Multiplexer Pins
int MuxA0=9;     // A0 pin of Mux
int MuxA1=8;     // A1 pin of Mux
int MuxA2=7;     // A2 pin of Mux
int ModuleOut=13;// Output of Entire Segment
int MuxOut=6;    // Output of the Multiplexed signal
int Mod2Dash=5;  // Output to LED in Dash Board

// Temperature Sensor Parameters
float Vout=0.0;    // Vout in A0 
float Rout=0.0;    // Rout in A0

float Vin=5.0;     // [V]        
float Rt=10000;    // Resistor t [ohm]
float R0=10000;    // value of rct in T0 [ohm]
float T0=298.15;   // use T0 in Kelvin [K]

// refer to the datasheet of the temperature sensor to get this data.
float T1=273.15;      // [K] in datasheet 0º C
float T2=373.15;      // [K] in datasheet 100° C
float RT1=35563;   // [ohms]  resistence in T1
float RT2=549;    // [ohms]   resistence in T2
float beta=0.0;    // initial parameters [K]
float Rinf=0.0;    // initial parameters [ohm]   
float TempK=0.0;   // variable output
float TempC=0.0;   // variable output



void setup() {
  // put your setup code here, to run once:
pinMode(MuxOut,INPUT);
pinMode(MuxA0,OUTPUT);
pinMode(MuxA1,OUTPUT);
pinMode(MuxA2,OUTPUT);
pinMode(ModuleOut,OUTPUT);
pinMode(Mod2Dash,OUTPUT);


//parameters
  beta=(log(RT1/RT2))/((1/T1)-(1/T2));
  Rinf=R0*exp(-beta/T0);
}

void loop() {
  // put your main code here, to run repeatedly:


float ValSensor[7];
int A0BitVal[]={0,1,0,1,0,1,0,1};
int A1BitVal[]={0,0,1,1,0,0,1,1};
int A2BitVal[]={0,0,0,0,1,1,1,1};

for(int i=0; i<=7; i++)
{
  digitalWrite(MuxA0,A0BitVal[i]);                                   // Setting values of pins from pre-determined pattern stored in an array
  digitalWrite(MuxA1,A1BitVal[i]);                                   // This snippet sets the digits of sensor number expressed in binary
  digitalWrite(MuxA2,A2BitVal[i]);
  
  Vout=Vin*((float)(analogRead(MuxOut))/1024.0);                     // Conversion of Data from NTC Thermistor to Temperatures Based on values from datasheet
  Rout=(Rt*Vout/(Vin-Vout));

  TempK=(beta/log(Rout/Rinf));                                       // calc for temperature
  TempC=TempK-273.15;                                                // conversion from Kelvin to celcius for eaier viewing  incase a display is interfaced later
  ValSensor[i]=TempC;                                               // storing temperatures into float array
}
  
  
  
/* As of now, the temperatures are logged into arrays. 
   Now we check for the stability of anomalous signals. 
   If any signal value is above a threshold value, we introduce a delay and check if the anomalous value is still above the threshold vaue for 1sec as per rulebook.    
*/  
  for(int i=0; i<=7; i++)
  {
    if(ValSensor[i]>55)                                                // Comparing received value with threshold value and furthering the algorithm
    {
      delay(1000);
      digitalWrite(MuxA0,A0BitVal[i]);                                   // Setting values of pins from pre-determined pattern stored in an array
      digitalWrite(MuxA1,A1BitVal[i]);                                   // This snippet sets the digits of sensor number expressed in binary
      digitalWrite(MuxA2,A2BitVal[i]);
      
       
      Vout=Vin*((float)(analogRead(MuxOut))/1024.0);                     // Conversion of Data from NTC Thermistor to Temperatures Based on values from datasheet
      Rout=(Rt*Vout/(Vin-Vout));

      TempK=(beta/log(Rout/Rinf));                                       // calc for temperature
      TempC=TempK-273.15;
      if( TempC > 55)                                                   // the value 55 has been decided by the accumulator design team to comply with the rules of the competition.
      {
        digitalWrite(ModuleOut,HIGH);
        digitalWrite(Mod2Dash,HIGH);
      }
    } 
      else
      {
        digitalWrite(ModuleOut,LOW);
        digitalWrite(Mod2Dash,LOW);
      }
  
  }
}  
