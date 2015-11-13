#include <AcceleroMMA7361.h>
#include <MsTimer2.h>

const int sizeData = 256;
int analogArray[sizeData];
long timeArray[sizeData];

AcceleroMMA7361 accelero;
int x;
int count = 0;
int lastMax=0;
int lastMin=9999;
long timeIni=0, timeEnd;
double freqacc=0, freqmed=0;
int freqcount = 0;

void getWave(){
    timeArray[count] = millis();
    analogArray[count++] = accelero.getXAccel();
    if (count >= sizeData){
        count = 0;
        for(int i=0;i<sizeData;i++){
            if(lastMax < analogArray[i]){
                lastMax = analogArray[i];
                if(analogArray[i+1] < lastMax){
                    if(timeIni == 0){
                        timeIni = timeArray[i];
                    }else{
                        timeEnd = timeArray[i];
                        double freq = 1000.0 / (timeEnd - timeIni);
                        freqcount++;
                        freqacc = freqacc + freq;
                        timeIni = 0;
                        timeEnd = 0;
                        lastMax = 0;
                        lastMin = 9999;
                    }
                }
            }
            if(lastMin > analogArray[i]){
                lastMin = analogArray[i];
                if(analogArray[i+1] > lastMin){
                    lastMax = 0;
                    lastMin = 9999;
                }
            }    
        }
        if(freqcount >= 50){
            freqmed = freqacc / freqcount;
            freqacc = 0;
            freqcount = 0;
            Serial.print("Freq Media: ");
            Serial.print(freqmed);               
            Serial.println(" Hz");
            //MsTimer2::stop();
        }
    }
}

void setup()
{
  Serial.begin(9600);
  accelero.begin(13, 12, 11, 10, A0, A1, A2);
  accelero.setARefVoltage(3.3);                   //sets the AREF voltage to 3.3V
  accelero.setSensitivity(LOW);                   //sets the sensitivity to +/-6G
  accelero.calibrate();
  Serial.println("Calibado...");
  MsTimer2::set(1, getWave);
  MsTimer2::start();  
  Serial.println("Monitorando...");
}

void loop()
{
     if(Serial.available()){
       char in = Serial.read();
       if(in == 'r'){
           Serial.println("Begin...");
       }else{
           if(in == 'p'){
               
           }else{
               if(in == 'd'){
                   for(int i=0;i<sizeData;i++){
                       Serial.print(analogArray[i]);
                       Serial.print(";");
                   }
                   Serial.println("");
                   for(int i=0;i<sizeData;i++){
                       Serial.print(timeArray[i]);
                       Serial.print(";");
                   }                 
               }
           }
       }
       
   }

}
