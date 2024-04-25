//estas son las librerias 
#include <IRremote.h>
#include <Servo.h>
#include <LedControl.h>
//se definen los pines de a los que se va a conectar la Matriz
#define NUM_DEVICES 1
#define DIN_PIN 10
#define CS_PIN 9
#define CLK_PIN 8

//se definene los pines donde se conectaran el receptor IR, los dos servos, la posicion inicial del servo
int IRpin = 3;
int servoPin1 = 5;
int servoPin2 = 6;
int servoPos = 0;
//las bariables servo se cambian de nombre a myServo1 y myServo2;
Servo myServo1;
Servo myServo2;

// Configuración de la matriz de LED
LedControl lc = LedControl(DIN_PIN, CLK_PIN, CS_PIN, NUM_DEVICES);

// Patrones de las caras
byte happyFace[8] = {
  B00111100,B01000010,B10100101,B10000001,B10100101,B10011001,B01000010,B00111100
};

byte neutralFace[8] = {
  B00111100,B01000010,B10100101,B10000001,B10111101,B10000001,B10100101,B01000010
};

byte sadFace[8] = {
  B00111100,B01000010,B10100101,B10000001,B10011001,B10100101,B10100101,B01111110
};

void setup() {
  //se decrara la cativacionde los servo motores y se mide la lactura de responsiba 9600, tambien se activa el sensor receptor IR
  myServo1.attach(servoPin1);
  myServo2.attach(servoPin2);
  Serial.begin(9600);
  IrReceiver.begin(IRpin);

  //los pines 5 y 6 se declaran como salidas 
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);

  // Inicializa la matriz de LED
  lc.shutdown(0, false); // Asegura que el dispositivo está encendido
  lc.setIntensity(0, 8); // Establece la intensidad del brillo (0~15)
  lc.clearDisplay(0); // Limpia la pantalla

  // Muestra la cara neutra al iniciar
  displayFace(neutralFace);
}

void loop() {
  //esta es la funcion de mantiene la cara neutra como la cara por defecto si no se manda ninguna señal 
  if (IrReceiver.decode() != 0) {
    int command = IrReceiver.decodedIRData.command;
    Serial.println(command);
    delay(1000);
    IrReceiver.resume();
    //aqui vamos a declarar la funcion 22 que es mandad por el control IR cuando se pulsa el nuemro 0
    if (command == 22) {
      servoPos = 150; // Si el comando es 22, establece la posición del servo en 150 grados
      myServo1.write(servoPos); // Mueve el servo a la posición deseada
      displayFace(happyFace); // Muestra la cara feliz en la matriz de LED
      delay(5000); // Espera 5 segundos
      servoPos = 0; // Devuelve el servo a la posición inicial (0 grados)
      myServo1.write(servoPos); // Mueve el servo a la posición deseada
      displayFace(neutralFace); // Muestra la cara neutra en la matriz de LED
      //es eta parte se creara la funcion trsite al pulsar el boton 1 del control IR con codigo 12
    } else if (command == 12) {
      displayFace(sadFace); // Muestra la cara triste en la matriz de LED
      delay(5000); // Espera 5 segundos
      displayFace(neutralFace); // Vuelve a mostrar la cara neutra en la matriz de LED
    } else {
      // Si no se recibe el comando 22 o 12, muestra la cara neutra en la matriz de LED
      displayFace(neutralFace);
    }
  }
}

// Función para mostrar un patrón de cara en la matriz de LED
void displayFace(byte face[8]) {
  for (int row = 0; row < 8; row++) {
    lc.setRow(0, row, face[row]);
  }
}
