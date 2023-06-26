#include "zaglavie.h"

void WIFIinit() {
  

  // Попытка подключения к точке доступа
  WiFi.mode(WIFI_STA);
  byte tries = 11;
  
  if (_ssid == "" && _password == "") {
    WiFi.begin();
  }
  else {
    WiFi.begin(_ssid.c_str(), _password.c_str());
  }
  // Делаем проверку подключения до тех пор пока счетчик tries
  // не станет равен нулю или не получим подключение
  while (--tries && WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(1000);
    
  }
  if (WiFi.status() != WL_CONNECTED)
  {
    // Если не удалось подключиться запускаем в режиме AP
    Serial.println("");
    Serial.println("WiFi up AP");
    StartAPMode();
  }
  else {
    // Иначе удалось подключиться отправляем сообщение
    // о подключении и выводим адрес IP
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    //jsonWrite(configJson, "ip", WiFi.localIP().toString()); 
  }
}

bool StartAPMode() {
  IPAddress apIP(192, 168, 177, 1);
  IPAddress staticGateway(192, 168, 177, 1);
  IPAddress staticSubnet(255, 255, 255, 0);
  //jsonWrite(configJson, "ip", apIP.toString()); 
  // Отключаем WIFI
  WiFi.disconnect();
  // Меняем режим на режим точки доступа
  WiFi.mode(WIFI_AP);
  // Задаем настройки сети
  WiFi.softAPConfig(apIP, staticGateway, staticSubnet);
  //Включаем DNS
  dnsServer.start(53, "*", apIP);
  // Включаем WIFI в режиме точки доступа с именем и паролем
  // хронящихся в переменных _ssidAP _passwordAP
  //String _ssidAP = jsonRead(configSetup, "ssidAP");
  //String _passwordAP = jsonRead(configSetup, "passwordAP");
  WiFi.softAP(_ssidAP.c_str(), _passwordAP.c_str());
  return true;
}


void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
pinMode(rele, OUTPUT);
  delay(7);
  Serial.println("Start 1-WIFI");
  //Запускаем WIFI
  WIFIinit();


  ds.begin();//инициализация 18b20
  ds.setResolution(sensor_tOutK,10);//18b20 установка разрешения преобразования
  ds.setResolution(sensor_tOutN,10);//18b20 установка разрешения преобразования


  server.on("/search", [](){
    /*
    if(!server.authenticate(www_username, www_password))
      return server.requestAuthentication();
      */
      //getDeviceAddress();
      //_18b20search();
      //printAddress;
      String message="<head></head><body>";
      message+="<table>";
      for( i = 0; i < 2; i++) {
        ds.getAddress(Thermometer, i);
        byte t[8];
        t[i]= ds.getTempCByIndex(i);
        delay(200);
        message+=tr_td;
        message+="id=";
        message+=i;
        message+="|"+_td_td;
        //message+="<tr><td>";
        for (uint8_t ii = 0; ii < 8; ii++)
        {
          // zero pad the address if necessary
          message+=Thermometer[ii];
          message+="#";
        }
        //message+=Thermometer;
        //message+="nnn";
        //message+=addr[i];
        //message+="mmm";
        //message+=t[i];
        message+="|"+_td_td+"|"+_td_td;
        //const uint8_t tmp4=addr[i];
        //DeviceAddress tmp4={addr[i]};
        //message+=ds.getTempC(tmp4);
        message+=t[i];
        //message+="</td></tr>";
        message+=_td__tr;
        //message+="</br>";
  };
      message+="</table>";
      message+="</body></html>";
      server.send(200, "text/html", message);
  });

server.on("/", [](){
    /*
    if(!server.authenticate(www_username, www_password))
      return server.requestAuthentication();
      */
      signed char schet2;
      //schet2=schet;
      //if (schet==0){
      //  schet2=1;
      //}
      String message="<!doctype html>\n<html>\n<head>\n<meta charset=\"utf-8\">\n<meta http-equiv=\"refresh\" content=\"30;/\"> \n</head>\n";
    message+="<body>\n<div style=\"position:absolute;left:20px;top:19px;width:250px;height:24px;z-index:0;\">\n<span style=\"color:#000000;font-family:Arial;font-size:21px;\">";
    
      message+="t выхода из коллектора:";
      message+=String(g_tOutK);
      message+="<br> t выхода из накопителя:";
      message+=String(g_tOutN);
      message+="<br> накоплено энергии итого:";
      message+=String(g_Watt);
      message+="<br> за время(часы):";
      short tmp300=g_Time*g_TOpros/3600/1000;
      message+=String(tmp300);
      message+="<br> статус циркуляционного насоса:";
      message+=String(g_Nasos);
      message+="<br> t принудительного отключения:";
      message+=String(g_TBreakOff);
      message+=String(tmp_tOutK);
      message+=String(ds.isConnected(sensor_tOutK));
      message+=String(tmp2);
      //message+="\n";
     // message+=String(schet2);
      //message+="\n";
      //message+=String(kof);
      //message+="\n";
      //message+=String(tmp1);
      //message+="\n";
      //message+=String(rew);
      //message+="\n";
      //message+=String(razgon_tmp);
      //message+="\n";
      //message+=String(razgon);
      //message+="\n";
      //message+=String(ryj);
     // message+="\n";
     // message+=String(tBreakOffK);
     // message+="\n";
     // message+=String(tBreakOffIn);
     // message+="\n";
      //message+=String(tDeltaKCritical);
      ////message+="\n";
      //message+=String(tDeltaIn);
     // message+="\n";
      //message+=String(tOutCritical);
      //message+="\n";
      //message+=String(tDeltaKCritical_economy);
     // message+="\n";
     // message+=String(razgon);
     // message+="\n";
     // message+=String(negative);
     // message+="\n";
      //message+=String(zalp_per);
      message+="\n";
      message+="</span></div>\n</body>\n</html>";
      server.send(200, "text/html", message);
  });




//запускаем сервер
server.begin();


ts.add(0, g_TOpros, [&](void*) { f_1();}, nullptr, true);
 


}

void f_1(){
      // signed short  tmp_tOutK=0;
  //signed short tmp_tOutN=0;


  if (ds.isConnected(sensor_tOutK)){
    //#define ETS_INTR_LOCK() ets_intr_lock() //запрет прерываний
  ds.requestTemperaturesByAddress(sensor_tOutK);
  //
  delay(200);
  tmp_tOutK= (short) ds.getTempC(sensor_tOutK);
  
  //#define ETS_INTR_UNLOCK() ets_intr_unlock() //разрешение всех прерываний
  }

  if (ds.isConnected(sensor_tOutN)){
    //#define ETS_INTR_LOCK() ets_intr_lock() //запрет прерываний
  ds.requestTemperaturesByAddress(sensor_tOutN);
  //
  delay(200);
  tmp_tOutN= (short) ds.getTempC(sensor_tOutN);
  
  //#define ETS_INTR_UNLOCK() ets_intr_unlock() //разрешение всех прерываний
  }

  g_Time=g_Time+1;
  //signed short dt=0;



  if (g_Time<2){
    g_tOutN=tmp_tOutN;
    g_tOutK=tmp_tOutK;

  } else {
    //signed short tmp1=0;
    //signed short tmp2=0;
    tmp1=(g_tOutN+tmp_tOutN)/2;
    tmp2=(g_tOutK+tmp_tOutN)/2;
    g_tOutN=tmp1;
    g_tOutK=tmp2;

  };

  dt=g_tOutK-g_tOutN;
  if (dt<0){
    dt=0;
  };
  g_Watt=g_Watt+g_C*dt*g_M*g_TOpros/60/10/1000;

  if (dt<3){
    g_Nasos=false;
    digitalWrite(rele, LOW);
  } else {
    digitalWrite(rele, HIGH);
    g_Nasos=true;
  }
  if (g_Watt> 10446744073709 or g_Time>10446744073709){
    Serial.println("Resetting ESP");
      ESP.restart();
  }

}

void loop() {
  // put your main code here, to run repeatedly:

  ts.update();
  server.handleClient();
  yield();

}
