#include <WiFiS3.h>
#include <Arduino_LED_Matrix.h>

// Datos del Wi-Fi
const char* SSID = "RedWifi";
const char* PASS = "Constraseña";

// LED externo
const int LED_EXTERNO = 4;

// Puente H
const int ENA = 9;
const int IN1 = 8;
const int IN2 = 7;

// Velocidades
const int PWM[4] = {0, 110, 180, 255};

const char* NOMBRE[4] = {
  "detenido",
  "velocidad baja",
  "velocidad media",
  "velocidad maxima"
};

int nivel = 0;

WiFiServer servidor(80);
ArduinoLEDMatrix matriz;

// Dibujo que aparecerá en la matriz
uint8_t dibujo[8][12] = {
  {1,1,1,1,1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1,1,1,1,1}
};

void aplicarVelocidad() {
  if (nivel == 0) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, 0);
  } else {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, PWM[nivel]);
  }

  Serial.print("Nivel: ");
  Serial.print(nivel);
  Serial.print(" | PWM: ");
  Serial.println(PWM[nivel]);
}

void setup() {
  Serial.begin(9600);

  pinMode(LED_EXTERNO, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  digitalWrite(LED_EXTERNO, LOW);
  aplicarVelocidad();

  // Iniciar y apagar la matriz
  matriz.begin();
  matriz.clear();

  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("No se encontro el modulo Wi-Fi.");

    while (true) {
    }
  }

  Serial.print("Conectando al Wi-Fi");

  WiFi.begin(SSID, PASS);

  unsigned long inicio = millis();

  while (WiFi.status() != WL_CONNECTED &&
         millis() - inicio < 30000) {
    Serial.print(".");
    delay(500);
  }

  Serial.println();

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("No se pudo conectar.");

    while (true) {
      delay(1000);
    }
  }

  Serial.println("Arduino conectado.");
  Serial.print("IP del Arduino: http://");
  Serial.println(WiFi.localIP());

  servidor.begin();
  Serial.println("Servidor iniciado.");
}

void loop() {
  WiFiClient cliente = servidor.available();

  if (!cliente) {
    return;
  }

  cliente.setTimeout(1000);

  String peticion = cliente.readStringUntil('\r');

  Serial.print("Peticion recibida: ");
  Serial.println(peticion);

  String respuesta = "Orden no reconocida";

  // Control del LED externo
  if (peticion.indexOf("/led/on") >= 0) {
    digitalWrite(LED_EXTERNO, HIGH);
    respuesta = "LED encendido";
  }

  if (peticion.indexOf("/led/off") >= 0) {
    digitalWrite(LED_EXTERNO, LOW);
    respuesta = "LED apagado";
  }

  // Control de la matriz
  if (peticion.indexOf("/matriz/on") >= 0) {
    matriz.renderBitmap(dibujo, 8, 12);
    respuesta = "Matriz encendida";
  }

  if (peticion.indexOf("/matriz/off") >= 0) {
    matriz.clear();
    respuesta = "Matriz apagada";
  }

  // Control de velocidad
  int posicion = peticion.indexOf("/vel/");

  if (posicion >= 0) {
    char numero = peticion.charAt(posicion + 5);

    if (numero >= '0' && numero <= '3') {
      nivel = numero - '0';
      aplicarVelocidad();
      respuesta = NOMBRE[nivel];
    }
  }

  cliente.println("HTTP/1.1 200 OK");
  cliente.println("Content-Type: text/plain");
  cliente.println("Connection: close");
  cliente.println();
  cliente.println(respuesta);

  delay(5);
  cliente.stop();
}