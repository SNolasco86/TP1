# TP1
Trabajo Práctico Nro1 Curso ARM-Book
## Nombre: Santiago Nolasco 
## email: santiagonolasco86@gmail.com

# Medidor de concentración de alcohol en el aire (Alcoholímetro)
## Descripción:
El sistema es un medidor de alcohol, esto lo toma a través de un sensor conectada a la entrada analógica (A1). Esta medición sólo lo hará cuando el sistema este encendido (BUTTON1) y enviará los datos cuando se pulse el pulsador(D2). Mientras no se encienda el sistema no se tomará medición alguna ni tampoco se enviarán datos por el puerto serial.
El sensor de alcohol será modelado por un potenciómetro conectado a la entrada analógica.
Cuando el sistema este encendido se iluminará un led (LED1) y cuando se sobrepase un nivel de la entrada analógica se encenderá pitando un buzzer modelado por otro led (LED2).

# Vistas
<img src="https://github.com/SNolasco86/TP1/blob/main/placa.jpeg" width="425" height="495">

# Consideraciones de la Placa

 - **Entradas digitales:** BUTTON1 conectadas pulldown y D2 conectada pullup.
 - **Entrada analógica:** A1 conectada al potenciómetro.
 - **Salidas digitales:** LED1 on/off sistema y LED2 alarma sobre nivel de alcohol.
 - **Coenxión serie:** Conexión usb-serial

# Materiales

 - **NUCLEO F429ZI
 - **Cable usb
 - **Potenciómetro
 - **Pulsador NA
 - **Cables macho-macho varios
   
# Video
[![Alt text](https://img.youtube.com/vi/0aKbFhh9uZo/0.jpg)](https://www.youtube.com/watch?v=0aKbFhh9uZo)
