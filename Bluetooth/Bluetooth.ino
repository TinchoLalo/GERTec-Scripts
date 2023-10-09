// Incluir librería para comunicación serie con módulo Bluetooth
#include <SoftwareSerial.h>
 
// Crear objeto de la clase SoftwareSerial
// El primer parámetro es el pin donde está conectado el TX del HC-05
// El segundo parámetro es el pin donde está conectado el RX del HC-05
SoftwareSerial moduloBluetooth(12, 11);
 
void setup()
{
  // Iniciamos comunicación serie
  // Por defecto utiliza la velocidad de 38400 baudios
  moduloBluetooth.begin(9600);
 
  // Iniciar comunicación serie para mostrar información
  // por el monitor serie
  Serial.begin(9600);
}
 
void loop()
{
  // Recibir
  // Comprobación de si hay algún dato 
  // en el puerto del módulo Bluetooth
  if (moduloBluetooth.available())
  {
    // Mostrar la información recibida en el monitor serie.
    Serial.write(moduloBluetooth.read());
  }
 
  // Enviar
  // Comprobar si hay algún dato en el monitor serie 
  // para ser enviado al módulo Bluetooth
  if (Serial.available())
  {
    // Enviar la información por Bluetooth
    moduloBluetooth.write(Serial.read());
  }
}