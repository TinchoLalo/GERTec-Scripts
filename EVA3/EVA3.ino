#include <IRremote.h> 
#include <IRremoteInt.h>


/*
  @Proyecto:    Robot Mini-Sumo
  @Autor:       Equipo Weber Bot
  @Placa:       Arduino NANO
  @Descripción: Código para robot minisumo, con sistema de detección infrarrojo
*/


// ========= MODOS ===========
boolean TEST    = true;      // Indica el modo pruebas del código
boolean LUCHA   = !TEST;     // Indica si el sumo ha sido activado para competir


// ========================== SENSORES INFRAROJOS ==========================
const int sensorR       = A7;  // Receptores IR
const int sensorL       = A6;  //
//const int sensorC       = A5;  //
//const int sensorD      = A3;  //


int vsensorR = 0;  //inicializamos el valor que va a corresonder con la lectura de los sensores
int vsensorL = 0;

int refEyes[2] = {700,700}; // SENSORES OJOS
int refLine[3] = {1000,1000,1000}; // SENSORES LINEAS

// ========================== SENSORES SIGUE LINEAS  ==========================
const int lineaR  = A4;
const int lineaL  = A5;
const int lineaB  = A3;
//int sensorB  = A7;

int valorR   = 0;
int valorL   = 0;
int valorB   = 0;

// ========================== MOTORES ==========================================
#define pwmR 5
#define pwmL 6
#define motorL1 8
#define motorL2 7
#define motorR1 4
#define motorR2 3


// velocidades del motor
int max = 255; 
int mid = 125;

// ========================== LEDS INDICADORES ==================================
int ledTest        = 13;  // Led indicador encendido
int ledR           = 10;   // Led indicador Ready, semaforo
int ledG           = 11;  // Led indicador LUCHA!
int ledB           = 12;  // Led indicador otros

/*
int botonA       = 6;   // Pin de activación
int botonModo    = 2;
//int botonReset = ;
*/



//  ========================= CONTROL REMOTO =========================//
int receptorIR = 2;
decode_results codigoLeido;

unsigned long previousMillis = 0;
int step = 0;

//========================= START =========================//

void setup() {
  Serial.begin(9600);
  Serial.println("EVA el destructor iniciando...");
  IrReceiver.begin(receptorIR, DISABLE_LED_FEEDBACK); 
  

  //configuración de los pines de entrada y salida
  
  pinMode(ledTest, OUTPUT);

  pinMode(ledR, OUTPUT);
  pinMode(ledG, OUTPUT);  
  pinMode(ledB, OUTPUT);
  

  digitalWrite(ledR, LOW);
  digitalWrite(ledG, LOW);
  digitalWrite(ledB, LOW);

  pinMode(motorR1, OUTPUT);
  pinMode(motorR2, OUTPUT);
  pinMode(motorL1, OUTPUT);
  pinMode(motorL2, OUTPUT);

  analogWrite(motorR1, 0);
  analogWrite(motorR2, 0);
  analogWrite(motorL1, 0);
  analogWrite(motorL2, 0);

  pinMode(sensorR, INPUT);
  pinMode(sensorL, INPUT);
  pinMode(lineaR, INPUT);
  pinMode(lineaL, INPUT);
  pinMode(lineaB, INPUT);
  
}

//========================= UPDATE =========================//
void loop(){
  if (TEST){ 
    test(); 
  }
  if (LUCHA){
    fiveSeconds();
    motores(-200,200);
    delay(200);
    lucha();
  }
}
 
void fiveSeconds(){
    for(int i=0; i<5; i++){
        digitalWrite(ledR, HIGH);
        digitalWrite(ledB, HIGH); 
        delay(500);
        digitalWrite(ledR, LOW);
        digitalWrite(ledB, LOW); 
        delay(500);    
    } 
}

void lucha() {
  do {
    reading();
    detectLine();
    attack();
    
  }while (true);
  
}

int eyes(){
  int valor = 0; // ambos sensores detectan
  if (vsensorR < refEyes[0]) { valor = valor - 1; } // solo detecta el izquierdo
  if (vsensorL < refEyes[1]) { valor = valor + 1; } // solo detecta el derecho
  else { valor = 2; } // ninguno detecta

  return valor;
}

void attack(){
  int detect = eyes();

  if (detect == 0){
    motores(255,255);
    onLedR();
  }
  else if (detect == 1){
    motores(255, 100);
    onLedR();
  }
  else if (detect == -1){
    motores(100, 255);
    onLedR();
  }
}


// ========= WALKINGS ===========
void search() { 
  unsigned long currentMillis = millis();
  switch (step) {
    case 0:
      motores(-100, 100);  // Comienza el primer movimiento
      previousMillis = currentMillis;
      step++;
      break;
    case 1:
      if (currentMillis - previousMillis >= 400) {
        motores(100, -100);  // Cambia al segundo movimiento después de 100 ms
        previousMillis = currentMillis;
        step++;
      }
      break;
    case 2:
      if (currentMillis - previousMillis >= 400) {
        onLedG();  // Enciende el LED verde después del segundo movimiento
        step = 0;  // Reinicia el proceso o ajusta según lo que necesites
      }
      break;
  }
}




// ========== LINEAS ==============
int foot(){
  reading();
  int valor = 0;
  if (valorR > refLine[0]){ valor = valor + 1; }
  if (valorL > refLine[1]){ valor = valor - 1; }
  if (valorB > refLine[2]){ valor = 2; }
  // no detecta linea
  if (valorR < refLine[0] && valorL < refLine[1] && valorB < refLine[2]){
    valor = 3;
  }
  return valor;
}

void detectLine(){
  int detect = foot();

  if (detect == 1){
    motores(-255,-255);
    onLedB();
    delay(150);
    motores(-255,255);    
    delay(100);
  }
  if (detect == -1){
    motores(-255,-255);
    onLedB();
    delay(150);
    motores(-255,255);    
    delay(100);
  }
  if (detect == 0){
    motores(-255,-255);
    onLedB();
    delay(150);
    motores(-255,255);    
    delay(100);
  }
  if (detect == 2){
    motores(100,100);
    onLedB();
    delay(150);
  }
  else {
    search();
  }

}

//FUNCION DE ENVIO DE VALORES A LOS MOTORES CON DRIVER 
void motores(int izq, int der)
{
  if(izq>=0)
  {
    digitalWrite(motorL1,HIGH);
    digitalWrite(motorL2,LOW);
    analogWrite(pwmL,izq);
  }
  if(izq<0)
  {
    digitalWrite(motorL1,LOW);
    digitalWrite(motorL2,HIGH);
    izq=izq*-1;
    analogWrite(pwmL,izq);
  }

  if(der>=0)
  {
    digitalWrite(motorR1,HIGH);
    digitalWrite(motorR2,LOW);
    analogWrite(pwmR,der);
  }
  if(der<0)
  {
    digitalWrite(motorR1,LOW);
    digitalWrite(motorR2,HIGH);
    der=der*-1;
    analogWrite(pwmR,der);
  }
}

void controlRemoto(){
  
  if (IrReceiver.decode()) {          
    auto myRawdata = IrReceiver.decodedIRData.decodedRawData;
    Serial.println(myRawdata);  
    IrReceiver.resume();             
  }
}


// ============ LEER SENSORES ===================
void reading(){ 
  vsensorR = analogRead(sensorR);
  vsensorL = analogRead(sensorL);
  valorR = analogRead(lineaR);
  valorL = analogRead(lineaL);
  valorB = analogRead(lineaB);
}


// ================== LEDS =====================
void onLedR(){
  digitalWrite(ledR, HIGH);
  digitalWrite(ledB, LOW);
  digitalWrite(ledG, LOW);
}

void onLedB(){
  digitalWrite(ledB, HIGH);
  digitalWrite(ledR, LOW);
  digitalWrite(ledG, LOW);
}

void onLedG(){
  digitalWrite(ledG, HIGH);
  digitalWrite(ledR, LOW);
  digitalWrite(ledB, LOW);
}



// =============== TEST SENSORES =============== 

void test() {

  //motores(255,-255); //Left/Right

  controlRemoto();
  /*
  reading();
  Serial.print(" SensorR: ");
  Serial.print(vsensorR);
  Serial.print(" SensorL: ");
  Serial.print(vsensorL);
  Serial.print(" lineaR: ");
  Serial.print(valorR);
  Serial.print(" lineaL: ");
  Serial.print(valorL);
  Serial.print(" lineaB: ");
  Serial.println(valorB);

  
  //Prueba LEDs
  digitalWrite(ledG, LOW);
  delay(200);
  digitalWrite(ledG, HIGH);
  digitalWrite(ledB, LOW);
  delay(200);
  digitalWrite(ledB, HIGH);
  digitalWrite(ledR, LOW);
  delay(200);
  digitalWrite(ledR, HIGH);
  */
}

void error(){
  Serial.println("Error desconocido..");
  digitalWrite(ledTest, HIGH);
  //reset();
}

/*
priorizar el sensor linea, si detecta linea retrocer y girar 180º
Activar remotamente, el robot espara 5s, con secuencia de
pitidos y ledR, luego comienza la secuencia lucha, ledV y ledB.
escanear y perseguir al contrincante (seguir al contrincante
 si no se detecta una linea)
La condicion va a ser que el valor de los sigue lineas sea "negro"
escanear, mover, repetir secuencia.
sino se cumple la condicion de estar dentro del color negro porque llegamos al limite
entonces ejecutar la funcion, "daleAtras();""
*/