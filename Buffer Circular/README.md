# Buffer circular con interrupción y matriz LED

Registro de pulsaciones mediante una interrupción externa, almacenadas en un buffer circular, con animación simultánea en la matriz LED del Arduino UNO R4 WiFi.

## Descripción

Programa creado en **Arduino IDE** que registra cada pulsación de un botón mediante una interrupción externa (`FALLING`) y guarda su marca de tiempo en un buffer circular de 32 posiciones. En `loop()`, los eventos se extraen y se imprimen por el Monitor Serial, mientras la matriz LED integrada del Arduino UNO R4 WiFi ejecuta una animación continua con `millis()`, sin usar `delay()` en ningún punto.

## Objetivos de aprendizaje

Implementar una interrupción externa para el registro de eventos, aplicar un filtro antirrebote por software, manejar un buffer circular con protección de variables compartidas, y mantener una animación no bloqueante en paralelo.

## Material utilizado

- Arduino UNO R4 WiFi y cable USB-C.
- Pulsador de cuatro patas.
- Protoboard.
- 2 cables de conexión.
- Matriz LED integrada del Arduino UNO R4 WiFi (no requiere hardware adicional).

## Diagrama del circuito

![Diagrama del circuito](Diagrama/Diagrama.jpeg)

Diagrama de referencia con Arduino UNO. En la práctica se utilizó el Arduino UNO R4 WiFi; el pulsador se conecta entre el pin D2 y GND, sin resistencia externa.

## Código

- [Programa de Arduino](Codigo/Buffer_Circular1.ino).

| Elemento | Pin / configuración | Función |
|---|---|---|
| Pulsador | D2, `INPUT_PULLUP`, interrupción `FALLING` | Genera el aviso de una pulsación |
| Buffer circular | Arreglo de 32 posiciones | Guarda la marca de tiempo (`micros()`) de cada evento |
| Filtro antirrebote | 40 000 µs | Ignora pulsaciones separadas por menos de 40 ms |
| Matriz LED | Integrada | Muestra una línea vertical que recorre las 12 columnas cada 100 ms |

La interrupción solo guarda el instante del evento y sale; `loop()` extrae un evento por vuelta, incrementa el contador y lo imprime por el Monitor Serial (9600 baudios), además de actualizar la animación y revisar cada segundo si hubo eventos rechazados por buffer lleno.

## Evidencias de armado

![Armado del circuito](Diagrama/Codigo.jpeg)

- [Captura del código y el Monitor Serial en Arduino IDE](Diagrama/Codigo.jpeg).

## Reporte

[Reporte_Practica_Buffer_Circular.pdf](Reporte/Reporte_Practica_Buffer_Circular.pdf)

Incluye:

- Objetivo, materiales y procedimiento.
- Organización del buffer circular y tabla de tiempos registrados.
- Gráficas del conteo acumulado y de los intervalos entre eventos.
- Observaciones sobre el comportamiento del sistema.

## Conclusiones

La práctica permitió implementar el registro de eventos con un buffer circular y documentar 30 mensajes del Monitor Serial junto con el encendido de la matriz LED. La interrupción guarda los avisos y `loop()` los procesa después mientras atiende la animación, un principio aplicable, por ejemplo, a contar piezas detectadas por un sensor en una banda transportadora. Queda pendiente documentar la vuelta completa del buffer y su respuesta al llegar al límite de 32 eventos pendientes.

## Resultados

El Monitor Serial mostró 30 eventos consecutivos (#1 a #30) con tiempos estrictamente crecientes, confirmando que el buffer respeta el orden de las pulsaciones. El filtro antirrebote funcionó correctamente: el intervalo más corto entre eventos fue de 67.595 ms, por encima del umbral de 40 ms. La animación de la matriz LED se mantuvo en movimiento constante durante toda la prueba, sin bloquearse por la interrupción, y no apareció ningún aviso de "eventos rechazados por buffer lleno".

| Indicador calculado | Resultado |
|---|---|
| Eventos visibles | 30 |
| Lapso entre el primer y el último evento | 32.839437 s |
| Menor intervalo entre eventos | 67.595 ms, entre #11 y #12 |
| Mayor intervalo entre eventos | 13.370827 s, entre #24 y #25 |
| Mediana de los 29 intervalos | 158.913 ms |

Estos valores se calcularon a partir de la captura del Monitor Serial, no de mediciones con cronómetro.

- [Resultados de la práctica.pdf](Resultados/ResultadosBC.pdf).

