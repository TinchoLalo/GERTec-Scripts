#include <Wire.h>

// MOTORES
int motorB1 = 7;
int motorB2 = 8;
int motorA1 = 4;
int motorA2 = 3;

int Low = 0;
int Linea = 400;
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
int sensores[6] = {A1, A2, A3, A4, A5};




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
  // Central
  if (v3 > Linea) {
    analogWrite(motorA1, 255);
    analogWrite(motorA2, 0);
    analogWrite(motorB1, 255);
    analogWrite(motorB2, 0);
  }
  // Laterales

  else if (v1 > Linea){
    analogWrite(motorA1, 255);
    analogWrite(motorA2, 190);
    analogWrite(motorB1, 0);
    analogWrite(motorB2, 0);
    delay(200);
  }

  else if (v2 > Linea) {
    analogWrite(motorA1, 255);
    analogWrite(motorA2, 190);
    analogWrite(motorB1, 100);
    analogWrite(motorB2, 0);
    delay(100);
  }
  else if (v4 > Linea){
    analogWrite(motorA1, 100);
    analogWrite(motorA2, 0);
    analogWrite(motorB1, 255);
    analogWrite(motorB2, 190);
    delay(100);
  }
  // Puntas
    else if (v1 > Linea){
    analogWrite(motorA1, 255);
    analogWrite(motorA2, 0);
    analogWrite(motorB1, 255);
    analogWrite(motorB2, 190);
    delay(200);
  }
  
  
  else {
    analogWrite(motorA1, 255);
    analogWrite(motorA2, 180);
    analogWrite(motorB1, 255);
    analogWrite(motorB2, 180);
    delay(50);
  
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
  //CLP = analogRead(sensores[5]);
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