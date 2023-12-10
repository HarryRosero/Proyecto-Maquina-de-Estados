/**
 * @file main.cpp
 \file statemachine.ino
 * @brief Programa principal para un sistema de seguridad basado en Arduino.
 */

 /**
 * @brief Inicializa la máquina de estados y sus transiciones.
 */
void setup_M()
{
  // Configurar transiciones
  stateMachine.AddTransition(Init, BLOQUEO, []() { return Inputc == BLOQUEADO; });
  stateMachine.AddTransition(Init, MonitorLuz, []() { return Inputc == CORRECTO; });
  stateMachine.AddTransition(BLOQUEO, Init, []() { return Inputc == Timeout; });

  stateMachine.AddTransition(MonitorLuz, ALERTA, []() { return Inputc == LUZ; });
  stateMachine.AddTransition(ALERTA, MonitorLuz, []() { return Inputc == Timeout; });
  stateMachine.AddTransition(MonitorLuz, TemperaturaM, []() { return Inputc == Timeout; });
  stateMachine.AddTransition(TemperaturaM, MonitorLuz, []() { return Inputc == Timeout; });

  stateMachine.AddTransition(TemperaturaM, ALARMA, []() { return Inputc == Temperatur; });
  stateMachine.AddTransition(ALARMA, TemperaturaM, []() { return Inputc == Timeout; });

  //configurar eventos de estado
  //para que se ejecuten al entrar
  stateMachine.SetOnEntering(Init, inputInit);
	stateMachine.SetOnEntering(BLOQUEO, inputBloq);
  stateMachine.SetOnEntering(MonitorLuz, inputLuz);
	stateMachine.SetOnEntering(ALERTA, inputALer);
  stateMachine.SetOnEntering(TemperaturaM, inputTem);
  stateMachine.SetOnEntering(ALARMA, inputAlar);
  //para que se ejecuten al salir
  stateMachine.SetOnLeaving(Init, outputInit);
  stateMachine.SetOnLeaving(BLOQUEO, outputBloq);
  stateMachine.SetOnLeaving(MonitorLuz, outputLuz);
	stateMachine.SetOnLeaving(ALERTA, outputAler);
  stateMachine.SetOnLeaving(TemperaturaM, outputTem);
  stateMachine.SetOnLeaving(ALARMA, outputAlar);
}

/**
 * @brief Función de entrada para el estado INIT de la máquina de estados.
 */
void inputInit()
{
  Inputc = Input::NoInfo;
  Serial.println("Estado INIT");
  Serial.println("A   B   C   D   E   F");
  Serial.println("    X                ");
  Serial.println();
  lcd.clear();
  lcd.setCursor(0, 0);
  Tarea1.Start();
}

/**
 * @brief Función de salida para el estado INIT de la máquina de estados.
 */
void outputInit()
{
  Tarea1.Stop();
}

/**
 * @brief Función de entrada para el estado BLOQUEO de la máquina de estados.
 */
void inputBloq()
{ 
  Inputc = Input::NoInfo;
  Serial.println("Estado BLOQUEADO");
  Serial.println("A   B   C   D   E   F");
  Serial.println("X                    ");
  Serial.println();
  lcd.clear();
  lcd.setCursor(0, 0);
  Tiempo4.Start();
}

/**
 * @brief Función de salida para el estado BLOQUEO de la máquina de estados.
 */
void outputBloq()
{  
  Tiempo4.Stop();
}

/**
 * @brief Función de entrada para el estado MonitorLuz de la máquina de estados.
 */
void inputLuz()
{
  Serial.println();
  Inputc = Input::NoInfo;
  Serial.println(" Estado Luz");
  Serial.println("A   B   C   D   E   F");
  Serial.println("        X            ");
  Serial.println();
  lcd.clear();
  lcd.setCursor(0, 0);
  Tarea2.Start();
  Tiempo1.Start();  
}

/**
 * @brief Función de salida para el estado MonitorLuz de la máquina de estados.
 */
void outputLuz()
{

  Tarea2.Stop();
  Tiempo1.Stop();
}

/**
 * @brief Función de entrada para el estado ALERTA de la máquina de estados.
 */
void inputALer()
{
  Inputc = Input::NoInfo;
  Serial.println("Estado Alerta Luz");
  Serial.println("A   B   C   D   E   F");
  Serial.println("            X        ");
  Serial.println();
  lcd.clear();
  lcd.setCursor(0, 0); 
  Tarea3.Start();
  Tiempo1.Start();
}

/**
 * @brief Función de salida para el estado ALERTA de la máquina de estados.
 */
void outputAler()
{
  Tarea3.Stop();
  Tiempo1.Stop();
}

/**
 * @brief Función de entrada para el estado TemperaturaM de la máquina de estados.
 */
void inputTem()
{
  Inputc = Input::NoInfo;
  Serial.println("Estado Temperatura");
  Serial.println("A   B   C   D   E   F");
  Serial.println("                X    ");
  Serial.println();
  lcd.clear();
  lcd.setCursor(0, 0); 
  Tarea4.Start();
  Tiempo3.Start();
}

/**
 * @brief Función de salida para el estado TemperaturaM de la máquina de estados.
 */
void outputTem()
{
  Tarea4.Stop();
  Tiempo3.Stop();
}

/**
 * @brief Función de entrada para el estado ALARMA de la máquina de estados.
 */
void inputAlar()
{
  Inputc = Input::NoInfo;
  Serial.println(" Estado Alarma");
  Serial.println("A   B   C   D   E   F");
  Serial.println("                    X");
  Serial.println();
  lcd.clear();
  lcd.setCursor(0, 0); 
  Tarea5.Start();
  Tiempo4.Start();
}

/**
 * @brief Función de salida para el estado ALARMA de la máquina de estados.
 */
void outputAlar()
{
  Tarea5.Stop();
  Tiempo4.Stop();
}

