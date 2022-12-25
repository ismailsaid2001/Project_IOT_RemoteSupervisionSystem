//projet_smart_device_groupe:ismail_said/iheb_abdelkader/jawher_jandoubi
#if defined(ESP32)
#include <WiFi.h>
#include <Wire.h>
#include <FirebaseESP32.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#endif
#include "ESP32_MailClient.h"
#include "ESP32_MailClient.h"
// I2C ADDRESS
#define MMA_ADDRESS   0x4C

//HTS Registers Addresses
#define MMA_MODE_REG       0x07
#define MMA_OUTX_REG       0x00
#define MMA_OUTY_REG       0x01
#define MMA_OUTZ_REG       0x02


//I2C0 PINS
#define SDA_Pin 21
#define SCL_Pin 22
// To send Email using Gmail use port 465 (SSL) and SMTP Server smtp.gmail.com
// YOU MUST ENABLE less secure app option https://myaccount.google.com/lesssecureapps?pli=1
#define emailSenderAccount    "ismailsaid570@gmail.com"    
#define emailSenderPassword   "onikqrxcdyhzkyji"
#define emailRecipient        "iheb30abdelkader@gmail.com"
#define smtpServer            "smtp.gmail.com"
#define smtpServerPort        465
#define emailSubject          "ESP32 Test"
SMTPData smtpData;

// Callback function to get the Email sending status
void sendingMail() ;
void sendCallback(SendStatus info);

#define LED_Blue 12
#define LED_Green 13
#define LED_Red 16

#define Button_Center 17
//Provide the token generation process info.
#include <addons/TokenHelper.h>

//Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

/* 1. Define the WiFi credentials */
#define WIFI_SSID "AP1"
#define WIFI_PASSWORD "ismail1234"
/* 2. Define the API Key */
#define API_KEY "YsxxTBgeg2rDEeSLkhiQ45t36M7IwFwULvmSIXqK"

/* 3. Define the RTDB URL */
#define DATABASE_URL "https://project1-f66fe-default-rtdb.firebaseio.com/" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
String main="";

#include "Wire.h"
#define TempSlave_Addr 0x48
//Registers Addresses 
#define temp_Reg 0x00
//pin used to select Slave 
#define SDA_Pin 21 
#define SCL_Pin 22 
//constants 
TwoWire MyI2cBus=TwoWire(0) ;
//variables 
float h ;
float pression ;
float humidite ;
float gaz ;
float t ;
float g ;
float p ;
float x;
float y;
float z;
void setup() {
Serial.begin(115200);
  delay(2000);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
   Serial.println();
  Serial.println("WiFi connected.");
  Serial.println();
  Serial.println("Preparing to send welcome email");
  Serial.println();
   // Set the SMTP Server Email host, port, account and password
  sendingWelcomeMail();
   //MMA configuration :
     while (!Serial);
     Serial.println("App Launched");       
     MyI2cBus.begin(SDA_Pin,SCL_Pin,400000) ;
     
      
     // enable MMA7760
         writeI2cReg(MMA_MODE_REG, 0x01);
         Serial.println("MMA7760 ENABLED");   
         








  /* Assign the api key (required) */
  config.api_key = API_KEY;

  config.database_url = DATABASE_URL;


  Firebase.begin(DATABASE_URL, API_KEY);

  //Comment or pass false value when WiFi reconnection will control by your code or third party library
 // Firebase.reconnectWiFi(true);

  Firebase.setDoubleDigits(5);
  Wire.begin() ;
  // put your setup code here, to run once:
  Serial.begin(115200) ;
  MyI2cBus.begin(SDA_Pin,SCL_Pin,400000) ;
}




void loop() {
  //lecture x,y,z
  Serial.println ("conversion OK ... Reading Accelerations");

          //Read Accelerations conversion results
          int8_t outX = (readI2cReg(MMA_OUTX_REG) <<2) ;
          Serial.print (" X axis MMA Output: ");Serial.println(outX);
          float accelX = (outX/4)*1.5/32;
          Serial.print("MMA7760 Accel  Conversion values  :");
          Serial.print (" X axis : ");Serial.println(accelX);

          
          int8_t outY = (readI2cReg(MMA_OUTY_REG)<<2)  ;
          Serial.print (" Y axis MMA Output: ");Serial.println(outY);
          float accelY = (outY/4)*1.5/32;          
          Serial.print("MMA7760 Accel Conversion values  :");
          Serial.print (" Y axis : ");Serial.println(accelY);
          
          int8_t outZ = (readI2cReg(MMA_OUTZ_REG)<<2) ;
          Serial.print (" Y axis MMA Output: ");Serial.println(outZ);
          float accelZ = (outZ/4)*1.5/32;        
          Serial.print("MMA7760 Accel Conversion values  :");
          Serial.print (" Z axis : ");Serial.println(accelZ);

          Serial.println("----------------------------");
          delay(1000);

  //pression 
  pression=random(1013,1100);
 //gaz 
  gaz=random(200,800);
 //humidité 
 humidite=random(20,80) ; 
 if (Firebase.ready()) 
  {
    //Firebase.setInt(fbdo, main, 5);
    Firebase.setInt(fbdo, "/Machine/temperature", temperature());
    Firebase.setInt(fbdo, "/Machine/pression", pression);
    Firebase.setInt(fbdo, "/Machine/gaz", gaz);
    Firebase.setInt(fbdo, "/Machine/humidite", humidite );
    Firebase.setInt(fbdo, "/Machine/Xvibration",accelX);
    Firebase.setInt(fbdo, "/Machine/Yvibration",accelY);
    Firebase.setInt(fbdo, "/Machine/Zvibration",accelZ);
    delay(1400);

    Serial.printf("Get float t--  %s\n", Firebase.getInt(fbdo, "/Machine/temperature") ? String(fbdo.to<float>()).c_str() : fbdo.errorReason().c_str());
     t=fbdo.to<float>();
     Serial.printf("Get float p--  %s\n", Firebase.getInt(fbdo, "/Machine/pression") ? String(fbdo.to<float>()).c_str() : fbdo.errorReason().c_str());
     p=fbdo.to<float>();
     Serial.printf("Get float g--  %s\n", Firebase.getInt(fbdo, "/Machine/gaz") ? String(fbdo.to<float>()).c_str() : fbdo.errorReason().c_str());
     g=fbdo.to<float>();
     Serial.printf("Get float h--  %s\n", Firebase.getInt(fbdo, "/Machine/humidite") ? String(fbdo.to<float>()).c_str() : fbdo.errorReason().c_str());
     h=fbdo.to<float>();
     
    Serial.printf("Get float x--  %s\n", Firebase.getInt(fbdo, "/Machine/Xvibration") ? String(fbdo.to<float>()).c_str() : fbdo.errorReason().c_str());
     x=fbdo.to<float>();
     Serial.printf("Get float  y--  %s\n", Firebase.getInt(fbdo, "/Machine/Yvibration") ? String(fbdo.to<float>()).c_str() : fbdo.errorReason().c_str());
     y=fbdo.to<float>();
     Serial.printf("Get float a--  %s\n", Firebase.getInt(fbdo, "/Machine/Zvibration") ? String(fbdo.to<float>()).c_str() : fbdo.errorReason().c_str());
     z=fbdo.to<float>();
  
  Serial.println();  
  Serial.print("temp:");
  Serial.print(t);  
  Serial.println();
  Serial.print("pression:");
  Serial.print(p);  
  Serial.println();
  Serial.print("gaz:");
  Serial.print(g);  
  Serial.println();
  Serial.print("humidite:");
  Serial.print(h);  
  Serial.println();
  Serial.println("------------------");
  Serial.println();
  Firebase.setInt(fbdo, "/Machine/temperature", temperature());
  if(h<25||h>70) {
    //HumidityWarning();
  }
  Firebase.setInt(fbdo, "/Machine/temperature", temperature());
  if(x>0.4||x<-0.4||y>0.4||y<-0.4) {
    //vibrationWarning();
  }
  Firebase.setInt(fbdo, "/Machine/temperature", temperature());
  if(g>600) {
    //GazWarning();
  }
  Firebase.setInt(fbdo, "/Machine/temperature", temperature());


  }

}
float  temperature (){
  MyI2cBus.beginTransmission(TempSlave_Addr) ;
  MyI2cBus.write(temp_Reg) ;
  MyI2cBus.endTransmission(false) ;
  MyI2cBus.requestFrom(TempSlave_Addr, 0x02) ;
  float temp=( ( (MyI2cBus.read()<<8)|(MyI2cBus.read()&0xE0 ) )>>5)*0.125 ;
  Wire.endTransmission(true) ;
  return temp ; 
}
void sendingWelcomeMail() {
  smtpData.setLogin(smtpServer, smtpServerPort, emailSenderAccount, emailSenderPassword);

  // For library version 1.2.0 and later which STARTTLS protocol was supported,the STARTTLS will be 
  // enabled automatically when port 587 was used, or enable it manually using setSTARTTLS function.
  //smtpData.setSTARTTLS(true);

  // Set the sender name and Email
  smtpData.setSender("ESP32", emailSenderAccount);

  // Set Email priority or importance High, Normal, Low or 1 to 5 (1 is highest)
  smtpData.setPriority("High");

  // Set the subject
  smtpData.setSubject(emailSubject);

  // Set the message with HTML format
  smtpData.setMessage("<div style=\"color:#2f4468;\"><h1>Hi dear customer ❤❤❤ ,with our solution your machine will be safe you will get notification with mails in case of danger !!!!</h1><p>- It's ismail from ESP 32</p></div>", true);
  // Set the email message in text format (raw)
  //smtpData.setMessage("Hello World! - Sent from ESP32 board", false);

  // Add recipients, you can add more than one recipient
  smtpData.addRecipient(emailRecipient);
  //smtpData.addRecipient("YOUR_OTHER_RECIPIENT_EMAIL_ADDRESS@EXAMPLE.com");

  smtpData.setSendCallback(sendCallback);

  //Start sending Email, can be set callback function to track the status
  if (!MailClient.sendMail(smtpData))
    Serial.println("Error sending Email, " + MailClient.smtpErrorReason());

  //Clear all data from Email object to free memory
  smtpData.empty();
  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

}
void vibrationWarning() {
  smtpData.setLogin(smtpServer, smtpServerPort, emailSenderAccount, emailSenderPassword);

  // For library version 1.2.0 and later which STARTTLS protocol was supported,the STARTTLS will be 
  // enabled automatically when port 587 was used, or enable it manually using setSTARTTLS function.
  //smtpData.setSTARTTLS(true);

  // Set the sender name and Email
  smtpData.setSender("MachineControlling_Project", emailSenderAccount);

  // Set Email priority or importance High, Normal, Low or 1 to 5 (1 is highest)
  smtpData.setPriority("High");

  // Set the subject
  smtpData.setSubject("Warning_Vibration");

  // Set the message with HTML format
  smtpData.setMessage("<div style=\"color:#2f4468;\"><h1> ⚠⚠⚠Warning!!!!⚠⚠⚠</h1><p>Your Machine is vibrating please check it out </p></div>", true);
  // Set the email message in text format (raw)
  //smtpData.setMessage("Hello World! - Sent from ESP32 board", false);

  // Add recipients, you can add more than one recipient
  smtpData.addRecipient(emailRecipient);
  //smtpData.addRecipient("YOUR_OTHER_RECIPIENT_EMAIL_ADDRESS@EXAMPLE.com");

  smtpData.setSendCallback(sendCallback);

  //Start sending Email, can be set callback function to track the status
  if (!MailClient.sendMail(smtpData))
    Serial.println("Error sending Email, " + MailClient.smtpErrorReason());

  //Clear all data from Email object to free memory
  smtpData.empty();
  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

}
void HumidityWarning() {
  smtpData.setLogin(smtpServer, smtpServerPort, emailSenderAccount, emailSenderPassword);

  // For library version 1.2.0 and later which STARTTLS protocol was supported,the STARTTLS will be 
  // enabled automatically when port 587 was used, or enable it manually using setSTARTTLS function.
  //smtpData.setSTARTTLS(true);

  // Set the sender name and Email
  smtpData.setSender("MachineControlling_Project", emailSenderAccount);

  // Set Email priority or importance High, Normal, Low or 1 to 5 (1 is highest)
  smtpData.setPriority("High");

  // Set the subject
  smtpData.setSubject("Warning_Humidity");
  
  // Set the message with HTML format
  smtpData.setMessage("<div style=\"color:#2f4468;\"><h1> ⚠⚠⚠Warning!!!!⚠⚠⚠</h1><p>Your Machine's humidity is not in a good range please check it out</p></div>", true);
  // Set the email message in text format (raw)
  //smtpData.setMessage("Hello World! - Sent from ESP32 board", false);

  // Add recipients, you can add more than one recipient
  smtpData.addRecipient(emailRecipient);
  //smtpData.addRecipient("YOUR_OTHER_RECIPIENT_EMAIL_ADDRESS@EXAMPLE.com");

  smtpData.setSendCallback(sendCallback);

  //Start sending Email, can be set callback function to track the status
  if (!MailClient.sendMail(smtpData))
    Serial.println("Error sending Email, " + MailClient.smtpErrorReason());

  //Clear all data from Email object to free memory
  smtpData.empty();
  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

}
void GazWarning() {
  smtpData.setLogin(smtpServer, smtpServerPort, emailSenderAccount, emailSenderPassword);

  // For library version 1.2.0 and later which STARTTLS protocol was supported,the STARTTLS will be 
  // enabled automatically when port 587 was used, or enable it manually using setSTARTTLS function.
  //smtpData.setSTARTTLS(true);

  // Set the sender name and Email
  smtpData.setSender("MachineControlling_Project", emailSenderAccount);

  // Set Email priority or importance High, Normal, Low or 1 to 5 (1 is highest)
  smtpData.setPriority("High");

  // Set the subject
  smtpData.setSubject("Warning_Gaz");
  // Set the message with HTML format
  smtpData.setMessage("<div style=\"color:#2f4468;\"><h1> ⚠⚠⚠Warning!!!!⚠⚠⚠</h1><p>Your Machine's under gaz leak please check it out !!!!!</p></div>", true);
  // Set the email message in text format (raw)
  //smtpData.setMessage("Hello World! - Sent from ESP32 board", false);

  // Add recipients, you can add more than one recipient
  smtpData.addRecipient(emailRecipient);
  //smtpData.addRecipient("YOUR_OTHER_RECIPIENT_EMAIL_ADDRESS@EXAMPLE.com");

  smtpData.setSendCallback(sendCallback);

  //Start sending Email, can be set callback function to track the status
  if (!MailClient.sendMail(smtpData))
    Serial.println("Error sending Email, " + MailClient.smtpErrorReason());

  //Clear all data from Email object to free memory
  smtpData.empty();
  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

}
void sendCallback(SendStatus msg) {
  // Print the current status
  Serial.println(msg.info());

  // Do something when complete
  if (msg.success()) {
    Serial.println("----------------");
  }
}
void writeI2cReg(uint8_t RegAddr, uint8_t Value){
    
          MyI2cBus.beginTransmission(MMA_ADDRESS);
         MyI2cBus.write(RegAddr);
         MyI2cBus.write(Value);
          if (MyI2cBus.endTransmission(true)!=0){
            Serial.println ("problem writing to I2C device");
            exit(0);
          }
   }


uint8_t readI2cReg(uint8_t RegAddr){    
          MyI2cBus.beginTransmission(MMA_ADDRESS);
          MyI2cBus.write(RegAddr);
           if (MyI2cBus.endTransmission(false)){ //if !=0
                Serial.println ("Problem writing without stop"); 
                exit(0);
            }
      MyI2cBus.requestFrom(MMA_ADDRESS,0x01);
        return(MyI2cBus.read());
   }
