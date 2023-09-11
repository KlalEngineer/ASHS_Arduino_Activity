#define NTC_PIN A7  // Pin,to which the voltage divider is connected
#define POT A0
#define BUZZER 11
#define LED_RED 4
#define LED_YELLOW 3
#define LED_GREEN 2
#define ambTemp 22

void setup(void) {
  Serial.begin(115200);  //initialize serial communication at a baud rate of 115200
  pinMode(BUZZER, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
}

void loop(void) {
  float T = tempSens();
  Serial.print("Temperature ");
  Serial.print(T);
  Serial.println(" *C");

  if (T <= 22){
    clearLEDs();
  }
  if (T >= ambTemp+2){
    clearLEDs();
    digitalWrite(LED_GREEN, HIGH);
  }
  if (T >= ambTemp+3){
    clearLEDs();
    digitalWrite(LED_YELLOW, HIGH);
    digitalWrite(LED_GREEN, HIGH);
  }
  if (T >= ambTemp+4){
    clearLEDs();
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_YELLOW, HIGH);
    digitalWrite(LED_GREEN, HIGH);
  }

  buzzer();
}

float tempSens(){
  #define NTC_resistance 10000 // Nominal resistance of NTC at room temperature (25⁰C)
  #define nominal_temperature 25 + 273.15 // Temperature for nominal resistance (almost always 25⁰ C)
  #define NTC_beta 3950
  #define samplingRate 5  // Number of samples
  #define Rref 10000 // Value of resistor used for the voltage divider

  uint8_t i;
  float average;
  int samples = 0;

  // take voltage readings from the voltage divider
  for (i = 0; i < samplingRate; i++) {
    samples += analogRead(NTC_PIN);
    // delay(10);
  }
  average = 0;
  average = samples / samplingRate;
  average = 1024 / average - 1;
  average = Rref / average;
  //  temperature;
  float temperature = average/NTC_resistance;
  temperature = log(temperature);  // ln(R/Ro)
  temperature /= NTC_beta;  // 1/B * ln(R/Ro)
  temperature += 1.0 / (nominal_temperature);  // + (1/To), and convert to Kelvin
  temperature = 1.0 / temperature;  // Invert
  temperature -= 273.15;  // convert absolute temp to C

  return temperature;
}

void clearLEDs(){
  digitalWrite(LED_RED, LOW); digitalWrite(LED_YELLOW, LOW); digitalWrite(LED_GREEN, LOW);
}

void buzzer(){
  int val = analogRead(POT);
  val = map(val, 0, 1024, 0, 4500);
  tone(BUZZER, val, 10);
}

