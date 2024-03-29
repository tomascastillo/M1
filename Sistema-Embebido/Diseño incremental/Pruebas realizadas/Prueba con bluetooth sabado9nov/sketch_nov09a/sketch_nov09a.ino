// Incluímos la librería para poder controlar el servo
#include <Servo.h>
#include <HX711.h>
#include <Wire.h>
#include <SoftwareSerial.h>

#define DOUT A1
#define CLK A0
/*
Si uso el millis() para el servo no funciona, es tan rapido que nunca llega a la posicion final del servo, nunca llega a escribir el 135 y de nuevo el 0,
necesitamos un delay porque ese paso es secuencial. ademas adentro del if tenemos una bandera
lo mismo con el buzzer, activo el buzzer en HIGH y tengo que esperar un segundo si o si para ponerlo en low
*/
HX711 balanza(DOUT, CLK);
// Declaramos la variable para controlar el servo
Servo servoMotor;
SoftwareSerial mySerial(10, 9);
int bandera=0;
int contador=0;
int tiempo_inicial = 0;
int tiempo_final = 0;
int tiempo_inicial2 = 0;
int tiempo_final2 = 0;
const int LEDPin = 12;
const int PIRPin = 7;
int banderaBluetooth = 0;
const int BUZZERpin = 8;
const int Trigger = 4; //Pin digital 2 para el Trigger del sensor
const int Echo = 2;    //Pin digital 3 para el Echo del sensor
int banderaLecturaInicial = 0;
int lecturaInicial = 0;
int operacion_alimentar = 0;
int operacion_buzzer=99;
int operacion_ultrasound=99;
int operacion_general = 98;
int banderaYaSonoBuzzer = 0;
char cadena_recibida_bluetooth='9';
int cantidad_de_aperturas_servo=0;
int bandera_servo=0;
int bandera_bluetooth=0;
/*
1. servo
2. balanza
3. pir
4. buzzer
5. todo el tiempo US+LED
*/

void setup()
{
  Serial.begin(9600);
  mySerial.begin(9600);
  //Serial.print("Lectura del valor del ADC:  ");
  //Serial.println(balanza.read());
  Serial.println("No ponga ningun objeto sobre la balanza");
  Serial.println("Destarando...");
  balanza.set_scale(); //La escala por defecto es 1
  balanza.tare(20);    //El peso actual es considerado Tara.
  Serial.println("Coloque un peso conocido:");
  // Iniciamos el servo para que empiece a trabajar con el pin 9
  servoMotor.attach(6);
  servoMotor.write(3);
  pinMode(LEDPin, OUTPUT);
  pinMode(PIRPin, INPUT);
  pinMode(BUZZERpin, OUTPUT); //definir pin como salida
  pinMode(Trigger, OUTPUT);   //pin como salida
  pinMode(Echo, INPUT);       //pin como entrada
  digitalWrite(Trigger, LOW); //Inicializamos el pin con 0
  tiempo_inicial = millis();
  digitalWrite(BUZZERpin, LOW);
}

void loop()
{
      //a 1 vez, b 2 veces, c 3 veces

    
      switch (operacion_general)
      {
      case 0:
        Serial.println("case 98");
        leer_bluetooth();
        break;
        
      case 1:
        abrir_servo_1_vez();
        break;

            
      case 2:
        cerrar_servo_1_vez();
        break;

      case 3:
        abrir_servo_2_veces();
        break;

            
      case 4:
        cerrar_servo_2_veces();
        break;

      case 5:
        abrir_servo_3_veces();
        break;

            
      case 6:
        cerrar_servo_3_veces();
        break;
                
        case 7:
        medir_peso_inicial();
        break;
        
      case 8:
        detectar_presencia_y_alertar();
        break;
    
      case 9:
        medir_cantidad_en_deposito_y_alertar();
        //reiniciar bandera para que se pueda volver a leer 
        banderaBluetooth =1;
        break;
    
      default:
        break;
      }


}
void abrir_servo_1_vez()
{
  
}
void leer_bluetooth()
{
  Serial.println("leer bluetooth");
  if (mySerial.available()>0&&bandera_bluetooth==0)
  {
    Serial.println("info disponible");
    cadena_recibida_bluetooth=mySerial.read();
    Serial.println(cadena_recibida_bluetooth);
    if(cadena_recibida_bluetooth=='a')
    {
      cantidad_de_aperturas_servo=1;  
      
    }
    
    if(cadena_recibida_bluetooth=='b')
    {
      
      cantidad_de_aperturas_servo=2;  

    }
    
    if(cadena_recibida_bluetooth=='c')
    {
      cantidad_de_aperturas_servo=3;  

    }
    bandera_bluetooth=1;
    operacion_general=0; //Cambiarlo******************

  }
  if(bandera_bluetooth==1)
  {
    operacion_general=0;
  }


}
void medir_cantidad_en_deposito_y_alertar()
{
  Serial.println("entrando a operacion general 3");
  operacion_ultrasound=0;
  //US
  long t; //timepo que demora en llegar el eco
  long d; //distancia en centimetros

  digitalWrite(Trigger, HIGH);
  int tiempo_micros_inicial;
  int tiempo_micros_final;
  delayMicroseconds(10); //Enviamos un pulso de 10us
  digitalWrite(Trigger, LOW);
    t = pulseIn(Echo, HIGH); //obtenemos el ancho del pulso
    d = t / 59;              //escalamos el tiempo a una distancia en cm

    Serial.print("Distancia ultrasonido: ");
    Serial.print(d); //Enviamos serialmente el valor de la distancia
    Serial.print("cm");
    Serial.println();
 /* tiempo_micros_inicial = micros();
  tiempo_micros_final = micros() - tiempo_micros_inicial;
  if (tiempo_micros_final > 10)
  {
    tiempo_micros_inicial = micros();
    digitalWrite(Trigger, LOW);
    t = pulseIn(Echo, HIGH); //obtenemos el ancho del pulso
    d = t / 59;              //escalamos el tiempo a una distancia en cm

    Serial.print("Distancia ultrasonido: ");
    Serial.print(d); //Enviamos serialmente el valor de la distancia
    Serial.print("cm");
    Serial.println();
  }
  */
  if (d > 17)
  {
    Serial.println("entrando al if dist>17");
    switch (operacion_ultrasound)
    {
    case 0:
      prender_led();
      break;

    case 1:
      apagar_led();
      break;

    default:
      break;
    }
  }
  else
  {
        Serial.println("entrando al else dist>17");
    digitalWrite(LEDPin, LOW);
  }

  // delay(100); //Hacemos una pausa de 100ms
  //dist>17 -> prendo LED
  //tiempo_final = millis() - tiempo_inicial;
  //if (tiempo_final > 5000) //5 segundos para sacar la comida
  //{
    //tiempo_inicial = millis();
    operacion_general = 98;
    operacion_alimentar=0;
  //}
}
void alimentar()
{
    switch (operacion_alimentar)
    {
    case 0:
    Serial.println("entrando en abrir servo");
      //cerrar_servo();
      abrir_servo();

      break;

    case 1:
    Serial.println("entrando en case 1 de operacion alimentar");
      cerrar_servo2();

      break;

    case 2:
      
      break;

    default:
      break;
    }
    
    
  

}
void medir_peso_inicial()
{
  if(contador<=cadena_recibida_bluetooth-'0')//para transformar al valor entero
  {
  contador++;
  Serial.print("Valor de lectura inicial:  ");
  lecturaInicial = balanza.get_value(10);
  Serial.println(lecturaInicial);
  banderaLecturaInicial = 1;
  Serial.println("delay para sacar la comida, 5 segundos...");
  
  }
  tiempo_final = millis() - tiempo_inicial;
  if (tiempo_final > 5000) //5 segundos para sacar la comida
  {
    tiempo_inicial = millis();
    operacion_general = 2;
    operacion_buzzer=0;
  }
  
  
}
void detectar_presencia_y_alertar()
{
  int value = digitalRead(PIRPin);
  Serial.println("lectura del PIR:");
  Serial.println(value);
  if (value == HIGH)
  {
    //leo de la balanza
    int lecturaComiendo = balanza.get_value(10);
    Serial.println("lectura balanza INICIAL: ");
    Serial.println(lecturaInicial);
    Serial.println("lectura balanza actual: ");
    Serial.println(balanza.get_value(10), 0);
    if (lecturaComiendo == lecturaInicial)
    {
    }                                                                                                                                  //no esta comiendo nada
    if (lecturaComiendo < lecturaInicial - 4000 /*&& lecturaComiendo != 0 && banderaLecturaInicial == 1 && banderaYaSonoBuzzer == 0*/) //esta comiendo muy rapido
    {
      //activar buzzer
      switch (operacion_buzzer)
      {
      case 0:
        Serial.print("case 0");
        prender_buzzer();
        break;

      case 1:
        Serial.print("case 1");
        apagar_buzzer();
        banderaYaSonoBuzzer = 1;
        break;

      default:
        break;
      }
    }
  }
  else
  //en este punto, el perro se fue, podemos ver el peso final.
  {
    int lecturaFinal = balanza.get_value(10);
  }
  Serial.println("pasando a operacion general 3");
  operacion_general = 3;
}
void prender_led()
{
  digitalWrite(LEDPin, HIGH); // poner el Pin en HIGH
  tiempo_final = millis() - tiempo_inicial;
  if (tiempo_final > 50)
  {
    tiempo_inicial = millis();
    operacion_ultrasound = 1;
  }
}
void apagar_led()
{
  digitalWrite(LEDPin, LOW); // poner el Pin en HIGH
  tiempo_final = millis() - tiempo_inicial;
  if (tiempo_final > 50)
  {
    tiempo_inicial = millis();
  }
}
void prender_buzzer()
{
  digitalWrite(BUZZERpin, HIGH); // poner el Pin en HIGH
  tiempo_final = millis() - tiempo_inicial;
  if (tiempo_final > 1000)
  {
    tiempo_inicial = millis();
    operacion_buzzer = 1;
  }
}
void apagar_buzzer()
{
  digitalWrite(BUZZERpin, LOW); // poner el Pin en HIGH
  tiempo_final = millis() - tiempo_inicial;
  if (tiempo_final > 1000)
  {
    tiempo_inicial = millis();
  }
}
void abrir_servo()
{

  /*if(bandera_servo==0)
  { 
    servoMotor.write(135);  
    bandera_servo=1;
  }*/
  servoMotor.write(135);  
    
  //delay(3000);
  tiempo_final = millis() - tiempo_inicial;
  if (tiempo_final > 3000)
  {
    Serial.println("abrir servo");
    tiempo_inicial = millis();
    operacion_alimentar = 1;
    operacion_general=0;
    bandera_servo=0;
    Serial.println("operacion_alimentar");
    Serial.println(operacion_alimentar);
    
  }
}
/*void cerrar_servo()
{
  servoMotor.write(0);
  tiempo_final = millis() - tiempo_inicial;
  if (tiempo_final > 1000)
  {
    Serial.println("cerrar servo");
    tiempo_inicial = millis();
    operacion_alimentar = 1;
  }
}*/
void cerrar_servo2()
{
  servoMotor.write(3);
  tiempo_final = millis() - tiempo_inicial;
  if (tiempo_final > 3000)
  {
    Serial.println("cerrar servo2");
    tiempo_inicial = millis();
    operacion_alimentar=99;
    operacion_general=1;
    
  }
}
