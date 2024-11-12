#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Adafruit_BMP085.h>
#include <SimpleDHT.h>


int pinDHT11 = 16;
SimpleDHT11 dht11(pinDHT11);

Adafruit_BMP085 bmp; 

ESP8266WebServer server(80);
WiFiClient cliente;//creamos un cliente

const char* ssid     = "Fibertel WiFi352 2.4GHz";
const char* password = "0049353561";
// Funcion que se ejecutara en la URI '/'
void PaginaWeb() 
{
 server.send(200,"text/html","<!DOCTYPE html><html lang='es'><head> <meta charset='UTF-8'> <meta name='viewport' content='width=device-width, initial-scale=1.0'> <title>ESP8266 estacion meteorologica</title> <link rel='stylesheet' href='https://unpkg.com/bulma@0.9.1/css/bulma.min.css'> <link rel='stylesheet' href='https://stackpath.bootstrapcdn.com/font-awesome/4.7.0/css/font-awesome.min.css'></head><body> <section id='app' class='hero is-link is-fullheight'> <div class='hero-body'> <div class='container'> <div class='columns has-text-centered'> <div class='column'> <h1 style='font-size: 2.5rem'>Termómetro</h1> <i class='fa' :class='claseTermometro' style='font-size: 4rem;'></i> </div><div class='column has-text-centered'> <h2 class='is-size-4 has-text-warning'>Humedad</h2> <h2 class='is-size-1'>{{humedad}}%</h2> </div><div class='column has-text-centered'> <h3 class='is-size-4 has-text-warning'>Luminosidad</h3> <h3 class='is-size-1'>{{luminosidad}}%</h3> </div><div class='column has-text-centered'> <h2 class='is-size-4 has-text-warning'>Temperatura</h2> <h2 class='is-size-1'>{{temperatura}}*C</h2> </div><div class='column has-text-centered'> <h3 class='is-size-4 has-text-warning'>Altitud</h3> <h3 class='is-size-1'>{{altitud}}m</h3> </div><div class='column has-text-centered'> <h3 class='is-size-4 has-text-warning'>Presion</h3> <h3 class='is-size-1'>{{presion}}Pa</h3> </div></div></div></div><div class='columns'> <div class='column'> <p>Última lectura: Hace <strong class='has-text-white'>{{ultimaLectura}}</strong> segundo(s)</p></div></div></section> <script src='https://unpkg.com/vue@2.6.12/dist/vue.min.js'></script> <script>const INTERVALO_REFRESCO=5000; new Vue({el: '#app', data: ()=> ({ultimaLectura: 0, temperatura: 0, humedad: 0, luminosidad: 0, altitud : 0, presion :0,}), mounted(){this.refrescarDatos();}, methods:{async refrescarDatos(){try{const respuestaRaw=await fetch('./api'); const datos=await respuestaRaw.json(); this.ultimaLectura=datos.u; this.temperatura=datos.t; this.humedad=datos.h; this.luminosidad=datos.l; this.altitud=datos.a; this.presion=datos.p; setTimeout(()=>{this.refrescarDatos();}, INTERVALO_REFRESCO);}catch (e){setTimeout(()=>{this.refrescarDatos();}, INTERVALO_REFRESCO);}}}, computed:{claseTermometro(){if (this.temperatura <=5){return 'fa-thermometer-empty';}else if (this.temperatura > 5 && this.temperatura <=13){return 'fa-thermometer-quarter';}else if (this.temperatura > 13 && this.temperatura <=21){return 'fa-thermometer-half';}else if (this.temperatura > 21 && this.temperatura <=30){return 'fa-thermometer-three-quarters';}else{return 'fa-thermometer-full';}}}}); </script></body></html>" );
}
// Funcion que se ejecutara en URI desconocida
void handleNotFound() 
{
   server.send(404, "text/plain", "Not found");
}
void rutaJson();

void setup()
{
  Serial.begin(9600);
  delay(10);
  
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Conectando a:\t");
  Serial.println(ssid); 

  // Esperar a que nos conectemos
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(200);
  Serial.print('.');
  }

  // Mostrar mensaje de exito y dirección IP asignada
  Serial.println();
  Serial.print("Conectado a:\t");
  Serial.println(WiFi.SSID()); 
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());

 
   // Ruteos
  server.on("/", PaginaWeb);
  server.on("/api", rutaJson);
  server.on("/inline", []() {
      server.send(200, "text/plain", "Esto tambien funciona");
   });
 
   // Ruteo para URI desconocida
   server.onNotFound(handleNotFound);
  
   // Iniciar servidor
   server.begin();
   Serial.println("Server iniciado");

   if (!bmp.begin()) {
	Serial.println("Could not find a valid BMP085 sensor, check wiring!");
	delay(500);
  }
} 
  int ultimaVezLeido = 0;
  long intervaloLectura = 5000; // Debería ser mayor que 2000
  unsigned long ultimaLecturaExitosa = 0;

  int luminosidad=0, temperatura=0, humedad=0, altitud =0, presion =0;

void loop()
{
   server.handleClient();
  
   if (ultimaVezLeido > intervaloLectura)
  {
    float nuevaLuminosidad = analogRead(A0);
    int nuevaAltitud= bmp.readAltitude();
    int nuevaPresion = bmp.readPressure();
    int nuevaTemperatura = bmp.readTemperature();
     // read without samples.
    byte temperature = 0;
    byte humidity = 0;
  int err = SimpleDHTErrSuccess;
  if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Read DHT11 failed, err="); Serial.print(SimpleDHTErrCode(err));
    Serial.print(","); Serial.println(SimpleDHTErrDuration(err)); delay(1000);
    return;
  }
    // Si los datos son correctos, actualizamos las globales
    if (isnan(nuevaLuminosidad)|| isnan(nuevaTemperatura) || isnan(nuevaAltitud)  || isnan(nuevaPresion) ) {
      ultimaVezLeido = 0;
      return;
    }
    ultimaLecturaExitosa = millis();
    luminosidad=nuevaLuminosidad *100 /1024;
    temperatura = nuevaTemperatura;
    altitud =nuevaAltitud;
    presion =nuevaPresion;
    ultimaVezLeido = 0;
    humedad =humidity;
  }
  delay(1);
  ultimaVezLeido += 1;
}
// Nuestra pequeña API
void rutaJson()
{
  // Calcular última lectura exitosa en segundos
  unsigned long tiempoTranscurridoEnMilisegundos = millis() - ultimaLecturaExitosa;
  int tiempoTranscurrido = tiempoTranscurridoEnMilisegundos / 1000;
  // Búfer para escribir datos en JSON
  char bufer[50];
  // Crear la respuesta pasando las variables globales
  // La salida será algo como:
  // {"t":14.20,"h":79.20,"l":5.00}
  sprintf(bufer, "{\"t\":%d,\"h\":%d,\"l\":%d,\"u\":%d, \"a\":%d, \"p\":%d}", temperatura, humedad, luminosidad, tiempoTranscurrido,  altitud, presion);
  // Responder con ese JSON
  server.send(200, "application/json", bufer);
}

