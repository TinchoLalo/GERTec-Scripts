// ========================= ROBOT FUTBOL ========================= 
/*  Se debe utilizar la aplicacion Car Bluetooth Controller (https://play.google.com/store/apps/details?id=braulio.calle.bluetoothRCcontroller&hl=es&gl=US)
    Para conectarse debe primero conectarse al robot por bluetooth, ingresar la contraseña 1234.
    Luego ya puede conectarse desde la aplicación y empezar a controlar el robot.
*/
// bibliotecas para la conexión por bluetooth
#include <SoftwareSerial.h> 
#include <Servo.h>
SoftwareSerial bluetooth(12,11); // RX, TX 11 / 12

// ======================== MOTORES ========================
int motorR1  = 6;  // Pin Motor Derecha Adelante
int motorR2  = 5;  // Pin Motor Derecha Atras
int motorL1  = 8;   // Pin Motor Izquierda Adelante 
int motorL2  = 9;   // Pin Motor Izquierda Atras


char dato;

// velocidad
int Speed = 0;
int ctrlSpeed = 0;

// FUNCION INICIAL
void setup() {
  // Configurar los pines de los motores como salidas
  pinMode(motorR1, OUTPUT);
  pinMode(motorR2, OUTPUT);
  pinMode(motorR1, OUTPUT);
  pinMode(motorR2, OUTPUT);

  // iniciamos los puertos
  Serial.begin(9600);  
  Serial.println("FUTBOL BOT INICIANDO"); //
  bluetooth.begin(115200);
  delay(100);
  bluetooth.begin(9600);
  
}

// FUNCION PRINCIPAL QUE SE REPITE
void loop() {
  // comprobamos el estado de bluetooth
  if (bluetooth.available()){ 
    char dato = bluetooth.read();
    Serial.println(dato);
    // llamamos a la función correspondiete de acuerdo al botón que presionemos
    if (dato == 'F'){ // comprobamos el valor del dato
      Adelante(); // si el valor es igual a 'F' vamos hacia adelante mientras el boton se precione
    }
    else if (dato == 'B') {
      Atras();
    }
    else if (dato == 'L') {
      Izquierda();
    }
    else if (dato == 'R') {
      Derecha();
    }

    // Combinaciones de teclas
    else if (dato == 'I') {
      AdelanteL();
    }
    else if (dato == 'G') {
      AdelanteR();
    }
    else if (dato == 'J') {
      AtrasL();
    }
    else if (dato == 'H') {
      AtrasR();
    }


    // Velocidad de los motores
    else if (dato == 'q'){
      Speed = 0;
      ctrlSpeed = 0;
    }
    else if (dato == '9'){
      Speed = 30;
      ctrlSpeed = 15;
    }
    else if (dato == '8'){
      Speed = 55;
      ctrlSpeed = 25;
    }
    else if (dato == '7'){
      Speed = 80;
      ctrlSpeed = 35;
    }
    else if (dato == '6'){
      Speed = 95;
      ctrlSpeed = 45;
    }
    else if (dato == '5'){
      Speed = 115;
      ctrlSpeed = 55;
    }
    else if (dato == '4'){
      Speed = 125;
      ctrlSpeed = 65;
    }
    else if (dato == '3'){
      Speed = 135;
      ctrlSpeed = 75;
    }
    else if (dato == '2'){
      Speed = 155;
      ctrlSpeed = 85;
    }
    else if (dato == '1'){
      Speed = 170;
      ctrlSpeed = 95;
    }
    else if (dato == '0'){
      Speed = 190;
      ctrlSpeed = 100;
    }
    else {
      Parar();
    }


    
  }
  
}

// ========= FUNCIONES MOVIMIENTOS =========
void Adelante(){
  //Serial.println("Adelante");
  analogWrite(motorR1, 255); // encendemos el motor derecho adelante
  analogWrite(motorR2, Speed); // apagamos el motor derecho atras
  analogWrite(motorL1, 255); // encendemos el motor izquierdo atras
  analogWrite(motorL2, Speed); // apagamos el motor izquierdo adelante
}

void Atras(){
  //Serial.println("Atras");
  analogWrite(motorR1, Speed);
  analogWrite(motorR2, 255);
  analogWrite(motorL1, Speed);
  analogWrite(motorL2, 255);
}

void Derecha(){
  //Serial.println("Izquierda");
  analogWrite(motorR1, Speed);
  analogWrite(motorR2, 255);
  analogWrite(motorL1, 255);
  analogWrite(motorL2, Speed);
}

void Izquierda(){
  //Serial.println("Derecha");
  analogWrite(motorR1, 255);
  analogWrite(motorR2, Speed);
  analogWrite(motorL1, Speed);
  analogWrite(motorL2, 255);
}

// Combinaciones de teclas

void AdelanteR(){
  //Serial.println("Adelante Derecha");
  analogWrite(motorR1, 255); // encendemos el motor derecho adelante
  analogWrite(motorR2, ctrlSpeed); // apagamos el motor derecho atras
  analogWrite(motorL1, 255); // encendemos el motor izquierdo atras lentamente 
  analogWrite(motorL2, ctrlSpeed); // apagamos el motor izquierdo atras
}
void AdelanteL(){
  //Serial.println("Adelante Izquierda");
  analogWrite(motorR1, 255); 
  analogWrite(motorR2, ctrlSpeed); 
  analogWrite(motorL1, 255); 
  analogWrite(motorL2, ctrlSpeed); 
}
void AtrasR(){
  //Serial.println("Atras Derecha");
  analogWrite(motorR1, ctrlSpeed); 
  analogWrite(motorR2, 255); 
  analogWrite(motorL1, ctrlSpeed); 
  analogWrite(motorL2, 255); 
}
void AtrasL(){
  //Serial.println("Atras Izquierda");
  analogWrite(motorR1, ctrlSpeed); 
  analogWrite(motorR2, 255); 
  analogWrite(motorL1, ctrlSpeed); 
  analogWrite(motorL2, 255); 
}

void Rotar(){
  for(int i=0;i<3;i++){
    Serial.println("Rotar");
    Derecha();
    delay(100);
  } 
}

void Parar (){
  digitalWrite(motorR2, LOW);
  digitalWrite(motorR1, LOW);
  digitalWrite(motorL2, LOW);
  digitalWrite(motorL1, LOW);
}