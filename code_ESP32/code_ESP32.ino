#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include <Adafruit_Sensor.h>
#include <string.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>
//#include <FirebaseESP32.h>

//#define FIREBASE_HOST  "iot206-default-rtdb.firebaseio.com"
//#define FIREBASE_AUTH  "hlRVMOoG9vpIo7diuumv0w255k6PR0UpMklfQSn5"

const char* ssid = "vivo 1907";
const char* password = "11112222";

#define DHTPIN 18   
#define DHTTYPE    DHT11  

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27,16,2);

AsyncWebServer server(80);

//FirebaseData fbdb;

String readDHTTemperature() {
  float t = dht.readTemperature();
  if (isnan(t)) {
    delay(500);   
    Serial.println("Failed to read from DHT sensor!");
    return "--";
  }
  else {
   Serial.print("\n");
    Serial.print(t);
    return String(t);
 }
}

String readDHTHumidity() {
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  if (isnan(h)) {
    delay(500);
    Serial.println("Failed to read from DHT sensor!");
    return "--";
  }
  else {
    Serial.print("\t");
    Serial.print(h);
    return String(h);
  }
}

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 5.0rem; }
    p { font-size: 5.0rem; }
    .units { font-size: 2.2rem; }
    .dht-labels{
      font-size: 2.5rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
  </style>
</head>
<body>
  <h2>ESP32 DHT Server</h2>
  <p>
    <i class="fas fa-thermometer-half" style="color:#059e8a;"></i> 
    <span class="dht-labels">Temperature</span> 
    <span id="temperature">%TEMPERATURE%</span>
    <sup class="units">&deg;C</sup>
  </p>
  <p>
    <i class="fas fa-tint" style="color:#00add6;"></i> 
    <span class="dht-labels">Humidity</span>
    <span id="humidity">%HUMIDITY%</span>
    <sup class="units">&percnt;</sup>
  </p>
  <p> 
    <span class="dht-labels">__NHOM 2__</span>
      <p> 
    <span class="dht-labels"> Nguyen Viet Hoa    </span>
    <span class="dht-labels"> Truong Minh Hoang  </span>
    <span class="dht-labels"> Huynh Chi Trung    </span>
  </p>
</body>
<script>
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperature").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperature", true);
  xhttp.send();
}, 500 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("humidity").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/humidity", true);
  xhttp.send();
}, 500 ) ;
</script>
</html>)rawliteral";

// Replaces placeholder with DHT values
String processor(const String& var){
  //Serial.println(var);
  if(var == "TEMPERATURE"){
    return readDHTTemperature();
  }
  else if(var == "HUMIDITY"){
    return readDHTHumidity();
  }
  return String();
}

void setup()
{
  Serial.begin(115200);

  dht.begin();
  lcd.init();                  
  lcd.backlight();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println(WiFi.localIP());

  //Firebase.begin(FIREBASE_HOST,FIREBASE_AUTH);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readDHTTemperature().c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readDHTHumidity().c_str());
  });

  server.begin();
}
 
void loop()
{
  float t = dht.readTemperature();
  
  if (!isnan(t)) 
  {
    Serial.print("\n");
    Serial.print(t);
  } else {
    Serial.println("Failed to read temperature from DHT sensor!");
  }

  float h = dht.readHumidity();

  if (!isnan(h)) 
  {
   Serial.print("\t");
   Serial.print(h);
  } else {
    Serial.println("Failed to read humidity from DHT sensor!");
  }
  delay(500); 
 // Firebase.setInt(fbdb, "/IOT/Temperature", t);
  //Firebase.setInt(fbdb, "/IOT/Humidity", h);
  //delay(200);

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Temp:  ");
  lcd.print(t);
  lcd.print(char(223));
  lcd.print("C");

  lcd.setCursor(0, 1);
  lcd.print("Hum:  ");
  lcd.print(h);
  lcd.print("%");
}