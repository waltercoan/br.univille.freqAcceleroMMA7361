#include <AcceleroMMA7361.h>
#include <MsTimer2.h>

const int sizeData = 256;
int analogArray[sizeData];
long timeArray[sizeData];

AcceleroMMA7361 accelero;
int x;
int count = 0;
long timeIni=0, timeEnd;
double freqacc=0, freqmed=0;
int freqcount = 0;
double freq = 0;
int actual=0, last =0;

void getWave(){
    timeArray[count] = millis();
    analogArray[count++] = accelero.getXAccel();
    if (count >= sizeData){
        count = 0;
        for(int i=0;i<sizeData;i++){
            if(analogArray[i] == 0){
                if(timeIni == 0){
                    timeIni = timeArray[i];
                }else{
                    timeEnd = timeArray[i];  
                    if((timeEnd - timeIni) > 10){
                        freq = 1.0 / ((timeEnd - timeIni) / 1000.0);
                        freqcount++;
                        freqacc = freqacc + freq;
                    }
                    timeEnd = 0;
                    timeIni = 0;
                }
            }
        }
        if(freqcount >= 2){
            delay(100);          
            freqmed = freqacc / freqcount;
            freqacc = 0;
            freqcount = 0;
            Serial.print("Freq: ");
            Serial.print(freqmed);               
            Serial.println(" Hz");
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
  Serial.println("Calibrado...");
  MsTimer2::set(1, getWave);
  MsTimer2::start();  
  Serial.println("Monitorando...");
}

void loop()
{
}
