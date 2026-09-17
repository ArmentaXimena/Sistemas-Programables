# Control de LED con delay y millis

Encendido de tres LED comparando dos formas de temporización en Arduino UNO R4 WiFi: `delay()` y `millis()`.

## Descripción

Dos programas creados en **Arduino IDE** sobre el mismo circuito de tres LED (verde, amarillo y rojo). El primero los enciende en secuencia usando `delay()`; el segundo los controla de forma independiente y simultánea usando `millis()`, además de enviar un mensaje por el Monitor Serie cada vez que se enciende el LED rojo.

## Objetivos de aprendizaje

Comparar el comportamiento de `delay()` y `millis()` para controlar tiempos en Arduino, identificando cómo `delay()` bloquea la ejecución del programa mientras que `millis()` permite controlar varios temporizadores sin detenerlo.

## Material utilizado

- Arduino UNO R4 WiFi y cable USB.
- 3 LED (1 verde, 1 amarillo y 1 rojo).
- 3 resistencias de 330 Ω.
- Protoboard.
- Cables Dupont.
- Computadora con Arduino IDE y Monitor Serie a 9600 baudios (para la versión con millis).

## Diagrama del circuito

![Diagrama del circuito](Diagrama/Delay.jpg).

Diagrama de referencia con Arduino UNO. En la práctica se utilizó el Arduino UNO R4 WiFi.

## Código

- [Programa con delay](Codigo/Delay.ino).
- [Programa con millis](Codigo/millis.ino).

| Elemento | Pin | Tiempo / intervalo |
|---|---|---|
| LED verde | 9 | 500 ms |
| LED amarillo | 10 | 1000 ms |
| LED rojo | 11 | 1500 ms |

Con `delay()`, cada LED se enciende, espera su tiempo asignado y se apaga antes de pasar al siguiente, por lo que solo uno está encendido a la vez y el ciclo completo dura 3000 ms. Con `millis()`, cada LED compara su propia variable de tiempo contra el tiempo transcurrido, así que los tres cambian de estado de forma independiente y pueden parpadear al mismo tiempo; al encender el rojo, se imprime el mensaje "Ximena la mas chambeadora" por el Monitor Serie.

## Evidencias de armado

![Armado del circuito](Diagrama/Delay1.jpg)

- [Detalle de los LED y las resistencias en protoboard](Diagrama/Delay1.jpg).
- [Conexión de los cables con el Arduino UNO R4 WiFi](Diagrama/Delay2.jpg).

## Reporte

- [Reporte de la práctica con delay](Reporte/Reporte_Practica_LED_Delay.pdf).
- [Reporte de la práctica con millis](Reporte/Reporte_Practica_Millis_Arduino_R4_WiFi.pdf).

Incluyen:

- Objetivo, materiales y procedimiento de cada versión.
- Tablas y gráficas de los tiempos programados.
- Observaciones sobre el comportamiento del sistema y, en el caso de millis, la salida del Monitor Serie.

## Conclusiones

La práctica permitió comparar ambas formas de temporización: `delay()` bloquea la ejecución del programa mientras espera, por lo que la secuencia de LED depende del orden de las instrucciones; `millis()` en cambio permite que el `loop()` siga revisando constantemente el tiempo transcurrido de cada LED, controlando varios temporizadores de forma independiente y sin detener el resto del sistema, como el envío de mensajes por serial.

## Resultados

**Con delay():** solo un LED está encendido a la vez; mientras uno está en HIGH, los otros dos permanecen en LOW. El ciclo completo dura aproximadamente 3 segundos (500 + 1000 + 1500 ms).

**Con millis():** los tres LED cambian de estado en su propio intervalo, mostrando parpadeos superpuestos en vez de una secuencia uno tras otro. El LED rojo se enciende por primera vez a los 1.5 segundos y repite cada 3 segundos, coincidiendo cada vez con un nuevo mensaje en el Monitor Serie.

| LED | Pin | Delay: tiempo encendido | Millis: intervalo de cambio |
|---|---|---|---|
| Verde | 9 | 500 ms | 500 ms |
| Amarillo | 10 | 1000 ms | 1000 ms |
| Rojo | 11 | 1500 ms | 1500 ms |

Estos valores corresponden a los tiempos programados, no a mediciones con cronómetro u osciloscopio.

- [Resultados de la práctica con delay.pdf](Resultados/ResultadosPD.pdf).
- [Resultados de la práctica con millis.pdf](Resultados/ResultadosPM.pdf).

