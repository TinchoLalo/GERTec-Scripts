#include <IRremote.h> 
#include <IRremoteInt.h>

/*$
  @Proyecto:    Robot Mini-Sumo
  @Autor:       Equipo Weber Bot
  @Placa:       Arduino NANO
  @Descripción: Código para robot minisumo, con sistema de detección infrarrojo y motores DC
*/

//========================== MODOS ========================== 

boolean Test    = false;      // Indica el modo pruebas del código
boolean LUCHA   = false;     // Indica si el sumo ha sido activado para competir
boolean TestMotor = false;
boolean linea   = false;


//========================== REFERENCIAS ========================== 
int ref[2] = {700,700}; // LINEA BLANCA
int dis[] = {50,45,90,135,180}; // 0, 45, 90, 135, 180



// ========================== SENSORES INFRAROJOS ==========================
int emisorIR       = 12;  // emisores infrarrojos controlados a través de transistor 2n2222a
//int emisorLinea    = 2;  // emisores infrarrojos sigue linas 
const int sensor0        = 0;  // Receptores IR
const int sensor45       = 0;  //
const int sensor90       = 0;  //
const int sensor135      = 0;  //
const int sensor180      = 0;  //
const int sensor270      = 0;  //

// ========================== SENSORES SIGUE LINEAS  ==========================
int sensorA  = A2;
int sensorB  = A1;

int valorLinea1   = 0;
int valorLinea2  = 0;

// ========================== MOTORES ==========================================
int motorR1        = 5;  // Pin Motor Derecha Adelante
int motorR2        = 2;   // Pin Motor Derecha Atras
int motorL1        = 4;   // Pin Motor Izquierda Adelante
int motorL2        = 3;   // Pin Motor Izquierda Atras


// ========================== LEDS INDICADORES ==================================
int ledTest        = 13;  // Led indicador encendido
int ledR           = A3;   // Led indicador Ready, semaforo
int ledG           = A5;  // Led indicador LUCHA!
int ledB           = A4;  // Led indicador otros

//== ULTRASONICO
int disp = 6;
int eco = 7;
long tPulso;
float  dstMedida;

//  ========================= CONTROL REMOTO =========================//
int receptorIR = 10;
IRrecv receptorIr(receptorIR);
decode_results codigoLeido;


//========================= START =========================//

void setup() {
  Serial.begin(9600);
  Serial.println("Weber Bot iniciando..."); //
  
  receptorIr.enableIRIn();

  //configuración de los pines de entrada y salida
  
  pinMode(ledTest, OUTPUT);
  pinMode(emisorIR, OUTPUT);
  pinMode(eco, INPUT);
  pinMode(disp, OUTPUT);
  digitalWrite(disp, LOW);

  //pinMode(emisorLinea, OUTPUT);
  pinMode(ledR, OUTPUT);
  pinMode(ledG, OUTPUT);  
 // pinMode(ledB, OUTPUT);
  //pinMode(bGND, OUTPUT);
  

  digitalWrite(ledR, HIGH);
  digitalWrite(ledG, HIGH);
  digitalWrite(ledB, HIGH);

  pinMode(motorR1, OUTPUT);
  pinMode(motorR2, OUTPUT);
  pinMode(motorL1, OUTPUT);
  pinMode(motorL2, OUTPUT);

  analogWrite(motorR1, 0);
  analogWrite(motorR2, 0);
  analogWrite(motorL1, 0);
  analogWrite(motorL2, 0);

  pinMode(sensorA, INPUT);
  pinMode(sensorB, INPUT);

  // 5 segundos
  for(int i=0; i<5; i++){
    digitalWrite(ledR, HIGH); // LED ROJO
    digitalWrite(ledG, LOW);
    digitalWrite(ledB, LOW);
    delay(500);
    digitalWrite(ledR, LOW); // LED ROJO
    digitalWrite(ledG, LOW);
    digitalWrite(ledB, LOW);
    delay(500);    
  }  
}

//========================= UPDATE =========================//
void loop(){
 
  if(LUCHA) {
    Serial.println("<<< MODO LUCHA >>>");
    lucha();
    /* 
    if (receptorIr.decode(&codigoLeido)) { 
      Serial.println(codigoLeido.value, HEX);
      lucha();
      
      switch (codigoLeido.value) {
        case 0xCB9A:
          for(int i=0; i<5; i++){
            digitalWrite(ledR, LOW);
            delay(500);
            digitalWrite(ledR, HIGH);
            delay(500);      
          } 
      receptorIr.resume();
    }
    */
    

  }//cierra el modo lucha

  else if (Test && !TestMotor) {
    Serial.println("<<< MODO TEST >>>");
    test();
  }

  else if (TestMotor){
    testMotor();
  }
  
  else {
    error();
    Serial.println("<<< ERROR: modo no seleccionado >>>");
  }


}//cierra el loop

////////////////////////////////////////////////
//Programas !!

void lucha(){do {
  valorLinea1 = analogRead(sensorA); 
  valorLinea2 = analogRead(sensorB); 

  digitalWrite(disp, LOW);
  //delayMicroseconds(25);
  digitalWrite(disp, HIGH);
  delayMicroseconds(10); 
  digitalWrite(disp, LOW);
  tPulso = pulseIn(eco, HIGH);

  // *************** Linea Blanca ***************
  if (valorLinea1 > ref[0] || valorLinea2 > ref[0]){ 
    //Serial.println("Linea!");
    digitalWrite(ledR, HIGH); // LED ROJO
    digitalWrite(ledG, LOW);
    digitalWrite(ledB, LOW);
    atras();
    delay(300);
    rotarR();
    delay(100);
    
  }
  // ************** Pista **************
  else if(valorLinea1 <= ref[0] || valorLinea2 <= ref[1]){ 

    // ************** Detectar Enemigo Cerca **************
    if(tPulso <= 600 ){
      //Serial.println("Detecta Cerca!");
      adelante();
      digitalWrite(ledB, HIGH); // LED AZUL
      digitalWrite(ledG, LOW);
      digitalWrite(ledR, LOW);
    } 
    // ************** Detectar Enemigo MEDIA **************
    else if(tPulso <= 900 ){
      //Serial.println("Detecta Cerca!");
      adelante();
      digitalWrite(ledB, HIGH); // LED AZUL
      digitalWrite(ledG, LOW);
      digitalWrite(ledR, LOW);
    } 
    else if(tPulso <= 1300 ){
      //Serial.println("Detecta Cerca!");
      adelante();
      digitalWrite(ledB, HIGH); // LED AZUL
      digitalWrite(ledG, LOW);
      digitalWrite(ledR, LOW);
    } 
    else {
      // Buscar
      rotarL(170,170);
      digitalWrite(ledG, HIGH); // LED VERDE
      digitalWrite(ledB, LOW);
      digitalWrite(ledR, LOW);
    }
    //Serial.println(tPulso);
    digitalWrite(ledG, HIGH); // LED VERDE
    digitalWrite(ledB, LOW);
    digitalWrite(ledR, LOW); 
  }   

  else{
    // Buscar
    rotarL(170,170);
    digitalWrite(ledG, HIGH); // LED VERDE
    digitalWrite(ledB, LOW);
    digitalWrite(ledR, LOW);
  }
  
} while(true);
}




//funcion test(#)

void test() {
  digitalWrite(disp, LOW);
  delayMicroseconds(50);
  digitalWrite(disp, HIGH);
  delayMicroseconds(10); 
  digitalWrite(disp, LOW);
  
  tPulso = pulseIn(eco, HIGH);

  Serial.print("tPulso- ");
  Serial.println(tPulso);

  valorLinea1 = analogRead(sensorA);
  valorLinea2 = analogRead(sensorB);

  Serial.print(valorLinea1);
  Serial.print(" - ");

  Serial.print(valorLinea2);
  Serial.print(" - ");

  //Prueba LEDs
  digitalWrite(ledG, HIGH);
  digitalWrite(ledB, LOW);
  digitalWrite(ledR, LOW);

 
  delay(1000);
}

void testMotor(){
  
  
  rotarR();
  delay(1000);
  rotarL(100,100);
  delay(1000);
  adelante();
  delay(1000);
  atras();
  delay(1000);
  stop();
  
  //taladro();
  //willy();

}

void error(){
  Serial.println("Error desconocido..");
  digitalWrite(ledTest, HIGH);
  //reset();
}

/*
priorizar el sensor linea, si detecta linea retrocer y girar 180º
Activar renotamente, el robot espara 5s, con secuencia de
pitidos y ledR, luego comienza la secuencia lucha, ledV y ledB.
escanear y perseguir al contrincante (seguir al contrincante
 si no se detecta una linea)
La condicion va a ser que el valor de los sigue lineas sea "negro"
escanear, mover, repetir secuencia.
sino se cumple la condicion de estar dentro del color negro porque llegamos al limite
entonces ejecutar la funcion, "daleAtras();""
*/

void atras(){
  analogWrite(motorR1, 0);
  analogWrite(motorR2, 255);
  analogWrite(motorL1, 0);
  analogWrite(motorL2, 255);
}
void adelante(){
  analogWrite(motorR1, 255); // motor derecho adelante
  analogWrite(motorR2, 0);
  analogWrite(motorL1, 255);
  analogWrite(motorL2, 0);
}
void adelante2(){
  analogWrite(motorR1, 150); // motor derecho adelante
  analogWrite(motorR2, 0);
  analogWrite(motorL1, 150);
  analogWrite(motorL2, 0);
}

void adelanteR(){
  analogWrite(motorR1, 255); // motor derecho adelante
  analogWrite(motorR2, 0);
  analogWrite(motorL1, 0);
  analogWrite(motorL2, 0);
}

void adelanteL(){
  analogWrite(motorR1, 0); // motor derecho adelante
  analogWrite(motorR2, 0);
  analogWrite(motorL1, 255);
  analogWrite(motorL2, 0);
}


void stop(){
  analogWrite(motorR1, 0);
  analogWrite(motorR2, 0);
  analogWrite(motorL1, 0);
  analogWrite(motorL2, 0);
}
void rotarR(){
  analogWrite(motorR1, 255);
  analogWrite(motorR2, 0);
  analogWrite(motorL1, 0);
  analogWrite(motorL2, 255);
}
void rotarL(int speedR, int speedL){
  analogWrite(motorR1, 0);
  analogWrite(motorR2, speedR);
  analogWrite(motorL1, speedL);
  analogWrite(motorL2, 0);
}

//Funciones divertidas

void taladro(){

  for(int C=0; C<200; C++){
  digitalWrite(ledG, HIGH);
  digitalWrite(ledB, HIGH);
  digitalWrite(ledR, LOW);
  rotarR();
  delay(40);
  stop();
  digitalWrite(ledG, HIGH);
  digitalWrite(ledB, LOW);
  digitalWrite(ledR, HIGH);
  rotarL(100,100);
  delay(40);
  }
}

void willy(){

  for(int C=0; C<10; C++){
  analogWrite(motorR1, 0);
  analogWrite(motorR2, 255);
  analogWrite(motorL1, 0);
  analogWrite(motorL2, 255);
  delay(500);
  analogWrite(motorR1, 255);
  analogWrite(motorR2, 0);
  analogWrite(motorL1, 255);
  analogWrite(motorL2, 0);
  delay(500);
  }
}
