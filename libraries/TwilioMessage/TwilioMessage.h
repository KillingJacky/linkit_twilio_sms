
#ifndef _TWILIO_MESSAGE_H_
#define _TWILIO_MESSAGE_H_


#include "Arduino.h"
#include <LWiFi.h>
#include <LWiFiClient.h>
#include <LGPRS.h>
#include <LGPRSClient.h>
#include <Temboo.h>


class TwilioMessageClass
{
public:
    
    TwilioMessageClass(char *accountName, char *appKeyName, char *appKeyValue, 
                        char *accountSid, char *authToken, char *type);
    
    int connect(char *wifiAP, char *wifiPassword);
    int connect(void);
    
    void GetLastMessageFromNumber(char *from);
    void SendSMS(char *from, char *to, char *body);
    void ListMessages(void);    
    void GetMessage(char *smsMessageSID);
    void GetLastMessageThatContains(char *filter);
    void GetMessagesThatContain(char *filter);
    
    int available(void);
    char read(void);
    void close(void);

private:

    LWiFiClient clientWifi;
    LGPRSClient clientGPRS;

    TembooChoreo *Choreo;
    
    char *m_type;
    char *m_accountName;
    char *m_appKeyName;
    char *m_appKeyValue;
    
    char *m_accountSid;
    char *m_authToken;
    
};



#endif
