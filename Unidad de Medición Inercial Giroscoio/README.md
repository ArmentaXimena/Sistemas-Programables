# Inclinómetro con MPU-6050 y control de motorreductor

**Práctica 3.2.3 – Unidad de Medición Inercial (IMU 6-DOF MPU-6050)**
Instituto Tecnológico de Mazatlán · Ingeniería en Sistemas Computacionales · Materia: Sistemas Programables
Profesor: Miguel Barrón Hernández
Integrantes: Ximena Armenta Dávila, Carlos Tadeo Ibarra Castañeda, Dulce Princesa Ulivarría González

## Contenido del repositorio

| Archivo | Descripción |
|---|---|
| `Inclinometro_R4.ino` | Sketch de Arduino (UNO R4 WiFi) con el inclinómetro y el control del motor |
| `Reporte_Unidad_Medicion_Inercial_MPU6050.pdf` | Reporte completo de la práctica |
| `img/conexion_i2c.jpg` | Diagrama de conexión I2C Arduino ↔ MPU-6050 |
| `img/montaje.jpg` | Foto del montaje funcionando |
| `img/monitor_serie.png` | Captura del Monitor Serie |

## Descripción

El MPU-6050 integra un acelerómetro y un giroscopio de tres ejes. En esta práctica se leen sus registros directamente por I2C (sin biblioteca externa) para estimar el ángulo de inclinación frontal (*pitch*, giro sobre el eje Y). El signo del ángulo define el sentido de giro de un motorreductor y su magnitud define el PWM, mediante un puente H L298N.

## Objetivos

- Comunicar el sensor por I2C y verificar su identidad leyendo `WHO_AM_I` (registro `0x75`, valor `0x68`).
- Configurar los rangos (±2 g y ±250 °/s) y calibrar el giroscopio en el eje Y.
- Combinar ambos sensores con un filtro complementario y aplicar una zona muerta de ±5°.
- Controlar el L298N con PWM, desacelerar antes de invertir el giro y detener el motor ante una falla.
- Coordinar cuatro tareas con `millis()` y mostrar la inclinación en el Monitor Serie y en la matriz LED.

## Material

- Arduino UNO R4 WiFi y cable USB-C de datos
- Módulo MPU-6050 (GY-521) y cables Dupont
- Módulo puente H L298N con disipador
- Motorreductor de CC con rueda
- Pila de 9 V para la alimentación del motor
- Arduino IDE con el paquete *Arduino UNO R4 Boards* (bibliotecas `Wire` y `Arduino_LED_Matrix`); Monitor Serie a 115200 baudios

## Conexiones

| Elemento | Arduino / conexión | Función |
|---|---|---|
| MPU-6050 SDA | A4 / SDA | Datos I2C |
| MPU-6050 SCL | A5 / SCL | Reloj I2C |
| MPU-6050 GND | GND común | Referencia |
| MPU-6050 AD0 | GND | Dirección I2C `0x68` |
| L298N ENA | D9 (PWM) | Potencia del canal A (sin jumper) |
| L298N IN1 / IN2 | D8 / D7 | Sentido de giro |
| L298N OUT1 / OUT2 | Cables del motor | Salida al motorreductor |

![Conexión I2C entre el Arduino UNO R4 WiFi y el MPU-6050](img/conexion_i2c.jpg)

**Notas de montaje**

- El diagrama alimenta VCC del módulo GY-521 con 5 V; esto solo es válido si la placa tiene regulador. El chip trabaja a ~3.3 V, y conviene verificar los niveles lógicos de SDA y SCL.
- El Arduino se alimenta por USB y la pila de 9 V va a la entrada de potencia del L298N. Debe haber tierra común entre Arduino, sensor, puente y fuente.
- Se retira el jumper de ENA para controlarlo con PWM. El código recomienda una resistencia de 10 kΩ entre ENA y GND para mantener el motor deshabilitado durante el arranque o reinicio.
- Una pila de 9 V rectangular tiene capacidad limitada para un motor.

## Funcionamiento del programa

1. **Arranque seguro:** las salidas del motor se ponen en bajo antes de iniciar el sensor.
2. **Identificación y configuración:** se lee `WHO_AM_I`, se despierta el sensor y se fijan los rangos.
3. **Calibración:** con el sensor quieto se promedian 500 lecturas del giroscopio Y (~5 s) para obtener el offset.
4. **Ángulo inicial** calculado con el acelerómetro, para que el filtro no parta de un cero ficticio.
5. **Filtro complementario** con constante de tiempo de 0.5 s, ajustado al intervalo real entre lecturas.
6. **Control del motor:** |pitch| < 5° → PWM 0; de 5° a 45° el PWM objetivo crece de 90 a 255; sobre 45° se limita al máximo. Positivo = adelante, negativo = reversa.
7. **Rampa:** cambia 9 unidades de PWM cada 20 ms (~580 ms de 0 a 255) y pasa por cero antes de invertir.
8. **Paro de seguridad:** ante una lectura fallida, ENA se pone en cero sin rampa; el programa reintenta localizar el sensor y reanuda desde PWM 0.

### Tareas del ciclo principal

| Tarea | Intervalo | Acción |
|---|---|---|
| Sensor y filtro | 10 ms | Obtener *pitch* y calcular PWM objetivo |
| Rampa y motor | 20 ms | Acercar el PWM aplicado al objetivo |
| Monitor Serie | 500 ms | Informar solo si cambia el estado mostrado |
| Matriz LED | 50 ms | Punto 2×2 según el ángulo, marco en ±2°, **X** en falla |

`delay()` solo se usa en el arranque y la calibración, con el motor apagado.

## Evidencia

![Montaje con Arduino UNO R4 WiFi, L298N y motorreductor con rueda](img/montaje.jpg)

Durante la puesta en marcha apareció el aviso `ERROR INICIAL: MPU-6050 no responde 0x68 o no se configura`. Para localizar el problema se preparó una prueba I2C que busca dispositivos en `Wire` y `Wire1` y lee `WHO_AM_I` en `0x68` y `0x69`.

![Monitor Serie con inclinación, sentido del motor, PWM y avisos del centro](img/monitor_serie.png)

## Resultados

El porcentaje de PWM es la orden aplicada al puente H, no una medición de velocidad (el montaje no tiene encoder).

| Ángulo | Motor | PWM | Interpretación |
|---|---|---|---|
| +12° | Adelante | 47 % | Orden proporcional fuera de la zona muerta |
| −8° | Reversa | 32 % | Transición hacia un objetivo negativo |
| −30° | Reversa | 76 % | Mayor inclinación, mayor PWM |
| −45° y −73° | Reversa | 100 % | PWM limitado al máximo |
| +30° | Adelante | 18 % | PWM transitorio durante la rampa |
| +6° | Reversa | 58 % | Desaceleración de una orden previa antes de invertir |
| 0° | Detenido | 0 % | Sin orden de potencia |

- Cerca de 5° hay registros con motor detenido y con giro: la decisión usa el ángulo con decimales, pero el Monitor Serie lo muestra redondeado (4.7° se ve como 5° y sigue en zona muerta).
- El marco de la matriz (±2°) y la zona muerta del motor (5°) son rangos distintos; al llegar al centro, la rampa puede tardar un instante en llevar la orden a cero.
- Una inclinación positiva puede coincidir con motor en reversa momentáneamente, porque el programa reduce el PWM anterior a cero antes de cambiar de sentido.
- Las sacudidas fuertes pueden alterar temporalmente la estimación del ángulo.

## Preguntas de análisis (resumen)

- **¿Por qué no solo acelerómetro?** Mide gravedad y movimiento; al sacudirlo, el ángulo calculado cambia bruscamente.
- **¿Y solo giroscopio?** Los errores se acumulan al integrar y aparece deriva, incluso con el sensor quieto.
- **Zona muerta de ±5°:** evita que ruido y vibraciones activen el motor o cambien su sentido.
- **PWM mínimo de 90:** ~35 % de 255, ayuda a vencer la fricción de los engranes.
- **Rampa:** reduce picos de corriente (caídas de alimentación o reinicios) y golpes mecánicos al invertir.
- **Paro sin rampa:** sin lectura confiable no se debe mantener la orden de movimiento.
- **Sin `delay()`:** evita bloquear la lectura y la respuesta ante cambios o fallas.
- **Aplicaciones:** robots autobalanceados, plataformas niveladoras, controles por inclinación.

## Conclusiones

El acelerómetro y el giroscopio se complementan: la calibración y el filtro reducen errores, la zona muerta y la rampa suavizan el control del motor, el paro de seguridad protege ante fallas del sensor, y organizar las tareas con `millis()` mantiene el sistema responsivo.

## Referencias

1. Documento de la práctica del profesor: "3.2.3 Unidad de Medición Inercial".
2. InvenSense / TDK. *MPU-6000 / MPU-6050 Register Map and Descriptions*.
3. Arduino. *Arduino UNO R4 WiFi User Manual*.
4. STMicroelectronics. *L298 Dual full-bridge driver*.
5. Analog Devices. *AN-1057: Using an Accelerometer for Inclination Sensing*.

