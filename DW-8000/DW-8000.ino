#include <IRremote.h> 
#include <IRremoteInt.h>


/*
  @Proyecto:    Robot Mini-Sumo
  @Autor:       Equipo Weber Bot
  @Placa:       Arduino UNO
  @Descripción: Código para robot minisumo, con sistema de detección infrarrojo y motores DC
*/

//========================== MODOS ========================== 

boolean Test = false;      // Indica el modo pruebas del código
boolean Lucha   = !Test;     // Indica si el sumo ha sido activado para competir
boolean TestMotor = false;
boolean linea   = false;


//========================== REFERENCIAS ========================== 
int ref = 930;
int senState = 5; //variable para los sensores
int low = 100;


// ========================== SENSORES INFRAROJOS ==========================
//int emisorIR       = 12;  // emisores infrarrojos controlados a través de transistor 2n2222a
//int emisorLinea    = 2;  // emisores infrarrojos sigue linas 

int valor0 = 0;  //inicializamos el valor que va a corresonder con la lectura de los sensores

// ========================== SENSORES SIGUE LINEAS  ==========================
int sensorLinea  = A2;

int valorLinea   = 0;


// ========================== MOTORES ==========================================
int motorR1        = 10;  // Pin Motor Derecha Adelante
int motorR2        = 11;   // Pin Motor Derecha Atras
int motorL1        = 5;   // Pin Motor Izquierda Adelante
int motorL2        = 6;   // Pin Motor Izquierda Atras


int R1 = 0;
int R2 = 0;
int L1 = 0;
int L2 = 0;

// ========================== LEDS INDICADORES ==================================
int ledTest    = 13;  // Led indicador encendido
int ledR       = 8;   // Led indicador Ready, semaforo
int ledG       = 7;  // Led indicador LUCHA!
int ledB       = 9;  // Led indicador otros

//===================== ULTRASONICO ======================
int disp = 3;
int eco = 2;
long tPulso;
float  dstMedida;

//  ========================= CONTROL REMOTO =========================//
int receptorIR = 12;
IRrecv receptorIr(receptorIR);
decode_results codigoLeido;



//========================= START =========================//

void setup() {
  Serial.begin(9600);
  Serial.println("Weber Bot iniciando..."); //
  
  receptorIr.enableIRIn();

  //configuración de los pines de entrada y salida

 // pinMode(emisorIR, OUTPUT);
  pinMode(eco, INPUT);
  pinMode(disp, OUTPUT);
  digitalWrite(disp, LOW);

  //pinMode(emisorLinea, OUTPUT);
  pinMode(ledR, OUTPUT);
  pinMode(ledG, OUTPUT);  
  pinMode(ledB, OUTPUT);


 // pinMode(botonA, INPUT);
  pinMode(motorR1, OUTPUT);
  pinMode(motorR2, OUTPUT);
  pinMode(motorL1, OUTPUT);
  pinMode(motorL2, OUTPUT);

  analogWrite(motorR1, 0);
  analogWrite(motorR2, 0);
  analogWrite(motorL1, 0);
  analogWrite(motorL2, 0);


  // tomar la referencia 
  //ref = analogRead(sensorLinea)+ 100;

}

//========================= UPDATE =========================//
void loop(){
  if(Lucha) {
    //Serial.println("<<< MODO LUCHA >>>");
    
    
    if (receptorIr.decode(&codigoLeido)) { 
      Serial.println(codigoLeido.value, HEX);


      for(int i=0; i<5; i++){
        digitalWrite(ledR, HIGH); 
        delay(50);
        digitalWrite(ledR, LOW);
        delay(50);    
      }  

      lucha(); 
    
      receptorIr.resume();
      
    }
    
  }
  else if (Test && !TestMotor) {
    test();
  }

  else if (TestMotor){
    testMotor();
  }
  
  else {
  //error();
  Serial.println("<<< ERROR: modo no seleccionado >>>");
  }


}//cierra el loop

/////////////////////////////////
//Programas !!


//===========================  MODO LUCHA ===========================

void lucha(){do {
    valorLinea = analogRead(sensorLinea);  

    if (valorLinea > ref){ // linea blanca
          senState = 1;
          digitalWrite(ledR, HIGH);
          digitalWrite(ledG, LOW);
          digitalWrite(ledB, LOW);
        }
        else if(valorLinea < ref){ 
          digitalWrite(disp, LOW);
          //delayMicroseconds(25);
          digitalWrite(disp, HIGH);
          delayMicroseconds(10); 
          digitalWrite(disp, LOW);
          tPulso = pulseIn(eco, HIGH);
          //Serial.println(tPulso);
          digitalWrite(ledG, LOW);
          digitalWrite(ledR, LOW);
    
          if(tPulso <= 500){
            senState = 2;
            //Serial.println("Detecta!");
            digitalWrite(ledB, HIGH);
          }    
          else if(tPulso > 500 && tPulso <= 1200){
            senState = 3;
            //Serial.println("Detecta!");
            digitalWrite(ledB, HIGH);
          }   
          else if(tPulso > 1200 && tPulso < 1400){
            senState = 4;
            //Serial.println("Detecta!");
            digitalWrite(ledB, HIGH);
            
          }   
          else {
            senState = 5;
            digitalWrite(ledG, HIGH);
            digitalWrite(ledB, LOW);
            digitalWrite(ledR, LOW);
          }
        }
        else{
          senState = 5;
        }
        
        //==================  LOCALIZACIÓN  ==================
        switch (senState) {
            case 0:// Stop
              stop();
              break;
            case 1: // Linea blanca
              atras(255,255);
              delay(100);
              rotarR(200,200);
              delay(50);
              break;
            case 2:// Detectar Enemigo Cerca
              rotarR(255,255);
              //delay(200);
              break;
            case 3: // Detectar Enemigo Media
              adelante(100,230);
              delay(100);
              break;
            case 4: // Detectar Enemigo Lejos
              adelante(100,255);
              delay(100);
              break;
            case 5:
              // Buscar
              adelante(80,80);
              break;
        }

  
    

    //concluimos
    /*
    analogWrite(motorR1, R1);
    analogWrite(motorR2, R2);
    analogWrite(motorL1, L1);
    analogWrite(motorL2, L2);
    */
    //delay(500);
  
} while(true);
}


void testMotor(){
  analogWrite(motorR1, 255);
  analogWrite(motorR2, 0);
  analogWrite(motorL1, 255);
  analogWrite(motorL2, 0);
  delay(3000);
  analogWrite(motorR1, 0);
  analogWrite(motorR2, 255);
  analogWrite(motorL1, 0);
  analogWrite(motorL2, 255);
  delay(3000);
}

void test() {

  digitalWrite(disp, LOW);
  delayMicroseconds(50);
  digitalWrite(disp, HIGH);
  delayMicroseconds(10); 
  digitalWrite(disp, LOW);
  
  tPulso = pulseIn(eco, HIGH);
  valorLinea = analogRead(sensorLinea);
  receptorIr.decode(&codigoLeido);

  Serial.print(" IR ");
  Serial.print(codigoLeido.value, HEX);
  Serial.print(" Pulso ");
  Serial.print(tPulso);
  Serial.print(" - Linea ");
  Serial.println(valorLinea);
  delay(200);
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



void atras(int rSpeed, int lSpeed){
  analogWrite(motorR1, 0);
  analogWrite(motorR2, rSpeed);
  analogWrite(motorL1, 0);
  analogWrite(motorL2, lSpeed);
}
void adelante(int rSpeed, int lSpeed){
  analogWrite(motorR1, rSpeed); 
  analogWrite(motorR2, low);
  analogWrite(motorL1, lSpeed);
  analogWrite(motorL2, low);
}

void buscar(){
  analogWrite(motorR1, 100); 
  analogWrite(motorR2, 0);
  analogWrite(motorL1, 50);
  analogWrite(motorL2, 0);
}

void stop(){
  analogWrite(motorR1, 0);
  analogWrite(motorR2, 0);
  analogWrite(motorL1, 0);
  analogWrite(motorL2, 0);
}
void rotarR(int rSpeed, int lSpeed){
  analogWrite(motorR1, rSpeed);
  analogWrite(motorR2, low);
  analogWrite(motorL1, low);
  analogWrite(motorL2, lSpeed);
}
void rotarL(int rSpeed, int lSpeed){
  analogWrite(motorR1, low);
  analogWrite(motorR2, rSpeed);
  analogWrite(motorL1, lSpeed);
  analogWrite(motorL2, low);
}
