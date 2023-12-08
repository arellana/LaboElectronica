const int dirPin = 8;
const int stepPin = 9;
 
float steps;
int microPausa = 10;
int i;
void setup() {
 pinMode(dirPin, OUTPUT);
 pinMode(stepPin, OUTPUT);
}

void loop() {


for (i = 1; i < 2; i++){
  digitalWrite(dirPin, HIGH);  // Establezco una dirección

    int mil = 15;

    steps = mil/0.00625;

  for (int x = 0; x < steps ; x++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(microPausa);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(microPausa);
    }

  } 
}
