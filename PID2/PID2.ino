#include "I2Cdev.h"
#include "MPU6050.h"
#include "Wire.h"

//PID

float Kp, Ki, Kd;
float L;
float integral, derivada, posIni, errPre;
long tiePre, tieNue;
float dt;
bool inicio;
float ang;
float error, errorPre;
float pos , posPre;

// GIRO
MPU6050 sensor;
float ang_x, ang_y;
float ang_x_prev = 0, ang_y_prev = 0;
int ax, ay, az;
int gx, gy, gz;

// MOTOR
const int dirPin = 8;
const int stepPin = 9;
float steps;
int microPausa = 100;//3



void setup() {

  // MOTOR
  pinMode(dirPin, OUTPUT);
  pinMode(stepPin, OUTPUT);

  //GIRO
  Serial.begin(57600);    //Iniciando puerto serial
  Wire.begin();           //Iniciando I2C  
  sensor.initialize();    //Iniciando el sensor

  if (sensor.testConnection()) Serial.println("Sensor iniciado correctamente");
  else Serial.println("Error al iniciar el sensor");

  //PID

  float kcrit  = 0.0002;
  Kp = 1;
  Ki = 0;//0.0000001;//0000001.;
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

  sensor.getAcceleration(&ax, &ay, &az);
  sensor.getRotation(&gx, &gy, &gz);
  
  dt = (millis()-tiePre)/100000.0;
  
  //Calcular los ángulos con acelerometro
  float accel_ang_y=atan(-ax/sqrt(pow(ay,2) + pow(az,2)))*(180.0/3.14);
  
  ang_y = 0.98*(ang_y_prev+(gy/131)*dt) + 0.02*accel_ang_y;
  
  ang_y_prev = ang_y;

  error = ang_y_prev;

  tieNue = millis();
  dt = tieNue - tiePre;

  //Serial.println(1.2*(Kp/500)*3);

  integral += error*dt;
  derivada = (error - errPre)/dt;

  steps = Kp*error+ Kd*derivada + Ki*integral;

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
  Serial.println(error);
  Serial.println(steps);
  //delay(1000);
  
}
