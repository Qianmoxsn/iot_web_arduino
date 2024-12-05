/*
 * gprs.cpp
 * A library for SeeedStudio seeeduino GPRS shield
 *
 * Copyright (c) 2013 seeed technology inc.
 * Author        :   lawliet zou
 * Create Time   :   Dec 2013
 * Change Log    :
 *
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <stdio.h>
#include "gprs.h"

int GPRS::init(void)
{
#if 0
    for(int i = 0; i < 2; i++){
        sendCmd("AT\r\n");
        delay(100);
    }
    sendCmd("AT+CFUN=1\r\n");
    if(0 != checkSIMStatus()) {
        ERROR("ERROR:checkSIMStatus");
        return -1;
    }
    return 0;

#endif
    if(sendCmdAndWaitForResp("AT\r\n","OK\r\n",DEFAULT_TIMEOUT*3)){
      return -1;
    }
    /*if(sendCmdAndWaitForResp("AT+CFUN=1\r\n","OK\r\n",DEFAULT_TIMEOUT*3)){
      return -1;
    }
    */
	
    if(checkSIMStatus()) {
		  return -1;
    }
     /*if(sendCmdAndWaitForResp("AT+CIURC=0\r\n","OK\r\n",DEFAULT_TIMEOUT*3)){
      return -1;
    }
    */
    return 0;

}
int GPRS::sleep(void){
	if(sendCmdAndWaitForResp("AT+CSCLK=1\r\n","OK\r\n",DEFAULT_TIMEOUT*3)){
      return -1;
    }
	return 0;
}

bool GPRS::join(const char *apn, const char *userName, const char *passWord)
{
	while (serialSIM800.available()) {
    	serialSIM800.read();
	}
    char cmd[64];
    char ipAddr[32];
    char gprsBuffer[32];
  /* int i=0;
    char buffer[28];
    //Select multiple connection
    //sim900_check_with_csendTCPDatamd("AT+CIPMUX=1\r\n","OK",DEFAULT_TIMEOUT,CMD);
     sendCmd("AT+CGATT?\r\n");
     delay(1000);
     while(serialSIM800.available()>0){
		char c = serialSIM800.read();
        if (c == '\r' || c == '\n') c = '$';                            
            buffer[i++] = c;
			Serial.println(buffer);
		}
		delay(1000);

		if(NULL!=strstr(buffer,"0"))
		{
			Serial.print("hello:");
			sendCmd("AT+CGATT=1\r\n");
            delay(1000);

}
*/


//sendCmd("AT+CGATT=1\r\n");    
		
delay(2000);

while (serialSIM800.available()) {
    		serialSIM800.read();
		}

    cleanBuffer(ipAddr,32);
    sendCmd("AT+CIFSR\r\n");    
    readBuffer(ipAddr,32,2);
	Serial.print("ipAddr:");
    Serial.println(ipAddr);


          delay(1000);
    // If no IP address feedback than bring up wireless 
    if( NULL != strstr(ipAddr, "ERROR") )
    {
        //if( 0 != sendCmdAndWaitForResp("AT+CSTT?\r\n", apn, DEFAULT_TIMEOUT) )
        //{
			
            /*sendCmd("AT+CSTT=\"");
            sendCmd(apn);
            sendCmd("\",\"");
            sendCmd(userName);
            sendCmd("\",\"");
            sendCmd(passWord);   */     
            sendCmdAndWaitForResp("AT+CGATT=1\r\n","OK\r\n",DEFAULT_TIMEOUT*3);
			sendCmdAndWaitForResp("AT+CGDCONT=1,\"IP\",\"CMNET\"\r\n","OK\r\n",DEFAULT_TIMEOUT*3);
			sendCmdAndWaitForResp("AT+CREG=1\r\n","OK\r\n",DEFAULT_TIMEOUT*3);
			sendCmdAndWaitForResp("AT+CGACT=1,1\r\n","OK\r\n",DEFAULT_TIMEOUT*3);
        //}
      
        //Brings up wireless connection
        //sendCmd("AT+CIICR\r\n");
        delay(5000);
		while (serialSIM800.available()) {
    		serialSIM800.read();
		}
		delay(1000);
        //Get local IP address
        cleanBuffer(ipAddr,32);
        sendCmd("AT+CIFSR\r\n");
        readBuffer(ipAddr,32,2); 
		Serial.println(ipAddr);       
    }  
 
#if 0    
    Serial.print("ipAddr: ");
    Serial.println(ipAddr);
#endif

    if(NULL != strstr(ipAddr,"AT+CIFSR")) {        
        _ip = str_to_ip(ipAddr+12);
        if(_ip != 0) {
            return true;
        }
    }
    return false;
}

uint32_t GPRS::str_to_ip(const char* str)
{
    uint32_t ip = 0;
    char *p = (char*)str;
    
    for(int i = 0; i < 4; i++) {
        ip |= atoi(p);
        p = strchr(p, '.');
        if (p == NULL) {
            break;
        }
        if(i < 3) ip <<= 8;
        p++;
    }
    return ip;
}

//HACERR lo de la IP gasta muuuucho espacio (ver .h y todo esto)
char* GPRS::getIPAddress()
{
    uint8_t a = (_ip>>24)&0xff;
    uint8_t b = (_ip>>16)&0xff;
    uint8_t c = (_ip>>8)&0xff;
    uint8_t d = _ip&0xff;

    snprintf(ip_string, sizeof(ip_string), "%d.%d.%d.%d", a,b,c,d);
    return ip_string;
}

//add by @zdz
//to return identify
String GPRS::getIdentify()
{
 
//int i=0;
//char buffer[30];
    unsigned long timerStart,timerEnd;
    timerStart = millis();

	while(serialSIM800.available()>0){
		serialSIM800.read();
		delay(100);
	}
	sendCmd("AT+GSN\r\n");
	String identify = "";
	delay(2000);
	bool i = false;
	while(serialSIM800.available()>0){

        char c = serialSIM800.read();
        if (c == '\r' || c == '\n') c = '$';                            
           // buffer[i++] = c;
			//Serial.println(buffer);


		//char inChar = serialSIM800.read();
		if(identify.length() >= 15 && i)
			break;
		identify += (char)c;
		//Serial.println(identify);
		if(identify == "AT+GSN$$$$"){
			identify = "";
			i = true;
		}
		delay(100);
   //if(timerEnd - timerStart > 10000 * DEFAULT_TIMEOUT) {
     //       break;
     //   }
}
	while(serialSIM800.available()>0){
		serialSIM800.read();
		delay(100);
	}
	return identify;
}

String GPRS::getTime(){
return sendCmdAndWaitForRespTime("AT+CCLK?\r\n",DEFAULT_TIMEOUT);
  
}

void GPRS::newProofTime(){
  String time1=sendCmdAndWaitForRespTime("AT+CCLK?\r\n",DEFAULT_TIMEOUT);
  Serial.println(time1);   
 String time2=newBeiJingTime(time1);
  char time3[20];
//time3=time2.c_str(); 
  strcpy(time3,time2.c_str());
   sendCmd("AT+CCLK=\"");
   sendCmd(time3);
   sendCmd("\"\r\n");
   delay(1000);
}

void GPRS::proofTime(){
	

String gpsTime;         //UTC时间，本初子午线经度0度的时间，和北京时间差8小时
String beiJingTime;  
char nmeaSentence[68];   //北京时间
char* beijing;
sendCmd("AT+GPS=1\r\n");
delay(5000);
//sendCmd("AT+AGPS=1\r\n");
//delay(20000);
sendCmd("AT+GPSRD=1\r\n");
delay(5000);
for (unsigned long start = millis(); millis() - start < 20000;)  //一秒钟内不停扫描GPS信息
  {
    while (serialSIM800.available())   //串口获取到数据开始解析
    {
      char c = serialSIM800.read();    //读取一个字节获取的数据

      switch(c)                 //判断该字节的值
      {
      case '$':                 //若是$，则说明是一帧数据的开始
        serialSIM800.readBytesUntil('*', nmeaSentence, 67);        //读取接下来的数据，存放在nmeaSentence字符数组中，最大存放67个字节
        Serial.println(nmeaSentence);
        
        gpsTime = parseGprmcTime(nmeaSentence);//获取GPS时间

      if(gpsTime > "")      //当不是空时候打印输出
        {
          
          beiJingTime = getBeiJingTime(gpsTime); 
         // break;   //获取北京时间 
               
        }   
        }   
    }

   // if(beiJingTime!="")
   //  break;


}

//Serial.println(beiJingTime);
beijing=beiJingTime.c_str();



/*for(int i=0;i<20;i++){
    beijing[i]='\0';
    beijing[i]=beiJingTime[i];
}
*/

sendCmd("AT+CCLK=\"");
sendCmd(beijing);
sendCmd("\"\r\n");


/*while(serialSIM800.available()){
        Serial.write(serialSIM800.read());
        delay(10);
}
*/

delay(1000);

sendCmd("AT+GPSRD=0\r\n");
delay(100);
sendCmd("AT+GPS=0\r\n");
//delay(100);
//sendCmd("AT+AGPS=0\r\n");
delay(5000);

}


String GPRS::getLocation(){

sendCmd("AT+GPS=1\r\n");
delay(5000);
//sendCmd("AT+AGPS=1\r\n");
//delay(20000);
sendCmd("AT+GPSRD=1\r\n");
delay(2000);

char nmeaSentence[68]="";  
  String loc = "";
  // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 10000;)  //一秒钟内不停扫描GPS信息
  {
    while (serialSIM800.available()) //串口获取到数据开始解析
    {
      switch(serialSIM800.read())         //判断该字节的值
      {
      case '$':         //若是$，则说明是一帧数据的开始
        serialSIM800.readBytesUntil('*', nmeaSentence, 67); 
        Serial.println(nmeaSentence);    //读取接下来的数据，存放在nmeaSentence字符数组中，最大存放67个字节
        if(parseGprmcLat1(nmeaSentence) > "")   //当不是空时候打印输出
          {
            loc += parseGprmcLat1(nmeaSentence);
            loc += ";";
          }

        if(parseGprmcLon1(nmeaSentence) > "")    //当不是空时候打印输出
        {
           loc += parseGprmcLon1(nmeaSentence);
        }
      }
    }
    if(loc != ""){
      sendCmd("AT+GPSRD=0\r\n");
      delay(100);	
      sendCmd("AT+GPS=0\r\n");
      delay(100);
      sendCmd("AT+AGPS=0\r\n");
      delay(2000);
      //Serial.println("RIGHT");
      return loc;
    }
  }

  sendCmd("AT+GPSRD=0\r\n");
  delay(100);
  sendCmd("AT+GPS=0\r\n");
  delay(100);
  //sendCmd("AT+AGPS=0\r\n");
 // delay(2000);
  return "0000.00000;00000.00000";
}


void GPRS::serialListen(){
	serialSIM800.listen();
}


int GPRS::checkSIMStatus(void)
{
    char gprsBuffer[100];
    int count = 0;
    cleanBuffer(gprsBuffer,100);
    while(count < 3) {
        sendCmd("AT+CPIN?\r\n");
        readBuffer(gprsBuffer,100,DEFAULT_TIMEOUT);
        if((NULL != strstr(gprsBuffer,"+CPIN:READY"))) {
            break;
        }
        count++;
        delay(300);
    }
    if(count == 3) {
        return -1;
    }
    return 0;
}

int GPRS::networkCheck(void)
{
    delay(1000);
    if(0 != sendCmdAndWaitForResp("AT+CGREG?\r\n","+CGREG: 0,1",DEFAULT_TIMEOUT*3)) {
        ERROR("ERROR:CGREG");
        return -1;
    }
    delay(1000);
    if(0 != sendCmdAndWaitForResp("AT+CGATT?\r\n","+CGATT: 1",DEFAULT_TIMEOUT)) {
        ERROR("ERROR:CGATT");
        return -1;
    }
    return 0;
}

int GPRS::sendSMS(char *number, char *data)
{
    char cmd[32];
    if(0 != sendCmdAndWaitForResp("AT+CMGF=1\r\n", "OK", DEFAULT_TIMEOUT)) { // Set message mode to ASCII
        ERROR("ERROR:CMGF");
        return -1;
    }
    delay(500);
    snprintf(cmd, sizeof(cmd),"AT+CMGS=\"%s\"\r\n", number);
    if(0 != sendCmdAndWaitForResp(cmd,">",DEFAULT_TIMEOUT)) {
        ERROR("ERROR:CMGS");
        return -1;
    }
    delay(1000);
    serialSIM800.write(data);
    delay(500);
    sendEndMark();
    return 0;
}

int GPRS::readSMS(int messageIndex, char *message,int length)
{
    int i = 0;
    char gprsBuffer[100];
    char cmd[16];
    char *p,*s;

    sendCmdAndWaitForResp("AT+CMGF=1\r\n","OK",DEFAULT_TIMEOUT);
    delay(1000);
    sprintf(cmd,"AT+CMGR=%d\r\n",messageIndex);
    serialSIM800.write(cmd);
    cleanBuffer(gprsBuffer,100);
    readBuffer(gprsBuffer,100,DEFAULT_TIMEOUT);

    if(NULL != ( s = strstr(gprsBuffer,"+CMGR"))){
        if(NULL != ( s = strstr(gprsBuffer,"+32"))){
            p = s + 6;
            while((*p != '$')&&(i < length-1)) {
                message[i++] = *(p++);
            }
            message[i] = '\0';
        }
    }
    return 0;
}
int GPRS::rendMS(char *message)
{
  return readBuffer(message,100,DEFAULT_TIMEOUT);
}

int GPRS::deleteSMS(int index)
{
    char cmd[16];
    snprintf(cmd,sizeof(cmd),"AT+CMGD=%d\r\n",index);
    sendCmd(cmd);
    return 0;
}

int GPRS::callUp(char *number)
{
    char cmd[24];
    if(0 != sendCmdAndWaitForResp("AT+COLP=1\r\n","OK",5)) {
        ERROR("COLP");
        return -1;
    }
    delay(1000);
    sprintf(cmd,"\r\nATD%s;\r\n", number);
    serialSIM800.write(cmd);
    return 0;
}

int GPRS::answer(void)
{
    serialSIM800.write("ATA\r\n");
    return 0;
}

int GPRS::connectTCP(const char *ip, int port)
{
	while (serialSIM800.available()) {
    	serialSIM800.read();
	}
	delay(100);
    char cipstart[50];
    sprintf(cipstart, "AT+CIPSTART=\"TCP\",\"%s\",%d\r\n", ip, port);
    if(0 != sendCmdAndWaitForResp(cipstart, "CONNECT OK", 5*DEFAULT_TIMEOUT)) {// connect tcp
        ERROR("ERROR:CIPSTART");
        return -1;
    }

    return 0;
}
int GPRS::sendTCPData(char *data)
{
    char cmd[32];
    int len = strlen(data); 
    snprintf(cmd,sizeof(cmd),"AT+CIPSEND=%d\r\n",len);
    if(0 != sendCmdAndWaitForResp(cmd,">",2*DEFAULT_TIMEOUT)) {
        ERROR("ERROR:CIPSEND");
        return -1;
    }
        
    //sendCmd(data);
   // delay(1000);    
    if(0 != sendCmdAndWaitForResp(data,"OK",2*DEFAULT_TIMEOUT)) {
        ERROR("ERROR:SendTCPData");
        return -1;
    }     
    
    return 0;
}

int GPRS::closeTCP(void)
{
    sendCmd("AT+CIPCLOSE\r\n");
    return 0;
}

int GPRS::shutTCP(void)
{
    sendCmd("AT+CIPSHUT\r\n");
    return 0;
}

void GPRS::reset(void){
 sendCmd("AT+CFUN=1\r\n");
  Serial.println("RESET");
}


String GPRS::parseGprmcTime(String s)
{
  int pLoc = 0; //paramater location pointer
  int lEndLoc = 0; //lat parameter end location
  int dEndLoc = 0; //direction parameter end location
  String gpsTime;
  String gpsDate;

  /*make sure that we are parsing the GPRMC string. 
   Found that setting s.substring(0,5) == "GPRMC" caused a FALSE.
   There seemed to be a 0x0D and 0x00 character at the end. */
  if(s.substring(0,4) == "GPRM")
  {
    //Serial.println(s);
    for(int i = 0; i < 2; i++)
    {
      if(i < 1) 
      {
        pLoc = s.indexOf(',', pLoc+1);
        /*Serial.print("i < 8, pLoc: ");
         Serial.print(pLoc);
         Serial.print(", ");
         Serial.println(i);*/
      }
      else
      {
        lEndLoc = s.indexOf(',', pLoc+1);
        gpsTime = s.substring(pLoc+1, lEndLoc);
        /*Serial.print("i = 8, pLoc: ");
         Serial.println(pLoc);
         Serial.print("lEndLoc: ");
         Serial.println(lEndLoc);*/
      }
    }
}
   pLoc = 0; //paramater location pointer
  lEndLoc = 0; //lat parameter end location
  dEndLoc = 0; //direction parameter end location
    if(s.substring(0,4) == "GPRM")
  {
    //Serial.println(s);
    for(int j = 0; j < 10; j++)
    {
      if(j < 9) 
      {
        pLoc = s.indexOf(',', pLoc+1);
        /*Serial.print("i < 8, pLoc: ");
         Serial.print(pLoc);
         Serial.print(", ");
         Serial.println(i);*/
      }
      else
      {
        lEndLoc = s.indexOf(',', pLoc+1);
        gpsDate = s.substring(pLoc+1, lEndLoc);
        
         Serial.println(gpsTime);
          Serial.println(gpsDate);
        /*Serial.print("i = 8, pLoc: ");
         Serial.println(pLoc);
         Serial.print("lEndLoc: ");
         Serial.println(lEndLoc);*/
      }
    }
}



    return gpsTime+gpsDate; 
  
}

String GPRS::newBeiJingTime(String s)
{
   Serial.println(s);
   int hour = s.substring(9,11).toInt();
   //int minute = s.substring(2,4).toInt();
   //int second = s.substring(4,6).toInt();
   int day = s.substring(6,8).toInt(); 
   //int month = s.substring(12,14).toInt();
   //int year = s.substring(14,16).toInt();
   String minute = s.substring(12,14);
   String second = s.substring(15,17);
   String month = s.substring(3,5);
   String year = s.substring(0,2);
   int zone= s.substring(18,20).toInt();
   //Serial.println(zone);
    hour += zone;
   //Serial.println(hour);
  if(hour > 24)
  {
    hour -= 24;
    day=day+1;
 }
 
 String a=String(hour);
 String b=String(day);
// Serial.println(a);



 //int len1=hour.length();
 //int len2=day.length();
 if(a.length()==1)
    a="0"+a;
 if(b.length()==1)
    b="0"+b;
 //Serial.println(a);
 //Serial.println(b);

 //s = String(year)+"/"+String(month)+"/"+String(b)+","+ String(a) +":"+String(minute) +":"+ String(second)+"+00";
  String c =year+"/"+month+"/"+b+","+a+":"+minute+":"+second+"+00";
  Serial.println(c);
    return c;
   
}

String GPRS::getBeiJingTime(String s)
{
   int hour = s.substring(0,2).toInt();
   //int minute = s.substring(2,4).toInt();
   //int second = s.substring(4,6).toInt();
   int day = s.substring(10,12).toInt(); 
   //int month = s.substring(12,14).toInt();
   //int year = s.substring(14,16).toInt();
   String minute = s.substring(2,4);
   String second = s.substring(4,6);
   String month = s.substring(12,14);
   String year = s.substring(14,16);
   

  // int day =1;
  // int hour=0;
 //   hour += 8;
 //   //Serial.println(hour);
 //  if(hour > 24)
 //  {
 //    hour -= 24;
 //    day=day+1;
 // }
 
 String a=String(hour);
 String b=String(day);
// Serial.println(a);



 //int len1=hour.length();
 //int len2=day.length();
 if(a.length()==1)
    a="0"+a;
 if(b.length()==1)
    b="0"+b;
 Serial.println(a);
 Serial.println(b);


s = String(year)+"/"+String(month)+"/"+String(b)+","+ String(a) +":"+String(minute) +":"+ String(second)+"+08";
  Serial.println(s);


  return s;
}

String GPRS::parseGprmcLat(String s)
{
  int pLoc = 0; //paramater location pointer
  int lEndLoc = 0; //lat parameter end location
  int dEndLoc = 0; //direction parameter end location
  String lat = "";
  /*make sure that we are parsing the GPRMC string. 
   Found that setting s.substring(0,5) == "GPRMC" caused a FALSE.
   There seemed to be a 0x0D and 0x00 character at the end. */
  if(s.substring(0,4) == "GPRM")
  {
	  
    //Serial.println(s);
    for(int i = 0; i < 5; i++)
    {
		
      if(i < 3) 
      {
        pLoc = s.indexOf(',', pLoc+1);
        /*Serial.print("i < 3, pLoc: ");
         Serial.print(pLoc);
         Serial.print(", ");
         Serial.println(i);*/
      }
      if(i == 3)
      {
        lEndLoc = s.indexOf(',', pLoc+1);
         for(int j= pLoc+1;j<lEndLoc;j++)
			lat += s[j];
        //lat = s.substring(pLoc+1, lEndLoc);
        /*Serial.print("i = 3, pLoc: ");
         Serial.println(pLoc);
         Serial.print("lEndLoc: ");
         Serial.println(lEndLoc);*/
      }
	  /*
      if( i == 4)
      {
        dEndLoc = s.indexOf(',', lEndLoc+1);
        //lat = lat + ";" + s.substring(lEndLoc+1, dEndLoc);
		lat += ";";
		lat += s.substring(lEndLoc+1, dEndLoc);
        /*Serial.print("i = 4, lEndLoc: ");
         Serial.println(lEndLoc);
         Serial.print("dEndLoc: ");
         Serial.println(dEndLoc);
		 //Serial.println(lat);
      }*/
    }
    return lat; 
  }
}

//Parse GPRMC NMEA sentence data from String
//String must be GPRMC or no data will be parsed
//Return Longitude
String GPRS::parseGprmcLon(String s)
{
  int pLoc = 0; //paramater location pointer
  int lEndLoc = 0; //lat parameter end location
  int dEndLoc = 0; //direction parameter end location
  String lon = "";

  /*make sure that we are parsing the GPRMC string. 
   Found that setting s.substring(0,5) == "GPRMC" caused a FALSE.
   There seemed to be a 0x0D and 0x00 character at the end. */
  if(s.substring(0,4) == "GPRM")
  {
    //Serial.println(s);
    for(int i = 0; i < 7; i++)
    {
      if(i < 5) 
      {
        pLoc = s.indexOf(',', pLoc+1);
        /*Serial.print("i < 3, pLoc: ");
         Serial.print(pLoc);
         Serial.print(", ");
         Serial.println(i);*/
      }
      if(i == 5)
      {
        lEndLoc = s.indexOf(',', pLoc+1);
        for(int j=pLoc+1; j<lEndLoc;j++)
			lon += s[j];
        //lon = s.substring(pLoc+1, lEndLoc);
        /*Serial.print("i = 3, pLoc: ");
         Serial.println(pLoc);
         Serial.print("lEndLoc: ");
         Serial.println(lEndLoc);*/
      }
	  /*
      if(i == 6)
      {
        dEndLoc = s.indexOf(',', lEndLoc+1);
        //lon = lon + ";" + s.substring(lEndLoc+1, dEndLoc);
		lon += ";" ;
		lon += s.substring(lEndLoc+1, dEndLoc);
        /*Serial.print("i = 4, lEndLoc: ");
         Serial.println(lEndLoc);
         Serial.print("dEndLoc: ");
         Serial.println(dEndLoc);
      }*/
    }
    return lon; 
  }
}
String GPRS::parseGprmcLat1(char* s){
  int pLoc;
  int lEndLoc;
  int dEndLoc = 0;
  int j=0;
  String lat = "";
  if(strstr(s,"GPRM")){
    //Serial.println(s);
    for(int i=0;i < 5; i++){
      if(i<3){
        for(;j<68;j++){
          if(s[j]==','){
            pLoc = j;
            j++;
            break;
            Serial.println("pLoc finish");
          }
        }
      }
      if(i==3){
        for(;j<68;j++){
          if(s[j]==','){
            lEndLoc=j;
            break;
          }
        }
      }
    }
    //Serial.print("pLoc:");
    //Serial.println(pLoc);
    //Serial.print("lEndLoc:");
    //Serial.println(lEndLoc);
    for(int k=pLoc+1;k<lEndLoc;k++)
      lat += s[k];
    return lat;
  }
}

String GPRS::parseGprmcLon1(char* s){
  int pLoc;
  int lEndLoc;
  int dEndLoc = 0;
  int j=0;
  String lon = "";
  if(strstr(s,"GPRM")){
    //Serial.println(s);
    for(int i=0;i < 7; i++){
      if(i<5){
        for(;j<68;j++){
          if(s[j]==','){
            pLoc = j;
            j++;
            break;
            Serial.println("pLoc finish");
          }
        }
      }
      if(i==5){
        for(;j<68;j++){
          if(s[j]==','){
            lEndLoc=j;
            break;
          }
        }
      }
    }
    for(int k=pLoc+1;k<lEndLoc;k++)
      lon += s[k];
    //Serial.print("lon:");
    //Serial.println(lon);
    return lon;
  }
}
