#include <IRremote.hpp> 
/*
  @Proyecto:    Robot Mini-Sumo
  @Autor:       Equipo GERTec
  @Placa:       Arduino UNO
  @Descripción: Código para robot minisumo, con sistema de detección infrarrojo y motores DC
*/

//========================== MODOS ========================== 

boolean Test = false;      // Indica el modo pruebas del código
boolean Lucha   = !Test;   // Indica si el sumo ha sido activado para competir
boolean TestMotor = true; 
boolean TestIR = false;      // Indica el modo pruebas del código
boolean linea   = false;

//========================== REFERENCIAS ========================== 
int ref[2] = {40,40}; // 930
int senState = 0; 
int low = 100;

// ========================== SENSORES SIGUE LINEAS  ==========================
int sensorLinea1  = A1;
int sensorLinea2  = A2;
int valorLinea1   = 0;
int valorLinea2   = 0;
int valor0 = 0; 


// ========================== MOTORES ==========================================
int motorR1        = 2;  // Pin Motor Derecha Adelante
int motorR2        = 3;   // Pin Motor Derecha Atras
int motorL1        = 4;   // Pin Motor Izquierda Adelante
int motorL2        = 5;   // Pin Motor Izquierda Atras

int R1 = 0;
int R2 = 0;
int L1 = 0;
int L2 = 0;

// ========================== LEDS INDICADORES ==================================
int ledR       = A7;  
int ledG       = A6;  
int ledB       = A5;  

//===================== ULTRASONICO ======================
int disp = 6;
int eco = 7;
long tPulso;

//  ========================= CONTROL REMOTO =========================//
int receptorIR = 12;
//IRrecv receptorIr(receptorIR);
//decode_results codigoLeido;

//========================= START =========================//

void setup() {
  Serial.begin(9600);
  Serial.println("LEONIDAS iniciando..."); //
  IrReceiver.begin(receptorIR, ledB);
 

  //configuración de los pines de entrada y salida

  pinMode(eco, INPUT);
  pinMode(disp, OUTPUT);
  digitalWrite(disp, LOW);

  pinMode(ledR, OUTPUT);
  pinMode(ledG, OUTPUT);  
  pinMode(ledB, OUTPUT);

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
    Serial.println("<<< MODO LUCHA >>>");
    //if (IrReceiver.decode()) { 
      //auto myRawdata = IrReceiver.decodedIRData.decodedRawData;
      //Serial.println(myRawdata);
      //if (myRawdata != 0 || myRawdata > 1000) // ON
      
        for(int i=0; i<5; i++){
          digitalWrite(ledR, HIGH); 
          delay(500);
          digitalWrite(ledR, LOW);
          delay(500);    
        }  

        lucha(); 
      
        //IrReceiver.resume();
  
    
  }
  else if (Test && !TestMotor && !TestIR) {  
    test();
  }

  else if (TestMotor && !TestIR){
    testMotor();
  }

  else if (TestIR){
    testIRremote();
  }

  else {
  Serial.println("Nada");
  }
  
}


///////////////////////////////////////////////////////////////////////////////////////////////////

//===========================  MODO LUCHA ===========================

void lucha(){do {
  valorLinea1 = analogRead(sensorLinea1); 
  valorLinea2 = analogRead(sensorLinea2); 
  // *************** Linea Blanca ***************
  if (valorLinea1 < ref[0] || valorLinea2 < ref[1]){ 
    //Serial.println("Linea!");
    // Leds
    R1= 0; 
    R2= 0; 
    L1= 0;
    L2= 0;
    digitalWrite(ledR, HIGH);
    digitalWrite(ledG, LOW);
    digitalWrite(ledB, LOW);
    analogWrite(motorR1, 0);
    analogWrite(motorR2, 100);
    analogWrite(motorL1, 0);
    analogWrite(motorL2, 100);
    delay(1000);
    
  }
  // ************** Pista **************
  else if(valorLinea1 > ref[0] && valorLinea2 > ref[1]){ 
    //Serial.println("Pista!");
    digitalWrite(disp, LOW);
    //delayMicroseconds(25);
    digitalWrite(disp, HIGH);
    delayMicroseconds(10); 
    digitalWrite(disp, LOW);
    tPulso = pulseIn(eco, HIGH);
    //Serial.println(tPulso);
    digitalWrite(ledG, HIGH);
    digitalWrite(ledB, LOW);
    digitalWrite(ledR, LOW);

    // ************** Detectar Enemigo Cerca **************
    if(tPulso <= 150 && tPulso > 0){
      //Serial.println("Detecta Cerca!");
      R1=250; 
      R2=0; 
      L1=250;
      L2=0;
      digitalWrite(ledB, HIGH);
      digitalWrite(ledG, LOW);
    }    

    // ************** Detectar Enemigo Media **************
    else if(tPulso > 150 && tPulso <= 600){
      //Serial.println("Detecta Media!");
      R1=90; 
      R2=0; 
      L1=90;
      L2=0;
      digitalWrite(ledB, HIGH);
      digitalWrite(ledG, LOW);
    }   

    // ************** Detectar Enemigo Lejos **************
    else if(tPulso > 600 && tPulso <= 700){
      //Serial.println("Detecta Lejos!");
      R1=70; 
      R2=0; 
      L1=70;
      L2=0;
      digitalWrite(ledB, HIGH);
      digitalWrite(ledG, LOW);
  
    }   
    // ************** Buscar **************
    else {
      L1= 70;
      L2= 0;
      R1= 60;
      R2= 0;
      digitalWrite(ledG, HIGH);
      digitalWrite(ledB, LOW);
      digitalWrite(ledR, LOW);
    }
  }
  else{
    // Buscar
    digitalWrite(ledR, LOW);
    digitalWrite(ledG, HIGH);
  }
  
  //concluimos
  analogWrite(motorR1, R1);
  analogWrite(motorR2, R2);
  analogWrite(motorL1, L1);
  analogWrite(motorL2, L2);
  
  //delay(500);
  
} while(true);
}


//===========================  MODO TEST MOTOR ===========================
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

//===========================  MODO TEST IR remote ===========================
void testIRremote(){
  if(IrReceiver.decode()) {
    // Print complete received data in one line
    auto myRawdata= IrReceiver.decodedIRData.decodedRawData;
    Serial.println(myRawdata);
    IrReceiver.resume();
  }
}

//===========================  MODO TEST ===========================
void test() {
  digitalWrite(disp, LOW);
  delayMicroseconds(50);
  digitalWrite(disp, HIGH);
  delayMicroseconds(10); 
  digitalWrite(disp, LOW);
  
  tPulso = pulseIn(eco, HIGH);
  valorLinea1 = analogRead(sensorLinea1);
  valorLinea2 = analogRead(sensorLinea2);

  Serial.print(" IR ");
  Serial.print(" Pulso ");
  Serial.print(tPulso);
  Serial.print(" - Linea1 ");
  Serial.print(valorLinea1);
  Serial.print(" - Linea2 ");
  Serial.println(valorLinea2);
  delay(200);
}