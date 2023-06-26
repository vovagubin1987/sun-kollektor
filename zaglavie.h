
#include <ESP8266WiFi.h>        
#include <ESP8266WebServer.h>   
#include <ESP8266SSDP.h>       
#include <FS.h>                 
#include <ArduinoJson.h>        //Установить из менеджера библиотек. https://arduinojson.org/
//                   
#include <ESP8266HTTPUpdateServer.h>  
#include <DNSServer.h> 
#include "TickerScheduler.h"       //https://github.com/Toshik/TickerScheduler Видео с уроком
#include <WebSocketsServer.h>    //https://github.com/Links2004/arduinoWebSockets 
// Библиотеки устройств
#include <DallasTemperature.h>
#include <OneWire.h>
#include <math.h> 
#include <stdlib.h> 

#define debug true // вывод отладочных сообщений
#define PIN18B20   4   // 18b20 пин
#define rele   5   // рэле пин
DeviceAddress sensor_tOutK = {0x28, 0xFC, 0xD8, 0x49, 0xF6, 0x8A, 0x3C, 0x8B};
DeviceAddress sensor_tOutN = {0x28, 0xB3, 0x22, 0x49, 0xF6, 0xE0, 0x3C, 0x7C};
// Объект для обнавления с web страницы
ESP8266HTTPUpdateServer httpUpdater;

// Web интерфейс для устройства
ESP8266WebServer server(80);

// Для файловой системы
File fsUploadFile;

// Для работы символьных имен в режиме AP
DNSServer dnsServer;

//Планировщик задач (Число задач)
TickerScheduler ts(2);

// Объект для  webSocket
WebSocketsServer webSocket = WebSocketsServer(81);

// Датчик DHT
//DHT dht;
//#define dhtPin 4

//global
signed short g_tOutK=0; //температура на выходе из коллектора
signed short g_tOutN=0; //температура на выходе из накопителя
byte i;
  byte present = 0;
  byte data[12];
  byte addr[8];

//
OneWire oneWire(PIN18B20); // вход датчиков 18b20
//
DallasTemperature ds(&oneWire);//как ds
//DallasTemperature ds1(&oneWire);//как ds
//OneWire ds1(PIN18B20);//как ds1 для поиска датчиков
//OneWire ds(PIN18B20);//как ds для поиска датчиков


const char index_html[] PROGMEM={"<!doctype html>\n<html>\n<head>\n<meta charset=\"utf-8\">\n<title>Web Server</title>\n<meta name=\"generator\" content=\"WYSIWYG Web Builder 12 - http://www.wysiwygwebbuilder.com\">\n<link href=\"Untitled1.css\" rel=\"stylesheet\">\n<link href=\"index.css\" rel=\"stylesheet\">\n</head>\n<body>\n<div id=\"wb_Text1\" style=\"position:absolute;left:34px;top:41px;width:570px;height:23px;z-index:0;\">\n<span style=\"color:#000000;font-family:Arial;font-size:20px;\">TURN ON:</span></div>\n<div id=\"wb_Text2\" style=\"position:absolute;left:34px;top:85px;width:570px;height:23px;z-index:1;\">\n<span style=\"color:#000000;font-family:Arial;font-size:20px;\">TURN OFF:</span></div>\n<input type=\"submit\" id=\"Button1\" onclick=\"window.location.href=' ./on';return false;\" name=\"\" value=\"Submit\" style=\"position:absolute;left:146px;top:40px;width:96px;height:25px;z-index:2;\">\n<input type=\"submit\" id=\"Button2\" onclick=\"window.location.href='./off';return false;\" name=\"\" value=\"Submit\" style=\"position:absolute;left:146px;top:84px;width:96px;height:25px;z-index:3;\">\n</body>\n</html>"};
String ip = "";
const String tr_td="<tr><td>";
const String _td_td="</td><td>";
const String _td__tr="</td><tr>";
const int g_TBreakOff=85;
bool g_Nasos=false;
unsigned long g_Watt=0;
unsigned long g_Time=0;
const unsigned long g_TOpros=180000;
byte indexforindex = 0;

float t[] = {0, 0, 0, 0, 0, 0, 0, 0};

String _ssid="";
String _password="";

String _ssidAP="sun_kollektor_v1";
String _passwordAP="sun_kollektor_v1";
const short g_M=10;
const short g_C=42;


//
DeviceAddress Thermometer;


     signed short  tmp_tOutK=0;
  signed short tmp_tOutN=0;
  signed short dt=0;
  signed short tmp1=0;
  signed short tmp2=0;

