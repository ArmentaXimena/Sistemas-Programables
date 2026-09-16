# Control de servomotor por interfaz web

Control de un servomotor **MG996R** desde una página web utilizando un **Arduino UNO R4 WiFi**.

## Descripción

El Arduino UNO R4 WiFi crea una red inalámbrica llamada **Arduino-Servo** y funciona como servidor web en el puerto 80. Desde un navegador se puede seleccionar un ángulo entre **0° y 180°** y enviarlo al Arduino para posicionar el servomotor.

La página web incluye una barra deslizante para seleccionar el ángulo y un botón **Mover Servo**. El valor recibido también se muestra en el Monitor Serie.

## Objetivos de aprendizaje

Controlar la posición de un servomotor desde una interfaz web, utilizar la conexión Wi-Fi del Arduino UNO R4 WiFi y comprender el envío de datos mediante peticiones HTTP.

## Material utilizado

- Arduino UNO R4 WiFi.
- Servomotor MG996R de 180°.
- Cables Dupont.
- Cable USB-C.
- Computadora o teléfono con navegador web.
- Arduino IDE.

## Diagrama del circuito

![Diagrama del circuito](diagrama/Captura%20de%20pantalla%202026-09-14%20212836.png)

El servomotor utiliza el pin **D9** del Arduino para la señal de control. La alimentación se conecta a **5V** y **GND**.

## Código

- [Programa de Arduino](codigo/CSW.ino).

El programa utiliza las librerías **WiFiS3** y **Servo**. El Arduino crea la red Wi-Fi, inicia el servidor web y recibe el parámetro `angle` enviado desde la página. Si el valor está entre 0 y 180, se utiliza `servo.write()` para mover el servomotor a la posición solicitada.

## Video del funcionamiento

[Ver video en YouTube](https://youtu.be/AtDGY__b80o)

## Evidencias de armado

![Armado del circuito](diagrama/WhatsApp%20Image%202026-09-14%20at%201.33.41%20PM.jpeg)

![Prueba del servomotor](diagrama/WhatsApp%20Image%202026-09-14%20at%201.33.41%20PM%20(1).jpeg)

- [Diagrama de conexión](diagrama/Captura%20de%20pantalla%202026-09-14%20212836.png).
- [Fotografía del montaje 1](diagrama/WhatsApp%20Image%202026-09-14%20at%201.33.41%20PM.jpeg).
- [Fotografía del montaje 2](diagrama/WhatsApp%20Image%202026-09-14%20at%201.33.41%20PM%20(1).jpeg).

## Reporte

[Reporte de la práctica.pdf](reporte/Reporte_Servomotor_Interfaz_Web_X.pdf)

Incluye:

- Objetivo, materiales y procedimiento.
- Gráfica y tabla de datos.
- Evidencias de la interfaz web y del Monitor Serie.
- Observaciones sobre el comportamiento del sistema.

## Conclusiones

La práctica permitió controlar la posición de un servomotor MG996R mediante una página web alojada directamente en el Arduino UNO R4 WiFi. Se comprobó que el Arduino puede recibir diferentes valores de ángulo por Wi-Fi y utilizarlos para mover el servomotor sin necesidad de utilizar módulos de comunicación adicionales.

También se observó en el Monitor Serie que los valores enviados desde la página fueron recibidos correctamente por el programa.

## Resultados

Durante las pruebas se enviaron diferentes ángulos desde la interfaz web y el Monitor Serie registró los valores recibidos. Entre los ángulos probados se encuentran:

| Prueba | Ángulo registrado |
|---|---:|
| 1 | 50° |
| 2 | 25° |
| 3 | 124° |
| 4 | 12° |
| 5 | 166° |
| 6 | 83° |
| 7 | 124° |
| 8 | 167° |
| 9 | 7° |

La interfaz permite seleccionar valores dentro del rango de **0° a 180°** y enviar la posición deseada al servomotor.

### Interfaz web

La siguiente captura muestra la página utilizada para seleccionar el ángulo y enviar la orden al Arduino.

![Interfaz web de control del servomotor](resultados/Captura%20de%20pantalla%202026-09-14%20132953.png)

### Monitor Serie

En el Monitor Serie se observa la creación de la red **Arduino-Servo**, la dirección IP **192.168.4.1** y los diferentes ángulos recibidos durante las pruebas.

![Monitor Serie con los ángulos registrados](resultados/Captura%20de%20pantalla%202026-09-14%20133018.png)

- [Captura de la interfaz web](resultados/Captura%20de%20pantalla%202026-09-14%20132953.png).
- [Captura del Monitor Serie](resultados/Captura%20de%20pantalla%202026-09-14%20133018.png).
- [Resultados de la práctica.pdf](resultados/ResultadosPCSW.pdf).
