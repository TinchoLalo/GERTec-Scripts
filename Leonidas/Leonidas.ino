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
int ref = 850; // 930
int senState = 0; 
int low = 100;

// ========================== SENSORES SIGUE LINEAS  ==========================
int sensorLinea  = 0;
int valorLinea   = 0;
int valor0 = 0; 


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
int ledR       = 8;  
int ledG       = 7;  
int ledB       = 9;  

//===================== ULTRASONICO ======================
int disp = 3;
int eco = 2;
long tPulso;

//  ========================= CONTROL REMOTO =========================//
int receptorIR = 12;
//IRrecv receptorIr(receptorIR);
//decode_results codigoLeido;

//========================= START =========================//

void setup() {
  Serial.begin(9600);
  Serial.println("Weber Bot iniciando..."); //
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
    //Serial.println("<<< MODO LUCHA >>>");
    
    if (IrReceiver.decode()) { 
      IrReceiver.printIRResultShort(&Serial); // Print complete received data in one line
      IrReceiver.printIRSendUsage(&Serial);   // Print the statement required to send this data


      for(int i=0; i<5; i++){
        digitalWrite(ledR, HIGH); 
        delay(500);
        digitalWrite(ledR, LOW);
        delay(500);    
      }  

      lucha(); 
    
      IrReceiver.resume();
      
    }
    
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
    if(IrReceiver.decode()) {
      auto myRawdata= IrReceiver.decodedIRData.decodedRawData;
      // Adelante
      if (myRawdata == 3208707840)
      {
        R1= 255; 
        R2= 0; 
        L1= 255;
        L2= 0;
      }
      // Atras
      else if (myRawdata == 3860463360){
        R1= 0; 
        R2= 255; 
        L1= 0;
        L2= 255;
      }
      // Left
      else if (myRawdata == 4161273600){
        R1= 100; 
        R2= 0; 
        L1= 255;
        L2= 0;
      }
      // Right
      else if (myRawdata == 4127850240){
        R1= 255; 
        R2= 0; 
        L1= 100;
        L2= 0;
      }

      else {
        R1= 0; 
        R2= 0; 
        L1= 0;
        L2= 0;
      }
      analogWrite(motorR1, R1);
      analogWrite(motorR2, R2);
      analogWrite(motorL1, L1);
      analogWrite(motorL2, L2);
      delay(300);
      IrReceiver.resume();
    
  }
  }
}


///////////////////////////////////////////////////////////////////////////////////////////////////

//===========================  MODO LUCHA ===========================

void lucha(){do {
  valorLinea = analogRead(sensorLinea);  

  // *************** Linea Blanca ***************
  if (valorLinea < ref){ 
    // Leds
    digitalWrite(ledR, HIGH);
    digitalWrite(ledG, LOW);
    digitalWrite(ledB, LOW);

    R1= 0; 
    R2= 0; 
    L1= 0;
    L2= 0;

    analogWrite(motorR1, 0);
    analogWrite(motorR2, 255);
    analogWrite(motorL1, 0);
    analogWrite(motorL2, 140);
    delay(450);
    analogWrite(motorR1, 255);
    analogWrite(motorR2, 0);
    analogWrite(motorL1, 0);
    analogWrite(motorL2, 255);
    delay(220);


  }

  // ************** Pista **************
  else if(valorLinea > ref){ 
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
    if(tPulso <= 500){
      //Serial.println("Detecta!");
      R1=255; 
      R2=0; 
      L1=255;
      L2=0;
      digitalWrite(ledB, HIGH);
      digitalWrite(ledG, LOW);
    }    

    // ************** Detectar Enemigo Media **************
    else if(tPulso > 500 && tPulso <= 1200){
      //Serial.println("Detecta!");
      R1=150; 
      R2=0; 
      L1=150;
      L2=0;
      digitalWrite(ledB, HIGH);
      digitalWrite(ledG, LOW);
    }   

    // ************** Detectar Enemigo Lejos **************
    else if(tPulso > 1200 && tPulso < 1400){
      //Serial.println("Detecta!");
      R1=100; 
      R2=0; 
      L1=100;
      L2=0;
      digitalWrite(ledB, HIGH);
      digitalWrite(ledG, LOW);
      senState = 1;
    }   
    else if (senState == 1){
      R1=140; 
      R2=0; 
      L1=140;
      L2=0;
    }
    
    // ************** Buscar **************
    else {
      L1= 200;
      L2= 100;
      R1= 120;
      R2= 100;
      digitalWrite(ledG, HIGH);
      digitalWrite(ledB, LOW);
      digitalWrite(ledR, LOW);
      senState = 0;
    }
  }
  else{
    // Buscar
    L1= 200;
    L2= 100;
    R1= 120;
    R2= 100;
    senState = 0;
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
    if (myRawdata == 3141861120)
    {
      Serial.println("Hola");
      digitalWrite(ledR, HIGH); 
    }
    else {digitalWrite(ledR, LOW); }
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
  valorLinea = analogRead(sensorLinea);

  Serial.print(" IR ");
  Serial.print(" Pulso ");
  Serial.print(tPulso);
  Serial.print(" - Linea ");
  Serial.println(valorLinea);

}
