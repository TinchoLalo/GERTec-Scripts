// MOTORES
int motorB1 = 5;
int motorB2 = 6;
int motorA1 = 10;
int motorA2 = 11;

int Low = 0;
int Linea = 800;
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
int sensores[6] = {A1, A5, A4, A3, A2, A0};



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
    Adelante(255, 255);
    //delay(Delay);
  }
  // Laterales
    else if (v2 > Linea) {
    Adelante(130, 100);
    State=1;
    //delay(Delay);
  }
  else if (v4 > Linea){
    Adelante(100, 130);
    State=2;
    //delay(Delay);
  }
  // Puntas
  else if (v1 > Linea){
    Atras2(145, 95);
    //delay(50);
    //Adelante(220, 100);
    State=1;  
  }
  else if (v5 > Linea){
    Atras(145, 95);
    //delay(50);
    //Adelante(100, 220);
    State=2;
    
  }
  
  else if (State==1){
    Adelante(100, 130);
    //delay(20);
  }
  else if (State==2){
    //Adelante(130, 100);
    //delay(20);

  }
  else {
    Adelante(255, 255);
  
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