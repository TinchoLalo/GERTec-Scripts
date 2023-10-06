// ========================= ROBOT FUTBOL LI-10N ========================= 
/*  Se debe utilizar la aplicacion Car Bluetooth Controller (https://play.google.com/store/apps/details?id=braulio.calle.bluetoothRCcontroller&hl=es&gl=US)
    Para conectarse debe primero conectarse al robot por bluetooth, ingresar la contraseña 1234.
    Luego ya puede conectarse desde la aplicación y empezar a controlar el robot.
*/
// bibliotecas para la conexión por bluetooth
#include <SoftwareSerial.h> 
#include <Servo.h>
SoftwareSerial bluetooth(2,4); // RX, TX 

// biblioteca para el brújula


// ======================== MOTORES ========================
int motorA1  = 10;  
int motorA2  = 11;  
int motorB1  = 6;   
int motorB2  = 9;   
int motorC1  = 5;   
int motorC2  = 3;   

// ======================== LEDS ========================
int ledR  = 12;  // Pin Motor Derecha Adelante
int ledB  = 8;  
int ledG = 7;   
 
char dato;

int Speed = 0;
int ctrlSpeed = 0;

// brújula




void setup() {
  // Inicio
  Serial.begin(9600);  
  Serial.println("FUTBOL LI-10N BOT INICIANDO"); 
  // Bluetooth
  bluetooth.begin(115200);
  delay(100);
  bluetooth.begin(9600);

  // comprobando brújula
  

}

void loop() {
  // comprobamos el estado de bluetooth
  if (bluetooth.available()){ 
    char dato = bluetooth.read();

    // llamamos a la función correspondiete de acuerdo al botón que presionemos
    if (dato == 'F'){ // comprobamos el valor del dato
      AdelanteA(); // si el valor es igual a 'F' vamos hacia adelante mientras el boton se precione
    }
    else if (dato == 'B') {
      AtrasA();
    }
    else if (dato == 'R') {
      Right();
    }
    else if (dato == 'L') {
      Left();
    }

    // Combinaciones de teclas
    else if (dato == 'I') {
      Rotar1();
    }
    else if (dato == 'G') {
      Rotar2();
    }
    else if (dato == 'J') {
      Rotar1();
    }
    else if (dato == 'H') {
      Rotar2();
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

// ========= FUNCIONES GIROSCOPIO =========
void Adelante(){
  AdelanteA();
}

void Atras(){
  AtrasA();
}

// ========= FUNCIONES MOVIMIENTOS =========

void AdelanteA(){
  //Serial.println("Adelante");
  analogWrite(motorA1, Speed);
  analogWrite(motorA2, Speed); 
  analogWrite(motorB1, Speed); 
  analogWrite(motorB2, 255); 
  analogWrite(motorC1, 255); 
  analogWrite(motorC2, Speed); 
}
void AtrasA(){
  //Serial.println("Adelante");
  analogWrite(motorA1, Speed);
  analogWrite(motorA2, Speed); 
  analogWrite(motorB1, 255); 
  analogWrite(motorB2, Speed); 
  analogWrite(motorC1, Speed);
  analogWrite(motorC2, 255); 
}
void Right(){
  //Serial.println("Adelante");
  analogWrite(motorA1, 255);
  analogWrite(motorA2, Speed); 
  analogWrite(motorB1, 200); 
  analogWrite(motorB2, Speed); 
  analogWrite(motorC1, 200);
  analogWrite(motorC2, Speed); 
}
void Left(){
  //Serial.println("Adelante");
  analogWrite(motorA1, Speed);
  analogWrite(motorA2, 255); 
  analogWrite(motorB1, Speed); 
  analogWrite(motorB2, 200); 
  analogWrite(motorC1, Speed);
  analogWrite(motorC2, 200); 
}

void Rotar1(){
  analogWrite(motorA1, Speed); // encendemos el motor derecho adelante
  analogWrite(motorA2, 255); // apagamos el motor derecho atras
  analogWrite(motorB1, Speed); // encendemos el motor izquierdo atras
  analogWrite(motorB2, 255); // apagamos el motor izquierdo adelante
  analogWrite(motorC1, Speed); // encendemos el motor izquierdo atras
  analogWrite(motorC2, 255); // apagamos el motor izquierdo adelante
}
void Rotar2(){
  analogWrite(motorA1, 255); // encendemos el motor derecho adelante
  analogWrite(motorA2, Speed); // apagamos el motor derecho atras
  analogWrite(motorB1, 255); // encendemos el motor izquierdo atras
  analogWrite(motorB2, Speed); // apagamos el motor izquierdo adelante
  analogWrite(motorC1, 255); // encendemos el motor izquierdo atras
  analogWrite(motorC2, Speed); // apagamos el motor izquierdo adelante
}





void Parar (){
  analogWrite(motorA1, LOW); // encendemos el motor derecho adelante
  analogWrite(motorA2, LOW); // apagamos el motor derecho atras
  analogWrite(motorB1, LOW); // encendemos el motor izquierdo atras
  analogWrite(motorB2, LOW); // apagamos el motor izquierdo adelante
  analogWrite(motorC1, LOW); // encendemos el motor izquierdo atras
  analogWrite(motorC2, LOW); // apagamos el motor izquierdo adelante
}
