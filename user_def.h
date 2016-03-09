#define voice 30
#define INTERVAL_LCD 50 
#define maxNoise 5
#define mic_pin A0
#define PIN 6
#define buzzer_pin 10
#define key_pin 8


int voice_data;
double db;
long time, timer;
int numNoise=0;
boolean buzzer_speak;
boolean add;
int i = 200;
double recodeDB=0.0;