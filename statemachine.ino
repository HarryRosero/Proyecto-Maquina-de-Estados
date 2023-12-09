/**
 * @file main.cpp
 \file statemachine.ino
 * @brief Programa principal para un sistema de seguridad basado en Arduino.
 */

 /**
 * @brief Inicializa la máquina de estados y sus transiciones.
 */
void setup_State_Machine()
{
  // Configurar transiciones
  stateMachine.AddTransition(INIT, BLOQUEO, []() { return currentInput == BLOQUEADO; });
  stateMachine.AddTransition(INIT, MONITOR_LUZ, []() { return currentInput == CORRECTO; });
  stateMachine.AddTransition(BLOQUEO, INIT, []() { return currentInput == TIME_OUT; });

  stateMachine.AddTransition(MONITOR_LUZ, ALERTA, []() { return currentInput == LUZ; });
  stateMachine.AddTransition(ALERTA, MONITOR_LUZ, []() { return currentInput == TIME_OUT; });
  stateMachine.AddTransition(MONITOR_LUZ, MONITOR_TEMP, []() { return currentInput == TIME_OUT; });
  stateMachine.AddTransition(MONITOR_TEMP, MONITOR_LUZ, []() { return currentInput == TIME_OUT; });

  stateMachine.AddTransition(MONITOR_TEMP, ALARMA, []() { return currentInput == TEMP; });
  stateMachine.AddTransition(ALARMA, MONITOR_TEMP, []() { return currentInput == TIME_OUT; });

  //configurar eventos de estado
  //para que se ejecuten al entrar
  stateMachine.SetOnEntering(INIT, inputInit);
	stateMachine.SetOnEntering(BLOQUEO, inputBlo);
  stateMachine.SetOnEntering(MONITOR_LUZ, inputLuz);
	stateMachine.SetOnEntering(ALERTA, inputALer);
  stateMachine.SetOnEntering(MONITOR_TEMP, inputTem);
  stateMachine.SetOnEntering(ALARMA, inputAlar);
  //para que se ejecuten al salir
  stateMachine.SetOnLeaving(INIT, outputInit);
  stateMachine.SetOnLeaving(BLOQUEO, outputBlo);
  stateMachine.SetOnLeaving(MONITOR_LUZ, outputLuz);
	stateMachine.SetOnLeaving(ALERTA, outputAler);
  stateMachine.SetOnLeaving(MONITOR_TEMP, outputTem);
  stateMachine.SetOnLeaving(ALARMA, outputAlar);
}

/**
 * @brief Función de entrada para el estado INIT de la máquina de estados.
 */
void inputInit()
{
  currentInput = Input::Unknown;
  Serial.println("Estado INIT");
  Serial.println("A   B   C   D   E   F");
  Serial.println("    X                ");
  Serial.println();
  lcd.clear();
  lcd.setCursor(0, 0);
  asyncTask_Seguridad.Start();
}

/**
 * @brief Función de salida para el estado INIT de la máquina de estados.
 */
void outputInit()
{
  asyncTask_Seguridad.Stop();
}

/**
 * @brief Función de entrada para el estado BLOQUEO de la máquina de estados.
 */
void inputBlo()
{ 
  currentInput = Input::Unknown;
  Serial.println("Estado BLOQUEADO");
  Serial.println("A   B   C   D   E   F");
  Serial.println("X                    ");
  Serial.println();
  lcd.clear();
  lcd.setCursor(0, 0);
  asyncTaskTime4.Start();
}

/**
 * @brief Función de salida para el estado BLOQUEO de la máquina de estados.
 */
void outputBlo()
{  
  asyncTaskTime4.Stop();
}

/**
 * @brief Función de entrada para el estado MONITOR_LUZ de la máquina de estados.
 */
void inputLuz()
{
  Serial.println();
  currentInput = Input::Unknown;
  Serial.println(" Estado Luz");
  Serial.println("A   B   C   D   E   F");
  Serial.println("        X            ");
  Serial.println();
  lcd.clear();
  lcd.setCursor(0, 0);
  asyncTask2MonitorLu.Start();
  asyncTaskTime1.Start();  
}

/**
 * @brief Función de salida para el estado MONITOR_LUZ de la máquina de estados.
 */
void outputLuz()
{

  asyncTask2MonitorLu.Stop();
  asyncTaskTime1.Stop();
}

/**
 * @brief Función de entrada para el estado ALERTA de la máquina de estados.
 */
void inputALer()
{
  currentInput = Input::Unknown;
  Serial.println("Estado Alerta Luz");
  Serial.println("A   B   C   D   E   F");
  Serial.println("            X        ");
  Serial.println();
  lcd.clear();
  lcd.setCursor(0, 0); 
  asyncTask3Alerta.Start();
  asyncTaskTime1.Start();
}

/**
 * @brief Función de salida para el estado ALERTA de la máquina de estados.
 */
void outputAler()
{
  asyncTask3Alerta.Stop();
  asyncTaskTime1.Stop();
}

/**
 * @brief Función de entrada para el estado MONITOR_TEMP de la máquina de estados.
 */
void inputTem()
{
  currentInput = Input::Unknown;
  Serial.println("Estado Temperatura");
  Serial.println("A   B   C   D   E   F");
  Serial.println("                X    ");
  Serial.println();
  lcd.clear();
  lcd.setCursor(0, 0); 
  asyncTask4Temperatura.Start();
  asyncTaskTime3.Start();
}

/**
 * @brief Función de salida para el estado MONITOR_TEMP de la máquina de estados.
 */
void outputTem()
{
  asyncTask4Temperatura.Stop();
  asyncTaskTime3.Stop();
}

/**
 * @brief Función de entrada para el estado ALARMA de la máquina de estados.
 */
void inputAlar()
{
  currentInput = Input::Unknown;
  Serial.println(" Estado Alarma");
  Serial.println("A   B   C   D   E   F");
  Serial.println("                    X");
  Serial.println();
  lcd.clear();
  lcd.setCursor(0, 0); 
  asyncTask4Alarma.Start();
  asyncTaskTime4.Start();
}

/**
 * @brief Función de salida para el estado ALARMA de la máquina de estados.
 */
void outputAlar()
{
  asyncTask4Alarma.Stop();
  asyncTaskTime4.Stop();
}
