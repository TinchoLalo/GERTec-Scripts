// MOTORES
int motorB1 = 7;
int motorB2 = 8;
int motorA1 = 4;
int motorA2 = 3;

int Low = 0;
int Linea = 900;
int Delay= 50;
int State= 0;
bool lineDetected = false;

// VALOR SENSORES
int v1=0;
int v2=0;
int v3=0;
int v4=0;
int v5=0;

int CLP=0;

// SENSORES
int sensores[6] = {A1, A2, A3, A4, A5, A0};



////////////////////////////////////////////////////////////////
void ReadSensors();

void setup() {
  // Iniciamos el puerto de comunicación
  Serial.begin(9600);
  Serial.println("FRANCHESCO SIGUELINI Bot iniciando...");
  ReadSensors();

}

void loop() {
 
  ReadSensors();
  //PrintValue();

  ///////////////// SEGUIR LINEA /////////////////
  if (CLP > Linea){
    Parar();
    State=0;
  }
  // Central
  else if (v3 > Linea && State == 0) {
    Adelante(200, 200);
    //delay(Delay);
  }
  // Laterales
  else if (v2 > Linea) {
    Adelante(140, 100);
    State=1;
    //delay(Delay);
  }
  else if (v4 > Linea){
    Adelante(100, 140);
    State=2;
    //delay(Delay);
  }
  // Puntas
  else if (v1 > Linea){
    Atras2(240, 100);
    State=1;
    //delay(50);
    //Adelante(220, 100);
  }
  else if (v5 > Linea){
    Atras(240, 100);
    State=2;
    //delay(50);
    //Adelante(100, 220);
  }
  else if (State==1 || v3 < Linea){
    Adelante(150, 220);
    State = 3;
    //delay(20);
    }
  else if (State==2 || v3 < Linea){
    Adelante(220, 150);
    State = 3;
    //delay(20);
  }
  else {
    Adelante(255, 255);
    //State = 0;
  }

}


////////////////////////////////////////////////////////////////

// LEER SENSORES
void ReadSensors(){
  v1 = analogRead(sensores[0]);
  v2 = analogRead(sensores[1]);
  v3 = analogRead(sensores[2]);
  v4 = analogRead(sensores[3]);
  v5 = analogRead(sensores[4]);
  CLP = analogRead(sensores[5]);
}

// MOSTRAR VALORES SENSORES
void PrintValue(){
  for (int i = 0; i <= 5; i++) {
    Serial.print(analogRead(sensores[i]));
    Serial.print(" - ");
  }
  Serial.print("\n");
  delay(500);
}


// MOVIMIENTOS
void Adelante(int SpeedA, int SpeedB){
  analogWrite(motorA1, SpeedA);
  analogWrite(motorA2, Low);
  analogWrite(motorB1, SpeedB);
  analogWrite(motorB2, Low);
}

void Atras(int SpeedA, int SpeedB){
  analogWrite(motorA1, Low);
  analogWrite(motorA2, SpeedB);
  analogWrite(motorB1, SpeedA);
  analogWrite(motorB2, Low);
}
void Atras2(int SpeedA, int SpeedB){
  analogWrite(motorA1, SpeedA);
  analogWrite(motorA2, Low);
  analogWrite(motorB1, Low);
  analogWrite(motorB2, SpeedB);
}

void Parar(){
  analogWrite(motorA1, Low);
  analogWrite(motorA2, Low);
  analogWrite(motorB1, Low);
  analogWrite(motorB2, Low);
}