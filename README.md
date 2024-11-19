## 2.1. Conexionado: Alimentación y Datos
Wemos D1 Mini:
Utilizamos una alimentación de 5V proporcionada por una computadora estándar, mediante un puerto USB.

Sensor LDR:
Es alimentado por 3.3V mediante el microcontrolador.
Es necesaria la colocación de una resistencia 1k ohm en serie entre la fuente y el sensor.
Utiliza el pin A0 para la transmisión de datos.


Sensor DHT11:
Es alimentado por 3.3V mediante el microcontrolador.
Es necesaria la colocación de una resistencia de 4k7 ohm en serie entre la fuente y el sensor.
Utiliza el pin D1(GPIO 16) para la transmisión de datos.


Sensor BMP180:
Es alimentado por 3.3V mediante el microcontrolador.
Utiliza los pines D1(GPIO 4) Y D2(GPIO 5) para la transmisión de datos, conectando SDA a D1 y SDL a D2.


## 2.2. Señales Indicadoras
Wemos D1 Mini:
Cuando el microcontrolador comienza a compilar el programa, comienza a titilar el led azul que viene incrustado en él.

Sensor DHT11:
Con una buena conexión, mientras está conectado a una fuente de alimentación, el led del sensor siempre estará encendido constantemente.
## 2.3. Forma de acceso a la información
Para visualizar la información, basta con seleccionar la pestaña de Serial Monitor que aparece en la esquina superior derecha.
 
Una vez compilado el código y abriendo el Serial Monitor, debería imprimir que dicho nuestro server está iniciado.

Ahora, para acceder a la página, debemos buscar nuestra URL en cualquier buscador.


Finalmente se puede visualizar la página web.


Para ver la api, basta con poner su IP /api en el buscador. Aparecerá una página similar a la adjuntada:

## 2.4. Procedimientos ante fallas del equipo y/o para funciones específicas.
Verificar que todas las librerías estén instaladas.
Adjuntamos las librerías utilizadas donde se especifica cómo instalarlas:

https://github.com/esp8266/Arduino

https://github.com/winlinvip/SimpleDHT

https://github.com/adafruit/Adafruit-BMP085-Library

En caso de utilizar otra librería, comprobar que sea funcional para los pines preestablecidos por la PCB.
Normalmente no se utiliza el pin D0 (GPIO 16) para las conexiones de sensores, en caso de utilizar la PCB adjuntada, verificar en la información de la librería si es apta para dicho pin.

Comprobar que todos los pines hayan sido soldados correctamente. 
Una de nuestras dificultades fue la mala conexión del pin y el microprocesador por mala soldadura, lo que nos atrasó el tiempo de finalizado del proyecto. Pudimos encontrar esta falla ya que al conectar el micro a una protoboard, este pin fue sacado de su lugar.  Otra forma de enterarse es viendo el led del DHT11, pues con una buena conexión, este queda encendido constantemente. En caso de titilar o no encenderse, verificar la conexión de los pines del sensor o del Wemos D1 Mini.

### El serial monitor no muestra una respuesta.
Si el código fue compilado correctamente y el microcontrolador no está mostrando respuestas, estas pueden ser algunas soluciones:

**Resetear el Wemos D1 Mini**
El proyecto fue configurado para que en caso que no haya una conexión entre la red y el micro, se genera un mensaje de la forma:.
Si el problema no es la red y persiste, puede que sea solo una falla de conexión por parte del micro que se solucione con un simple reseteo; para ello solo basta con pulsar el botón de reset que viene incluido en el dispositivo.

**Mala calibración**
Si el código y el programa de programación están configurados a distintas velocidades, se obtendrán respuestas de la forma 
Para solucionar este problema, se debe mantener todo el proyecto a la misma velocidad. Este punto se especifica en el apartado 2.5 Calibración.

## 2.5. Calibración.

Este proyecto fue ejecutado a una velocidad de ejecución de 9600 baudios. 

void setup()
{
  Serial.begin(9600);
}


Las dos opciones de velocidades utilizadas para proyectos en Arduino IDE son 9600 baudios y 115200 baudios. Esta última es más rápida y, en caso de utilizarla, deberá configurar el código. 

**Consideraciones al Cambiar la Velocidad de Transmisión**
Asegúrate de que el dispositivo al que se conecta el microcontrolador esté configurado a la misma velocidad de baudios.
Si utilizas el monitor serial del IDE de Arduino, selecciona en el menú desplegable la misma velocidad que configuraste en el código (9600, 115200.).
Ten en cuenta que velocidades más altas permiten transferir datos más rápido, pero en algunos dispositivos o entornos pueden producir errores de transmisión si la comunicación no es estable.
