#if defined(ESP32)
#include <WiFi.h>
#include <FirebaseESP32.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#endif
#include "ESP32_MailClient.h"
#include "ESP32_MailClient.h"

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
#define API_KEY "3PYPKcuVlmcyEPHAOFh8s2Id5x4zqXcpiF0sQyWi"

/* 3. Define the RTDB URL */
#define DATABASE_URL "https://projettpsmartdevice-default-rtdb.firebaseio.com/" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app
#define FirebaseData fbdo;

#define FirebaseAuth auth;
#define FirebaseConfig config;
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
float t ;

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
  sendingMail();




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

 if (Firebase.ready()) 
  {
    //Firebase.setInt(fbdo, main, 5);
    Firebase.setInt(fbdo, "/Machine/temperature", temperature());
    delay(200);

    Serial.printf("Get int a--  %s\n", Firebase.getInt(fbdo, "/Machine/temperature") ? String(fbdo.to<int>()).c_str() : fbdo.errorReason().c_str());
     t=fbdo.to<float>();
 

  Serial.println();  
  Serial.print("temp:");
  Serial.print(t);  
  Serial.println();
  Serial.println("------------------");
  Serial.println();
  

  delay(2500);
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
void sendingMail() {
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
  smtpData.setMessage("<div style=\"color:#2f4468;\"><h1>Smart device project hi gadour!!!!</h1><p>- It's ismail from ESP 32</p></div>", true);
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
