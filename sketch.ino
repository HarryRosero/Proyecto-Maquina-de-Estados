/**
\file   sketch.ino
\date   2023-08-12
author Harry Rosero <harryrosero@unicauca.edu.co> - Daniel Muñoz <danielfmunoz@unicauca.edu.co>
@brief  Final Arquiitectura.
\par Copyright
Information contained here in is proprietary to and constitutes valuable
confidential trade secrets of Unicauca, and
is subject to restrictions on use and disclosure.
\par
Copyright (c) Unicauca 2022. All rights reserved.
\par
The copyright notices above do not evidence any actual or
intended publication of this material.
******************************************************************************
*/


#include "Pines.h"
#include "StateMachineLib.h"
#include <LiquidCrystal.h>
#include <Keypad.h>
#include <Wire.h>
#include "AsyncTaskLib.h"

/********************************************//**
 *  Inicialización del objeto LCD
 ***********************************************/
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

/********************************************//**
 *  Definición de la disposición del teclado
 ***********************************************/
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

/********************************************//**
 *  Inicialización del objeto Keypad
 ***********************************************/
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);


/********************************************//**
 * Contraseña de seguridad
 ***********************************************/
const String password = "6789";
int attempts = 0;

/********************************************//**
 *  Definición de variables para la reproducción de melodía
 ***********************************************/
int buzzer = 7;
int tempo = 140;
int wholenote = (60000 * 4) / tempo;

int divider = 0, noteDuration = 0;
int melodia[] = {

   // Dart Vader theme (Imperial March) - Star wars 
  // Score available at https://musescore.com/user/202909/scores/1141521
  // The tenor saxophone part was used

  NOTE_AS4,8, NOTE_AS4,8, NOTE_AS4,8,//1
  NOTE_F5,2, NOTE_C6,2,
  NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F6,2, NOTE_C6,4,  
  NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F6,2
};
int notes = sizeof(melodia) / sizeof(melodia[0]) / 2;

/********************************************//**
 * Prototipos de funciones
 ***********************************************/
String Escribir(); 

void Seguridad();
AsyncTask asyncTask_Seguridad(0, Seguridad);

void MonitorLu();
AsyncTask asyncTask2MonitorLu(0, true, MonitorLu);

void Alerta_led();
AsyncTask asyncTask3Alerta(0, true, Alerta_led);

void Temperatura();
AsyncTask asyncTask4Temperatura(500, true, Temperatura);

void AlarmaTem();
AsyncTask asyncTask4Alarma(0, true, AlarmaTem);

void Timeout_4sg();
AsyncTask asyncTaskTime1(4000, Timeout_4sg);
void Timeout_3sg();
AsyncTask asyncTaskTime2(3000, Timeout_3sg);
void Timeout_6sg();
AsyncTask asyncTaskTime3(6000, Timeout_6sg);
void Timeout_5sg();
AsyncTask asyncTaskTime4(5000, Timeout_5sg);

/********************************************//**
 * Enumeraciones para los estados y entradas de la máquina de estado
 ***********************************************/
enum State
{
  INIT = 0,
  BLOQUEO = 1,
  MONITOR_LUZ = 2,
  ALERTA = 3,
  MONITOR_TEMP = 4,
  ALARMA = 5
};

enum Input
{
  CORRECTO = 0,
  BLOQUEADO = 1,
  TIME_OUT = 2,
  LUZ = 3,
  TEMP = 4,
  Unknown = 5,
};

/********************************************//**
 * Inicialización de la máquina de estado
 ***********************************************/
StateMachine stateMachine(6, 9);
Input currentInput;

/********************************************//**
 * Configuración inicial del Arduino
 ***********************************************/
void setup()
{
  Serial.begin(9600);
  pinMode(pingreen, OUTPUT);
  pinMode(pinblue, OUTPUT);
  pinMode(pinred, OUTPUT);

  Serial.println("Starting State Machine...");
  setup_State_Machine();

  Serial.println("Start Machine Started");
  stateMachine.SetState(State::INIT, false, true);
  lcd.begin(16, 2);
}

void loop()
{
  //Actualizar tarea asincrónica
  asyncTask_Seguridad.Update();
  asyncTask2MonitorLu.Update();
  asyncTask3Alerta.Update();
  asyncTask4Temperatura.Update();
  asyncTask4Alarma.Update();
  
  asyncTaskTime1.Update();
  asyncTaskTime2.Update();
  asyncTaskTime3.Update();
  asyncTaskTime4.Update();
  //Actualizar máquina de estado
  stateMachine.Update();
 
}

void Timeout_4sg()
{
  currentInput = Input::TIME_OUT;
}
void Timeout_3sg()
{
  currentInput = Input::TIME_OUT; 
 
}
void Timeout_6sg()
{
  currentInput = Input::TIME_OUT;  
}
void Timeout_5sg()
{
  currentInput = Input::TIME_OUT; 
}
/********************************************//**
 * Función para ingresar la contraseña
 ***********************************************/
String Escribir() 
{
  lcd.print("Contrasenia:");
  lcd.setCursor(0, 1);
  String result = "";
  char key;
  while (true) {
    key = keypad.getKey();
    if (key >= '0' && key <= '9') {
      lcd.print("*");
      result += key;
  // Salir del bucle cuando se presiona '#'
    } else if (key == '#') {
      break;
    }
  }
  lcd.clear();
  return result;

}
/********************************************//**
 * Procedimiento de seguridad para ingresar la contraseña
 ***********************************************/
void Seguridad() 
{
  const int maxAttempts = 3;

  for (int intentos = 0; intentos < maxAttempts; ++intentos) {

    String enteredPassword = Escribir();

    if (enteredPassword.equals(password)) {      
      lcd.print("contrasenia");
      lcd.setCursor(0,1);
      lcd.print("correcta");
      digitalWrite(pingreen, HIGH);
      delay(1000);
      digitalWrite(pingreen, LOW);
      currentInput = Input::CORRECTO;
      return; 
      // Sale de la función si la contraseña es correcta
    } else {
      lcd.print("Contrasenia");
      lcd.setCursor(0,1);
      lcd.print("incorrecta");
      digitalWrite(pinblue, HIGH);
      delay(1000);
      digitalWrite(pinblue, LOW);
      delay(1000);
      lcd.clear();
    }
  }
    //No tiene mas intentos para ingresar la contraseña 
    lcd.print("Bloqueado");
    digitalWrite(pinred, HIGH);
    delay(1000);  
    digitalWrite(pinred, LOW);
    currentInput = Input::BLOQUEADO;
    lcd.print(currentInput);
    delay(1000);
    lcd.clear();

}
/********************************************//**
 *  Procedimiento para monitorear la luz
 ***********************************************/
void MonitorLu()
{
  lcd.clear();
  lcd.setCursor(0,0);
  int outputValue = analogRead(PinLu);
  lcd.print("Luz: ");
  lcd.setCursor(0, 1);
  lcd.print(outputValue);
  Serial.println(outputValue);
  delay(1000);
  
  if (outputValue < 40)
  {
    currentInput = Input::LUZ;
  }
}
/********************************************//**
 *  Procedimiento para monitorear la luz
 ***********************************************/
void Alerta_led()
{
  lcd.print("Alerta   ");
  digitalWrite(pinred, HIGH);
  delay(500);
  digitalWrite(pinred, LOW);
  delay(200);
}
/********************************************//**
 *  Procedimiento para medir la temperatura
 ***********************************************/
void Temperatura(){
  lcd.clear();
  lcd.setCursor(0,0);
  int analogValue = analogRead(DHTPIN);
  float celsius = 1 / (log(1 / (1023. / analogValue - 1)) / BETA + 1.0 / 298.15) - 273.15;
  Serial.print(" Temperatura:");
  Serial.print(celsius);
  Serial.print("°C");
  lcd.print(" Temperatura:");
  lcd.setCursor(0, 1);
  lcd.print(celsius);
  delay(1000);

  if(celsius > 30)
  {
    currentInput = Input::TEMP;
  }
  Serial.println();
}

void AlarmaTem(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Alarma");
  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {

    // calcula la duración de cada nota
    divider = melodia[thisNote + 1];

    if (divider > 0) {
      noteDuration = (wholenote) / divider;

    } else if (divider < 0) {
      //aumenta la duración a la mitad para las notas punteadas
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5;
    }

    tone(buzzer, melodia[thisNote], noteDuration * 0.9);
    delay(noteDuration);
    noTone(buzzer);
  }
}
