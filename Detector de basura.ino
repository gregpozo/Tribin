#include <Servo.h> 
#include <Metro.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVOMIN  	 100 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  	 700 // this is the 'maximum' pulse length count (out of 4096)
#define ATRAS_BASURA     750
#define ALANTE_BASURA    100
#define STOP_BASURA      652
#define ATRAS_METAL	 100
#define ALANTE_METAL 700
#define STOP_METAL   333
#define ATRAS_PP	   100
#define ALANTE_PP	   800
#define STOP_PP		   463
#define STOP_FEED    385
Servo PALETA_BASURA;
Servo PUERTA_ZAFACON;

// our servo # counter
uint8_t SERVO_IZQ = 2;
uint8_t SERVO_DER = 1;
uint8_t PLASTICO_PAPEL = 3;
uint8_t METAL = 7;
uint8_t BASURA = 6;
int INDUC1 = 8;
int INDUC2 = 9;
int INDUC3 = 11;
int INDUC4 = 12;
int LIMIT_FEEDER_SUP = 7;
int LIMIT_FEEDER_INF = 6;
int VIBRADOR1 = 53;
int VIBRADOR2 = 52;
int BEAGLE_PAPEL = 50;
int BEAGLE_BASURA = 48;
int SWITCHFOOT = 23;
int PALETA_STANDBY = 0;
int METAL_CHECK = 0;
int PAPEL_CHECK = 0;
int BASURA_CHECK = 0;
int PALETA_CHECK = 0;
int ESTADO_FEED = 0;
int SENSOR1 = 33;
int SENSOR2 = 35;
int SENSOR3 = 37;
int SENSOR4 = 39;
Metro TIEMPO_ALANTE_PAL = Metro(12000);
Metro SHOT_FIND_TIME = Metro(30000);
Metro TIEMPO_BAJADA = Metro(900);
Metro TIEMPO_SUBIDA = Metro(700);

void setup() {
  Serial.begin(9600);
  pinMode(INDUC1, INPUT);
  pinMode(INDUC2, INPUT);
  pinMode(INDUC3, INPUT);
  pinMode(INDUC4, INPUT);
  pinMode(LIMIT_FEEDER_SUP, INPUT);
  pinMode(LIMIT_FEEDER_INF, INPUT);
  pinMode(SWITCHFOOT, INPUT_PULLUP);
  pinMode(VIBRADOR2, OUTPUT);
  pinMode(VIBRADOR1,OUTPUT);
  pinMode(SENSOR4,OUTPUT);
  pinMode(SENSOR3, OUTPUT);
  pinMode(SENSOR2, OUTPUT);
  pinMode(SENSOR1, OUTPUT);
  pwm.begin();
  
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
  
}

// you can use this function if you'd like to set the pulse length in seconds
// e.g. setServoPulse(0, 0.001) is a ~1 millisecond pulse width. its not precise!
void setServoPulse(uint8_t n, double pulse) {
  double pulselength;
  
  pulselength = 1000000;   // 1,000,000 us per second
  pulselength /= 60;   // 60 Hz
  Serial.print(pulselength); Serial.println(" us per period"); 
  pulselength /= 4096;  // 12 bits of resolution
  Serial.print(pulselength); Serial.println(" us per bit"); 
  pulse *= 1000;
  pulse /= pulselength;
  Serial.println(pulse);
  pwm.setPWM(n, 0, pulse);
}

void loop() 
{
  ////PALETA_BASURA.write(90);
  //digitalWrite(SENSOR1,HIGH);
  //digitalWrite(SENSOR2,HIGH);
  //digitalWrite(SENSOR3,HIGH);
  //digitalWrite(SENSOR4,HIGH);
  int buttonState1 = digitalRead(INDUC1);
  int buttonState2 = digitalRead(INDUC2);
  int buttonState3 = digitalRead(INDUC3);
  int buttonState4 = digitalRead(INDUC4);
  int buttonState5 = digitalRead(LIMIT_FEEDER_SUP);
  int buttonState6 = digitalRead(LIMIT_FEEDER_INF);
  int buttonState7 = digitalRead(BEAGLE_BASURA);
  int buttonState8 = digitalRead(BEAGLE_PAPEL);
  int buttonState9 = digitalRead(SWITCHFOOT);
  if(buttonState9 == 1)
  {
    abrirZafacon();    
  }
  //if(buttonState5 == 1)
  //{
      //pwm.setPWM(SERVO_IZQ,0,STOP_FEED);
      //pwm.setPWM(SERVO_DER,0,STOP_FEED);
  //    delay(5);
  //}
  //fullStopServos();
  PALETA_BASURA.attach(10);
  PALETA_BASURA.writeMicroseconds(1360);
  bestShotFinder();
  ProcesoAlimentador(buttonState5,buttonState6);
  encontrarMetales();
  if((buttonState4 == 0 || buttonState3 == 0 || buttonState2 == 0 || buttonState1 == 0) && METAL_CHECK == 0)
  {
    metalEncontrado();
    pwm.setPWM(METAL,0,ALANTE_METAL);
    delay(57000);
    pwm.setPWM(METAL,0,STOP_METAL);
    delay(2000);
  }
  ////if(buttonState8 == 0 && PAPEL_CHECK == 0)
  ////{
  ////  bestShotFinder(); 
  ////}
  if(buttonState8 == 1 && PAPEL_CHECK == 0)
  {
    //PAPEL_CHECK = 1;
    papelEncontrado();  
    golpeoBasura();
    pwm.setPWM(PLASTICO_PAPEL,0, ALANTE_PP);
    delay(33000);
    fullStopServos();
    ESTADO_FEED = 0;
  }
  if(buttonState7 == 1 && BASURA_CHECK == 0)
  {
    //BASURA_CHECK = 1;
    basuraEncontrada();
    golpeoBasura();
    pwm.setPWM(BASURA,0,ALANTE_BASURA);
    delay(55000);
    fullStopServos();
    ESTADO_FEED = 0;    
  }
  //ProcesoAlimentador(buttonState5,buttonState6);
  //PALETA_BASURA.write(81);
  //PALETA_BASURA.writeMicroseconds(1360);
  //golpeoBasura();
  //PALETA_BASURA.write(80);
  //delay(1000);

  //pwm.setPWM(BASURA,0,ALANTE_BASURA);
  //delay(2000);
  //pwm.setPWM(BASURA,0,STOP_BASURA);
  //delay(60000000);

  //pwm.setPWM(METAL,0,ALANTE_METAL);
  //delay(2000);
  //pwm.setPWM(METAL,0,STOP_METAL);
  //delay(60000000);

  //pwm.setPWM(METAL,0,STOP_METAL);
  //pwm.setPWM(PLASTICO_PAPEL,0,ALANTE_PP);
  //delay(1000);
  //pwm.setPWM(PLASTICO_PAPEL,0,STOP_PP);
  //delay(60000000);


}

/**
*
* servo de metal (7)-------------  700 hacia adelante, 100 hacia atras, 332 para detenerse
* servo basura general (5)-------------- 800 hacia atras, 100 adelante, 650 para detenerse
paleta a 0, 79 stop
servo de plasticos (3)--------------- 100 hacia atras, 800 hacia adelante, 464 para detenerse
servos de movimiento se detienen en 385
**/


void ProcesoAlimentador(int limitFeedSup, int limitFeedInf)
{
  if(BASURA_CHECK == 0 && PAPEL_CHECK == 0 && METAL_CHECK == 0)
  {
    PALETA_CHECK = 1;
    if(ESTADO_FEED == 0)
    {
      digitalWrite(VIBRADOR1, HIGH);
      digitalWrite(VIBRADOR2, HIGH);
      pwm.setPWM(SERVO_IZQ,0,100);
      pwm.setPWM(SERVO_DER,0,100);       
    }

    if(limitFeedSup == 1)
    {
      pwm.setPWM(SERVO_IZQ,0,STOP_FEED);
      pwm.setPWM(SERVO_DER,0,STOP_FEED);
      delay(2000);
      ESTADO_FEED = 1;
      pwm.setPWM(SERVO_IZQ,0,100);
      pwm.setPWM(SERVO_DER,0,100);
      delay(700);
    }

    if(ESTADO_FEED == 1)
    {
      digitalWrite(VIBRADOR1, HIGH);
      digitalWrite(VIBRADOR2, HIGH);
      pwm.setPWM(SERVO_IZQ,0,STOP_FEED);
      pwm.setPWM(SERVO_DER,0,STOP_FEED);
      delay(500);
      PALETA_CHECK = 0;        
    }
  }
}

void encontrarMetales()
{
  if(PALETA_CHECK == 0)
  {
    if(PALETA_STANDBY == 0)
    {
      PALETA_BASURA.attach(10);
      PALETA_BASURA.write(77);
      if(TIEMPO_ALANTE_PAL.check() == 1)
      {
        PALETA_BASURA.write(82);
        delay(5500);
        PALETA_STANDBY = 1;
        PALETA_BASURA.writeMicroseconds(1360);
        PALETA_CHECK = 1;   
      }      
    }
    else
    {
      PALETA_BASURA.writeMicroseconds(1360);
    }
  }
}

void metalEncontrado()
{
  //METAL_CHECK = 1;
  PALETA_STANDBY = 1;
  PALETA_BASURA.writeMicroseconds(1360);
  pwm.setPWM(METAL,0,ATRAS_METAL);            //Hacia atras
  delay(55000);
  pwm.setPWM(METAL,0,STOP_METAL);
  delay(2000);
  golpeoBasura();
  PALETA_BASURA.writeMicroseconds(1360);
  ESTADO_FEED = 0;
}

void golpeoBasura()
{
  PALETA_BASURA.write(70);
  delay(250);
  PALETA_BASURA.writeMicroseconds(1360);
  delay(1000); 
  PALETA_BASURA.write(70);
  delay(500);
  PALETA_BASURA.writeMicroseconds(1360);
  delay(1000); 
  PALETA_BASURA.write(70);
  delay(400);
  PALETA_BASURA.writeMicroseconds(1360);
  delay(1000); 
  PALETA_BASURA.write(70);
  delay(300);
  PALETA_BASURA.writeMicroseconds(1360);
  delay(1000); 
  PALETA_BASURA.write(70);
  delay(250);
  PALETA_BASURA.writeMicroseconds(1360);
  delay(1000); 
  PALETA_BASURA.write(70);
  delay(200);
  PALETA_BASURA.writeMicroseconds(1360);
  delay(1000); 
  PALETA_BASURA.write(70);
  delay(250);
  PALETA_BASURA.writeMicroseconds(1360);
  delay(1000);
  PALETA_BASURA.write(70);
  delay(500);
  PALETA_BASURA.writeMicroseconds(1360);
  delay(1000); 
  PALETA_BASURA.write(70);
  delay(400);
  PALETA_BASURA.writeMicroseconds(1360);
  delay(1000); 
  PALETA_BASURA.write(70);
  delay(300);
  PALETA_BASURA.writeMicroseconds(1360);
  delay(1000);
  PALETA_BASURA.write(70);
  delay(250);
  PALETA_BASURA.writeMicroseconds(1360);
  delay(1000); 
  PALETA_BASURA.write(70);
  delay(200);
  PALETA_BASURA.writeMicroseconds(1360);
  delay(1000); 
  PALETA_BASURA.write(81);
  delay(5000);
}

void fullStopServos()
{
  pwm.setPWM(SERVO_IZQ,0,STOP_FEED);
  pwm.setPWM(SERVO_DER,0,STOP_FEED);
  pwm.setPWM(METAL,0,STOP_METAL);
  pwm.setPWM(BASURA,0,STOP_BASURA);
  pwm.setPWM(PLASTICO_PAPEL,0,STOP_PP);
  PALETA_BASURA.writeMicroseconds(1360);
}

void bestShotFinder()
{
  if(PALETA_CHECK == 1)
  {
    PALETA_BASURA.attach(10);
    PALETA_BASURA.writeMicroseconds(1360);
    if(SHOT_FIND_TIME.check() == 1)
    {
      PALETA_BASURA.write(70);
      delay(800);      
    }
    PALETA_BASURA.writeMicroseconds(1360);
    if(SHOT_FIND_TIME.check() == 1)
    {
      PALETA_BASURA.write(70);
      delay(800);      
    }
    PALETA_BASURA.writeMicroseconds(1360);
    if(SHOT_FIND_TIME.check() == 1)
    {
      PALETA_BASURA.write(82);
      delay(3500);      
    }
    PALETA_BASURA.writeMicroseconds(1360);
    delay(2000);      
  }
}

void papelEncontrado()
{
  pwm.setPWM(PLASTICO_PAPEL,0, ATRAS_PP);
  delay(35000);
  pwm.setPWM(PLASTICO_PAPEL,0, STOP_PP);
  delay(2000);
}

void basuraEncontrada()
{
  pwm.setPWM(BASURA,0,ATRAS_BASURA);
  delay(55000);
  pwm.setPWM(BASURA,0,STOP_BASURA);
  delay(2000);
}

void abrirZafacon()
{
  PUERTA_ZAFACON.attach(9);
  PUERTA_ZAFACON.write(150);
  delay(5000);
  PUERTA_ZAFACON.write(0);
}
