#include <ESP8266WiFi.h>            
#include <ESP8266WebServer.h>

int auxPin1 = 12;
int auxPin1_alt = 14;

int auxPin2 = 15;
int auxPin2_alt = 13;

bool w1_rev = false;
bool w2_rev = false;

bool delayStop = false;
int delayTimer = 0;

double power = 0;

ESP8266WebServer server(80);    //Webserver Object
String ipaddress = "";

int RH =0;
int BH =0;
int GH =0;
int Aux1H =0;
int Aux2H =0;


int Aux1C =1;
int Aux2C =1;


bool Aux1_On =false;
bool Aux2_On =false;

void delayFunc(int d_time)
{
  delayTimer = d_time*100;
  delayStop = true;

}

void setAux(int aux1, int aux2)
{
  Aux1H=aux1;
  Aux2H=aux2;
}

void updateStop(int elapsed)
{
  delayTimer -=elapsed;
  if(delayTimer<0)
  {
    if(delayStop)
    {
      power=1;
      delayStop=false;
    }
    else
    {
      delayTimer = 0;
    }
  }
  
  
  
}
void updateLights(int elapsed)
{

  Aux1C-=elapsed;
  Aux2C-=elapsed;

  if(Aux1C<=0)
  {
    
    if(Aux1_On)
    {
      Aux1_On=false;
      if(Aux1H!=100)
      {
        if(!w1_rev)
        {
          digitalWrite(auxPin1, HIGH);    
          digitalWrite(auxPin1_alt, LOW);
        }
        else
        {
          digitalWrite(auxPin1, LOW);    
          digitalWrite(auxPin1_alt, HIGH);
        }
           
      }
      Aux1C=100-Aux1H;
    }
    else
    {
      Aux1_On=true;  
      digitalWrite(auxPin1, LOW);
      digitalWrite(auxPin1_alt, LOW);       
      Aux1C=Aux1H;
    }
  }
  if(Aux2C<=0)
  {
    if(Aux2_On)
    {
      Aux2_On=false;
      if(Aux2H!=100)
      {
        if(!w2_rev)
        {
          digitalWrite(auxPin2, HIGH); 
          digitalWrite(auxPin2_alt, LOW); 
        }
        else
        {
          digitalWrite(auxPin2, LOW); 
          digitalWrite(auxPin2_alt, HIGH);
        }
             
      }
      Aux2C=100-Aux2H;
    }
    else
    {
      Aux2_On=true;  
      digitalWrite(auxPin2, LOW);
      digitalWrite(auxPin2_alt, LOW);      
      Aux2C=Aux2H;
    }
  }
  
}

int prevTime=0;
int curTime =0;
int state = 0;

String javascriptCode = " <!DOCTYPE html> "

"<html> "

"<style>"
"body {"
    "background-color: #abb2b5;"
"}"
"</style>"
"<body> "

"<style>"
".button {"
    "border: none;"
    "color: white;"
    "padding: 15px 32px;"
    "text-align: center;"
    "text-decoration: none;"
    "display: inline-block;"
    "font-size: 16px;"
    "margin: 8px 4px;"
    "cursor: pointer;"
    "width: 192px;"
    "height: 150px;"
"}"

".border {"
    "border-style: solid;"
    "border-color: #7d8284;"
    "margin: 4px 2px;"
    "width: 600px;"
"}"

".buttonOff {background-color: #3a3c3d;"
    "width: 592px;"
    "height: 100px;"
"}" 

".buttonWhite {background-color: #ffffff;"
    "width: 592px;"
    "height: 100px;"
"}"

".buttonBrightControl {background-color: #ced8db;"
    "width: 92px;"
    "height: 92px;"
"}" 
".buttonUp {background-color: #ced8db;"
    "width: 196px;"
    "height: 142px;"
    "margin: 4px 202px;"
"}" 
".buttonCen {background-color: #ced8db;"
    "width: 196px;"
    "height: 142px;"
    "margin: 4px 2px;"
"}" 
".buttonRed {background-color: #f44336;}" 
".buttonGreen {background-color: #4CAF50;}" 
".buttonBlue {background-color: #008CBA;}"
".buttonPink {background-color: #ff00ff;}"
".buttonLightGreen {background-color: #ffff00;}"
".buttonLightBlue {background-color: #00ffff;}"
".buttonOrange {background-color: #ff9000;}"
".buttonTeal {background-color: #0090ff;}"
".buttonPurple {background-color: #9000ff;}"
".buttonDarkPink {background-color: #ff0090;}"
".buttonBlueGreen {background-color: #00ff90;}"
".buttonGreenYellow {background-color: #90ff00;}"
"</style>"

"<h2>RC Control</h2>"



"<div class=\"border\">"
"<h2>Power</h2>"
"<button class=\"button buttonBrightControl\" id=\"bAux1_0\">0%</button>"
"<button class=\"button buttonBrightControl\" id=\"bAux1_20\">20%</button>"
"<button class=\"button buttonBrightControl\" id=\"bAux1_40\">40%</button>"
"<button class=\"button buttonBrightControl\" id=\"bAux1_60\">60%</button>"
"<button class=\"button buttonBrightControl\" id=\"bAux1_80\">80%</button>"
"<button class=\"button buttonBrightControl\" id=\"bAux1_100\">100%</button>"
"</div>"

"<div class=\"border\">"
"<button class=\"button buttonUp\" id=\"bUp\">^</button>"
"</div>"

"<div class=\"border\">"
"<button class=\"button buttonCen\" id=\"bLeft\"><</button>"
"<button class=\"button buttonCen\" id=\"bOff\">Off</button>"
"<button class=\"button buttonCen\" id=\"bRight\">></button>"
"</div>"

"<div class=\"border\">"
"<button class=\"button buttonUp\" id=\"bDown\">^</button>"
"</div>"

"<script>"

"document.getElementById(\"bAux1_0\").addEventListener(\"click\", function(){"
"location.replace(\"/aux1_0\")"
"});"
"document.getElementById(\"bAux1_20\").addEventListener(\"click\", function(){"
"location.replace(\"/aux1_20\")"
"});"
"document.getElementById(\"bAux1_40\").addEventListener(\"click\", function(){"
"location.replace(\"/aux1_40\")"
"});"
"document.getElementById(\"bAux1_60\").addEventListener(\"click\", function(){"
"location.replace(\"/aux1_60\")"
"});"
"document.getElementById(\"bAux1_80\").addEventListener(\"click\", function(){"
"location.replace(\"/aux1_80\")"
"});"
"document.getElementById(\"bAux1_100\").addEventListener(\"click\", function(){"
"location.replace(\"/aux1_100\")"
"});"

"document.getElementById(\"bUp\").addEventListener(\"click\", function(){"
"location.replace(\"/_up\")"
"});"

"document.getElementById(\"bDown\").addEventListener(\"click\", function(){"
"location.replace(\"/_down\")"
"});"

"document.getElementById(\"bLeft\").addEventListener(\"click\", function(){"
"location.replace(\"/_left\")"
"});"
"document.getElementById(\"bRight\").addEventListener(\"click\", function(){"
"location.replace(\"/_right\")"
"});"
"document.getElementById(\"bOff\").addEventListener(\"click\", function(){"
"location.replace(\"/_off\")"
"});"


"</script>"

"</body> "

"</html> ";


void setup() {

pinMode(auxPin1, OUTPUT);
pinMode(auxPin1_alt, OUTPUT);
pinMode(auxPin2, OUTPUT);
pinMode(auxPin2_alt, OUTPUT);

prevTime=micros()/100;

Serial.begin(115200);                                             //Open Serial connection

WiFi.begin("NETWORK_NAME", "NETWORK_PASSWORD");                      //Connect to the WiFi network

while (WiFi.status() != WL_CONNECTED) {    //Wait for connection

delay(500);
Serial.println("Waiting to connect…");

power=1;

setAux(100,100);
}

Serial.print("IP address: ");
ipaddress = WiFi.localIP().toString();
Serial.println(ipaddress);  //Print the local IP

server.on("/", []() {                     //Define the handling function for root path (HTML message)
server.send(200, "text/html", javascriptCode);
power=1;
});

server.on("/aux1_0", []() { //Define the handling function for the javascript path
server.send(200, "text/html", javascriptCode);
power=1;
});
server.on("/aux1_20", []() { //Define the handling function for the javascript path
server.send(200, "text/html", javascriptCode);
power=.8;
});
server.on("/aux1_25", []() { //Define the handling function for the javascript path
//server.send(200, "text/html", javascriptCode);
power=.75;
});
server.on("/aux1_30", []() { //Define the handling function for the javascript path
//server.send(200, "text/html", javascriptCode);
power=.7;
});
server.on("/aux1_35", []() { //Define the handling function for the javascript path
//server.send(200, "text/html", javascriptCode);
power=.65;
});
server.on("/aux1_40", []() { //Define the handling function for the javascript path
server.send(200, "text/html", javascriptCode);
power=.6;
});
server.on("/aux1_45", []() { //Define the handling function for the javascript path
//server.send(200, "text/html", javascriptCode);
power=.55;
});
server.on("/aux1_50", []() { //Define the handling function for the javascript path
//server.send(200, "text/html", javascriptCode);
power=.5;
});
server.on("/aux1_55", []() { //Define the handling function for the javascript path
//server.send(200, "text/html", javascriptCode);
power=.45;
});
server.on("/aux1_60", []() { //Define the handling function for the javascript path
server.send(200, "text/html", javascriptCode);
power=.4;
});
server.on("/aux1_65", []() { //Define the handling function for the javascript path
//server.send(200, "text/html", javascriptCode);
power=.35;
});
server.on("/aux1_70", []() { //Define the handling function for the javascript path
//server.send(200, "text/html", javascriptCode);
power=.3;
});
server.on("/aux1_75", []() { //Define the handling function for the javascript path
//server.send(200, "text/html", javascriptCode);
//aux1State=100;
power=.25;
});
server.on("/aux1_80", []() { //Define the handling function for the javascript path
server.send(200, "text/html", javascriptCode);
power=.2;
});
server.on("/aux1_85", []() { //Define the handling function for the javascript path
//server.send(200, "text/html", javascriptCode);
power=.15;
});
server.on("/aux1_90", []() { //Define the handling function for the javascript path
//server.send(200, "text/html", javascriptCode);
power=.1;
});
server.on("/aux1_95", []() { //Define the handling function for the javascript path
//server.send(200, "text/html", javascriptCode);
power=.05;
});
server.on("/aux1_100", []() { //Define the handling function for the javascript path
server.send(200, "text/html", javascriptCode);
power=0;
});

server.on("/_up", []() { //Define the handling function for the javascript path
server.send(200, "text/html", javascriptCode);
w1_rev = false;
w2_rev = false;
});

server.on("/_down", []() { //Define the handling function for the javascript path
server.send(200, "text/html", javascriptCode);
w1_rev = true;
w2_rev = true;
});

server.on("/_left", []() { //Define the handling function for the javascript path
server.send(200, "text/html", javascriptCode);
w1_rev = true;
w2_rev = false;
});
server.on("/_right", []() { //Define the handling function for the javascript path
server.send(200, "text/html", javascriptCode);
w1_rev = false;
w2_rev = true;
});
server.on("/_off", []() { //Define the handling function for the javascript path
server.send(200, "text/html", javascriptCode);
power=1;
});
server.on("/_r", []() { //Define the handling function for the javascript path
server.send(200, "text/html", javascriptCode);
power=.3;
w1_rev = false;
w2_rev = true;
delayFunc(10);
});
server.on("/_l", []() { //Define the handling function for the javascript path
server.send(200, "text/html", javascriptCode);
power=.3;
w1_rev = true;
w2_rev = false;
delayFunc(10);
});
server.on("/_for", []() { //Define the handling function for the javascript path
server.send(200, "text/html", javascriptCode);
power=.4;
w1_rev = true;
w2_rev = true;
delayFunc(20);
});
server.on("/_rev", []() { //Define the handling function for the javascript path
server.send(200, "text/html", javascriptCode);
power=.4;
w1_rev = false;
w2_rev = false;
delayFunc(20);
});



server.begin(); //Start the server

Serial.println("Server listening");

}

void loop() {
  
  curTime=micros()/100;
  updateLights(curTime-prevTime);
  updateStop(curTime-prevTime);
  setAux((100*power),(100*power));
  server.handleClient(); //Handling of incoming requests

  prevTime=curTime;
}
