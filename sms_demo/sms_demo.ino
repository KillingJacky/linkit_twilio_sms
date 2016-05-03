
#include <Wire.h>
#include <Seeed_OLEDDisplay.h>
#include <LWiFi.h>
#include <LWiFiClient.h>
#include <Temboo.h>
#include <LTask.h>
#include <LFlash.h>
#include <LSD.h>
#include <LStorage.h>


#define Drv LFlash


LFile myFile;
LWiFiClient client;
OLEDDisplay Display;


char bufferTemp[2048];

char twilio_account_sid[40];
char twilio_auth_token[40];
char twilio_number[20];

char temboo_account[40];
char temboo_app_name[40];
char temboo_app_key[40];

char wifi_ap[40];
char wifi_password[40];

#define MESSAGE_CONTENT "I am LinkItOne, send message by Twilio."
char message_sid[40];
char message_from[40];
char message_body[200];


void getAccountInfo(char *buffer);
void loadConfigfile(void);
char getTheLastMessage(void);
void sendMessage(void);


void setup()
{
    Serial.begin(115200);
    //while(!Serial);
    Serial.println("Temboo Message Test ...");
    
    Display.Init();
    Display.setGrayLevel(12);
    
    loadConfigfile();
    
    boolean connected = false;
    while(1)
    {
        Serial.println("Connecting");
        DisplayMessage(0, "Connecting", 10);
        connected = LWiFi.connect(wifi_ap, LWiFiLoginInfo(LWIFI_WPA, wifi_password));
        if(connected)break;
        delay(1000);
    }
    Serial.println("Wifi setup complete.\r\n");
    DisplayMessage(0, "Wifi Ok   ", 10);
    
    getTheLastMessage();
    Serial.println("Read message sid first.\r\n");
    DisplayMessage(0, "Start     ", 10);
}

void loop()
{       
    if(getTheLastMessage())
    {
        Serial.print("New message from:");
        Serial.println(message_from);
        Serial.print("Message content is:");
        Serial.println(message_body);
        Display.clearDisplay();
        DisplayMessage(0, "New From:", 9);
        DisplayMessage(1, message_from, strlen(message_from));
        DisplayMessage(3, "Content:", 8);
        DisplayMessage(4, message_body, strlen(message_body));
        
        sendMessage();
    }

    Serial.println("wait...\r\n");
    delay(5000);
}

void loadConfigfile(void)
{
    int i = 0;
    
    Drv.begin();
    
    myFile = Drv.open("config.txt");
    
    if(myFile)
    {
        Serial.println("config.txt:");
        myFile.seek(0);
        while(myFile.available())
        {           
            bufferTemp[i ++] = myFile.read();
            if(i >= sizeof(bufferTemp))break;
        }
        myFile.close();
    }
    else
    {
        Serial.println("error opening config.txt");
    }

    Serial.println(bufferTemp);
    
    getAccountInfo(bufferTemp);
}

void getAccountInfo(char *buffer)
{
    int i;
    char *ptr;
    
    ptr = strstr(buffer, "TWILIO_ACCOUNT_SID:");
    if(ptr == NULL){Serial.print("TWILIO_ACCOUNT_SID get error.\r\n");while(1);}
    i = 0;
    while(1)
    {
        twilio_account_sid[i] = *(ptr + 19 + i);
        i ++;
        if(i >= sizeof(twilio_account_sid)){Serial.print("TWILIO_ACCOUNT_SID is to long.\r\n");while(1);}
        if(*(ptr + 19 + i) == '\r' && *(ptr + 19 + i + 1) == '\n')break;
    }
    twilio_account_sid[i] = '\0';
    //Serial.println(twilio_account_sid);
    
    ptr = strstr(buffer, "TWILIO_AUTH_TOKEN:");
    if(ptr == NULL){Serial.print("TWILIO_AUTH_TOKEN get error.\r\n");while(1);}
    i = 0;
    while(1)
    {
        twilio_auth_token[i] = *(ptr + 18 + i);
        i ++;
        if(i >= sizeof(twilio_auth_token)){Serial.print("TWILIO_AUTH_TOKEN is to long.\r\n");while(1);}
        if(*(ptr + 18 + i) == '\r' && *(ptr + 18 + i + 1) == '\n')break;
    }
    twilio_auth_token[i] = '\0';
    //Serial.println(twilio_auth_token);

    ptr = strstr(buffer, "TWILIO_NUMBER:");
    if(ptr == NULL){Serial.print("TWILIO_NUMBER get error.\r\n");while(1);}
    i = 0;
    while(1)
    {
        twilio_number[i] = *(ptr + 14 + i);
        i ++;
        if(i >= sizeof(twilio_number)){Serial.print("TWILIO_NUMBER is to long.\r\n");while(1);}
        if(*(ptr + 14 + i) == '\r' && *(ptr + 14 + i + 1) == '\n')break;
    }
    twilio_number[i] = '\0';
    //Serial.println(twilio_number);
    
    ptr = strstr(buffer, "TEMBOO_ACCOUNT:");
    if(ptr == NULL){Serial.print("TEMBOO_ACCOUNT get error.\r\n");while(1);}
    i = 0;
    while(1)
    {
        temboo_account[i] = *(ptr + 15 + i);
        i ++;
        if(i >= sizeof(temboo_account)){Serial.print("TEMBOO_ACCOUNT is to long.\r\n");while(1);}
        if(*(ptr + 15 + i) == '\r' && *(ptr + 15 + i + 1) == '\n')break;
    }
    temboo_account[i] = '\0';
    //Serial.println(temboo_account);
    
    ptr = strstr(buffer, "TEMBOO_APP_NAME:");
    if(ptr == NULL){Serial.print("TEMBOO_APP_NAME get error.\r\n");while(1);}
    i = 0;
    while(1)
    {
        temboo_app_name[i] = *(ptr + 16 + i);
        i ++;
        if(i >= sizeof(temboo_app_name)){Serial.print("TEMBOO_APP_NAME is to long.\r\n");while(1);}
        if(*(ptr + 16 + i) == '\r' && *(ptr + 16 + i + 1) == '\n')break;
    }
    temboo_app_name[i] = '\0';
    //Serial.println(temboo_app_name);
    
    ptr = strstr(buffer, "TEMBOO_APP_KEY:");
    if(ptr == NULL){Serial.print("TEMBOO_APP_KEY get error.\r\n");while(1);}
    i = 0;
    while(1)
    {
        temboo_app_key[i] = *(ptr + 15 + i);
        i ++;
        if(i >= sizeof(temboo_app_key)){Serial.print("TEMBOO_APP_KEY is to long.\r\n");while(1);}
        if(*(ptr + 15 + i) == '\r' && *(ptr + 15 + i + 1) == '\n')break;
    }
    temboo_app_key[i] = '\0';
    //Serial.println(temboo_app_key);
    
    ptr = strstr(buffer, "WIFI_AP:");
    if(ptr == NULL){Serial.print("WIFI_AP get error.\r\n");while(1);}
    i = 0;
    while(1)
    {
        wifi_ap[i] = *(ptr + 8 + i);
        i ++;
        if(i >= sizeof(wifi_ap)){Serial.print("WIFI_AP is to long.\r\n");while(1);}
        if(*(ptr + 8 + i) == '\r' && *(ptr + 8 + i + 1) == '\n')break;
    }
    wifi_ap[i] = '\0';
    //Serial.println(wifi_ap);
    
    ptr = strstr(buffer, "WIFI_PASSWORD:");
    if(ptr == NULL){Serial.print("WIFI_PASSWORD get error.\r\n");while(1);}
    i = 0;
    while(1)
    {
        wifi_password[i] = *(ptr + 14 + i);
        i ++;
        if(i >= sizeof(wifi_password)){Serial.print("WIFI_PASSWORD is to long.\r\n");while(1);}
        if(*(ptr + 14 + i) == '\r' && *(ptr + 14 + i + 1) == '\n')break;
    }
    wifi_password[i] = '\0';
    //Serial.println(wifi_password);
    //Serial.print("\r\n");
}

char getTheLastMessage(void)
{
    char result = 0;
    int i = 0;
    char *ptr;
    char message_sid_new[40];
    
    TembooChoreo Choreo(client);
    
    Choreo.begin();
    
    Choreo.setAccountName(temboo_account);
    Choreo.setAppKeyName(temboo_app_name);
    Choreo.setAppKey(temboo_app_key);
    Choreo.addInput("AccountSID", twilio_account_sid);
    Choreo.addInput("AuthToken", twilio_auth_token);
    Choreo.addInput("PageSize", "1");
    Choreo.addInput("Page", "0");
    Choreo.addInput("To", twilio_number);
    Choreo.setChoreo("/Library/Twilio/SMSMessages/ListMessages");
    
    Choreo.run();
    
    while(Choreo.available())
    {
        bufferTemp[i ++] = Choreo.read();
        if(i >= sizeof(bufferTemp))break;
    }
    
    Choreo.close();
    
    bufferTemp[i] = '\0';
    Serial.println(bufferTemp);
    
    //ptr = strstr(bufferTemp, "HTTP_CODE\x0A\x1F200\x0A\x1FResponse");
    ptr = strstr(bufferTemp, "200");
    if(ptr != NULL)
    {
        Serial.print("Get http code ok.\r\n");
        
        ptr = strstr(bufferTemp, "\"sid\": \"");
        if(ptr == NULL){Serial.print("Get message sid fail.\r\n");return 0;}
        i = 0;
        while(1)
        {
            message_sid_new[i] = *(ptr + 8 + i);
            i ++;
            if(i >= sizeof(message_sid_new)){Serial.print("message sid is to long.\r\n");return 0;}
            if(*(ptr + 8 + i) == '"')break;
        }
        message_sid_new[i] = '\0';
        if(strstr(message_sid, message_sid_new) == NULL)
        {
            result = 1;
            strcpy(message_sid, message_sid_new);
            Serial.print("message sid is new.\r\n");
        }
        else 
        {
            result = 0;
            Serial.print("message sid is old.\r\n");
        }
        Serial.print("message sid is:");
        Serial.println(message_sid);
    
        ptr = strstr(bufferTemp, "\"from\": \"");
        if(ptr == NULL){Serial.print("Get message from fail.\r\n");return 0;}
        i = 0;
        while(1)
        {
            message_from[i] = *(ptr + 9 + i);
            i ++;
            if(i >= sizeof(message_from)){Serial.print("message from is to long.\r\n");return 0;}
            if(*(ptr + 9 + i) == '"')break;
        }
        message_from[i] = '\0';
        
        ptr = strstr(bufferTemp, "\"body\": \"");
        if(ptr == NULL){Serial.print("Get message body fail.\r\n");return 0;}
        i = 0;
        while(1)
        {
            message_body[i] = *(ptr + 9 + i);
            i ++;
            if(i >= sizeof(message_body)){Serial.print("message body is to long.\r\n");return 0;}
            if(*(ptr + 9 + i) == '"')break;
        }
        message_body[i] = '\0';

        if(result == 0)
        {
            Serial.print("Message from:");
            Serial.println(message_from);
            Serial.print("Message content is:");
            Serial.println(message_body);
            
            Display.clearDisplay();
            DisplayMessage(0, "From:    ", 9);
            DisplayMessage(1, message_from, strlen(message_from));
            DisplayMessage(3, "Content:", 8);
            DisplayMessage(4, message_body, strlen(message_body));
        }
    }
    else 
    {
        Serial.print("Get http code fail.\r\n");
        DisplayMessage(0, "Get sid fail", 12);
    }
    
    return result;
}

void sendMessage(void)
{
    TembooChoreo Choreo(client);
    
    Choreo.begin();
    
    Choreo.setAccountName(temboo_account);
    Choreo.setAppKeyName(temboo_app_name);
    Choreo.setAppKey(temboo_app_key);
    Choreo.addInput("AccountSID", twilio_account_sid);
    Choreo.addInput("AuthToken", twilio_auth_token);
    Choreo.addInput("From", twilio_number);
    Choreo.addInput("To", message_from);
    Choreo.addInput("Body", MESSAGE_CONTENT);
    Choreo.setChoreo("/Library/Twilio/SMSMessages/SendSMS");
    
    Choreo.run();
    
    while(Choreo.available())
    {
        char c = Choreo.read();
        Serial.print(c);
    }
    
    Choreo.close();
}

void DisplayMessage(char line, char *buffer, char len)
{
    char str[13];
    char num;
    
    num = len / 12;
    if(num > 8)num = 8;
    if(len % 12)num += 1;
    
    for(int i = 0; i < num; i ++)
    {
        snprintf(str, sizeof(str), "%s", buffer + i * 12);
        Display.setTextXY(i + line, 0, str);
    }
}