#include "I2Cdev.h"
//#include "Wire.h"

//PID

float Kp, Ki, Kd;
float L;
float integral, derivada, posIni, errPre;
long tiePre, tieNue;
float dt;
bool inicio;
float error, errorPre;
float pos , posPre;

float potencia, offset;
// MOTOR
const int dirPin = 8;
const int stepPin = 9;
float steps;
int microPausa = 15;//3

long valor;

void setup() {

  // MOTOR
  pinMode(dirPin, OUTPUT);
  pinMode(stepPin, OUTPUT);

  //GIRO
  Serial.begin(57600);    //Iniciando puerto serial

  //PID

  float kcrit  = 0.0002;
  Kp = 10;
  Ki = 0.0001;//0.0000001;//0000001.;
  Kd = 0;//0.05;//0.01;

  L = 175;

  integral = 0;
  derivada = 0;
  posPre = 0;
  errPre = 0;

  inicio = true;

}


void loop() {
  
  if (inicio == true){
    tiePre = millis();
    inicio = false;
    pos=0;
  }

  dt = (millis()-tiePre)/100000.0;
  
  offset = 340;
  potencia = analogRead(A0);
  error = -potencia + offset;

  tieNue = millis();
  dt = tieNue - tiePre;

  //Serial.println(1.2*(Kp/500)*3);

  integral += error*dt;
  derivada = (error - errPre)/dt;

  steps = Kp*error + Kd*derivada + Ki*integral;

  errorPre = error;
  tiePre = tieNue;

  //steps = 5000;
  if (steps>0){
    digitalWrite(dirPin, LOW);
  }
  if (steps<0){
    digitalWrite(dirPin, HIGH);
  }

  
 for (int x = 0; x < abs(steps) ; x++) {
   digitalWrite(stepPin, HIGH);
   delayMicroseconds(microPausa);
   digitalWrite(stepPin, LOW);
   delayMicroseconds(microPausa);
 }
  //Serial.println(error);
  //Serial.println(steps);
  //delay(1000);
  
}
