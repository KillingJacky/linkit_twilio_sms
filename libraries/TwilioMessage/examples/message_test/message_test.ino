
#include <LWiFi.h>
#include <LWiFiClient.h>
#include <LGPRS.h>
#include <LGPRSClient.h>
#include <Temboo.h>
#include <TwilioMessage.h>

#define TYPE "wifi"
//#define TYPE "gprs"

#define TEMBOO_ACCOUNT "jinyuanweng"
#define TEMBOO_APP_KEY_NAME "myFirstApp"
#define TEMBOO_APP_KEY_VALUE "ZRb5rUZpPSV0unCuBiXeYTDPqrExmWQd"
#define TWILIO_ACCOUNT_SID "AC0f6059279135e52c0636802c7085eb83"
#define TWILIO_AUTH_TOKEN "1743261260693926085fc2d4cbc4b0d3"

#define WIFI_AP "STU-EE"
#define WIFI_PASSWORD "stu-ee-2016"


TwilioMessageClass Message(TEMBOO_ACCOUNT, TEMBOO_APP_KEY_NAME, TEMBOO_APP_KEY_VALUE, 
                            TWILIO_ACCOUNT_SID, TWILIO_AUTH_TOKEN, TYPE);


int i = 0;


void setup()
{
    Serial.begin(115200);
    while(!Serial);
    Serial.println("Temboo Message Test ...");
    
    boolean connected = false;
    while(1)
    {
        Serial.println("Connecting");
        if(strcmp(TYPE, "wifi") == 0)
        {
            connected = Message.connect(WIFI_AP, WIFI_PASSWORD);
        }
        else if(strcmp(TYPE, "gprs") == 0)
        {
            connected = Message.connect();
        }
        
        if(connected)break;
        delay(1000);
    }
  

    Serial.println("Setup complete.\r\n");
    
    //Message.SendSMS("+12247231109", "+8615915323285", "Twilio send sms test: Hello World!");
    //delay(3000);
}

void loop()
{
    if(i < 10)
    {
        Serial.print("i = ");
        Serial.println(i ++);
        
        Message.GetLastMessageFromNumber("+8615915323285");
        //Message.ListMessages();
        //Message.GetMessage("SM7f1cd7e8e304fffdd904608aee995ee2");
        //Message.GetMessagesThatContain("Hello");
        //Message.GetLastMessageThatContains("Hello");
        
        while(Message.available())
        {
            char c = Message.read();
            Serial.print(c);
        }
        Message.close();
    }
    
    Serial.println("wait...\r\n");
    delay(5000);
} 