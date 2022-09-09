#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <EEPROM.h>


const int light_pin=14;
String ssid="Smart light";
String password="rajib@hit";

AsyncWebServer server(80);
const char* input_parameter="value";
String slider_value = "0";
String S=slider_value;

const char index3_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>ok page</title>
    <style>
    body{
        background-color: black;
    }
    .textt{
        text-align: center;
        margin: auto;
        margin-top: 3vw;
    }
    .textt h1{
        color: white;
        font-size: 6vw;
    }
    </style>
</head>
<body>
    <div class="textt">
        <h1>Upload done.</h1>
    </div>
</body>
</html>
)rawliteral";

const char index2_html[] PROGMEM = R"rawliteral(<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Change password</title>
    <style>
    body{
        background-color: black;
    }
    h1{
        color: white;
    }
        .cpass{
    text-align: center;
    margin: auto;
    margin-top: 3vw;
    display: flex;
    flex-direction: column;
}
.cpass h2{
    color: white;
}
form h3{
    color: white;
}
form{
    text-align: center;
}
form input{
    width: 30vw;
    height: 3vw;
}
form button{
    display: block;
    margin: auto;
    margin-top: 3vw;
    width: 15vw;
    height: 3vw;
    font-size: medium;
    background-color: aqua;
    border: none;
    border-radius: 2vw;
}

@media only screen and (max-width: 600px) {
form input{
        width: 40vw;
        height: 5vw;
    }
    form button{
        width: 18vw;
        height: 5vw;
    }}
    </style>
</head>
<body>
    <div class="chpass">
        <form action="/changepass" method="get">
            <h2>Change Password</h2>
            <h3>New Wifi name:</h3>
            <input type="text" name="wname" id="wname" placeholder="Type wifi name">
            <h3>New password:</h3>
            <input type="password" name="pass" id="pass" placeholder="Enter new password">
            <h3>Confirm password:</h3>
            <input type="password" name="cpass" id="cpass" placeholder="Enter confirm password">
            <button type="submit">Save</button>
        </form>
        <a href="/">Home</a>
</body>
</html>)rawliteral";



const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Smart lamp offline</title>
    <style>body{
        background-color: black;
    }
    .title{
        text-align: center;
    }
    p{
        color: white;
    }
    h1{
        color: white;
    }
    .status{
        margin-top: 3vw;
        text-align: center;
    }
    
    h2{
        color: white;
    }
    .switch{
        margin: auto;
        text-align: center;
        display: flex;
        flex-direction: column;
    }
    .switch1{
        margin: auto;
        text-align: center;
        display: flex;
        flex-direction: column;
    }
    .switch2{
        margin: auto;
        text-align: center;
        display: flex;
        flex-direction: column;
    }
    .switch1 button{
        margin: auto;
        margin-top: 3vw;
        width: 20vw;
        height: 3vw;
        font-size: medium;
    }
    
    .in input{
        width: 30vw
    }
    .switch h2{
        color: white;
    }
    
    @media only screen and (max-width: 600px) {
        body {
          background-color: rgb(16, 0, 29);
        }
        h1{
            font-size: 7vw;
        }
        h2{
            font-size: 5vw;
        }
        p{
            font-size: 3.5vw;
        }
        .switch1 button{
            margin: auto;
            margin-top: 3vw;
            width: 31vw;
            height: 7.3vw;
            font-size: medium;
        }
        .in input{
            width: 51vw
        }
          height: 5vw;
    }
      }</style>
</head>
<body>
    <div class="title">
        <h1>Smart Tube light</h1>
        <p>This is a small project.</p>
    </div>
    <div class="status">
        <h2>Current Status</h2>
        <p><span id="status_value">%S%</span>%</p>
    </div>
    <div class="switch1">
        <h2>Switch</h2>
        <button id="btnon">ON</button>
        <button id="btnoff">OFF</button>
    </div>
    <div class="switch2">
        <h2>Brightness</h2>
        <div class="in">
        <input type="range" min="0" max="100" id="slider1" step="1" onchange="slider_fun(this.vlaue)" value="%S%">
        </div>
    </div>
    
    <div class="switch">
        <h2>About me</h2>
        <p>For more information copy the url.</p>
        <h2>https://github.com/rm10078</h2>
        <a href="/chpage">Change password</a>
    </div>
    
  
</body>
<script>
    document.getElementById("status_value").innerHTML = 0;
    document.getElementById("slider1").value= 0;
    var btnon = document.getElementById("btnon");
    var btnoff = document.getElementById("btnoff");
    function up_data(s_value){
        v=s_value;
        var t=document.getElementById("slider1");
        t.value=s_value;
        document.getElementById("status_value").innerHTML = v;
        var xhr = new XMLHttpRequest();
        xhr.open("GET", "/slider?value="+v, true);
        xhr.send();
      }
    btnon.addEventListener("click", function(){
        console.log("ON");
        up_data(100);
    });
    btnoff.addEventListener("click", function(){
        console.log("OFF");
        up_data(0);
    });

    function slider_fun(val){
        var t=document.getElementById("slider1");
        var v=t.value;
        document.getElementById("status_value").innerHTML = v;
        console.log(v);
        var xhr = new XMLHttpRequest();
        xhr.open("GET", "/slider?value="+v, true);
        xhr.send();
    }

</script>
</html>
)rawliteral";

String processor(const String& var){
}

void eeprom_write(String word,int start_add) {
  delay(10);

  for (int i = 0; i < word.length(); ++i) {
    Serial.println(word[i]);
    EEPROM.write(start_add+i, word[i]);
  }

  EEPROM.write(start_add+word.length(), '\0');
  EEPROM.commit();
}

String eeprom_read(int start_add) {
  String word;
  char readChar='n';
  int i = 0;

  while (readChar != '\0') {
    int tem=start_add+i;
    readChar = char(EEPROM.read(tem));
    delay(10);
    i++;

    if (readChar != '\0') {
      word += readChar;
    }
  }

  return word;
}

void light_write(int duty_cycle){
  int val=map(duty_cycle,0,100,0,255);
  analogWrite(light_pin,val);
  EEPROM.write(0,duty_cycle);
  EEPROM.commit();
  }

void setup(){
  EEPROM.begin(512);
  pinMode(light_pin,OUTPUT);
  analogWriteFreq(10000);
  byte a=EEPROM.read(0);
  int b=(int)a;
  light_write(b);
  delay(1000);
  Serial.begin(115200);
  ssid=eeprom_read(10);
  password=eeprom_read(100);
  Serial.println(ssid+"   "+"   "+password);
  Serial.println();
  Serial.print("Configuring access point...");
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  delay(500);
   server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  server.on("/slider", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String message;
    if (request->hasParam(input_parameter)) {
      message = request->getParam(input_parameter)->value();
      slider_value = message;
    //call the output function
    light_write(slider_value.toInt());
    }
    else {
      message = "No message sent";
    }
    Serial.println(message);
    request->send(200, "text/plain", "OK");
  });
  
  server.on("/changepass", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String message;
    String wname_v,pass_v,cpass_v;
    if (request->hasParam("wname")) {
      message = request->getParam("wname")->value();
      wname_v = message;
    }
    if (request->hasParam("pass")) {
      message = request->getParam("pass")->value();
      pass_v = message;
    }
    if (request->hasParam("cpass")) {
      message = request->getParam("cpass")->value();
      cpass_v = message;
    }
    Serial.println(wname_v+"  "+pass_v+"  "+cpass_v);
    if(wname_v.length() > 2 && pass_v.length() > 2 && cpass_v.length() > 2){
      eeprom_write(wname_v,10);
      if(pass_v==cpass_v && pass_v.length()>8){
      eeprom_write(pass_v,100);
      request->send_P(200, "text/html", index3_html, processor);
      }
      else{
      request->send(200, "text/plain", "password is not save");
      }
      }
    else{
      request->send(200, "text/plain", "Some values are missing.");
      }
  });
  server.on("/chpage", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index2_html, processor);
  });

  server.begin();
}

void loop(){}
