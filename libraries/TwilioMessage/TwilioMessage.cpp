
#include "TwilioMessage.h"


TwilioMessageClass::TwilioMessageClass(char *accountName, char *appKeyName, char *appKeyValue, 
                                        char *accountSid, char *authToken, char *type)
{   
    m_type = type;
    m_accountName = accountName;
    m_appKeyName = appKeyName;
    m_appKeyValue = appKeyValue;
    
    m_accountSid = accountSid;
    m_authToken = authToken;
    
    static TembooChoreo choreoWifi(clientWifi);
    static TembooChoreo choreoGPRS(clientGPRS);

    if(strcmp(type, "wifi") == 0)Choreo = &choreoWifi;
    else if(strcmp(type, "gprs") == 0)Choreo = &choreoGPRS;
}

int TwilioMessageClass::connect(char *wifiAP, char *wifiPassword)
{
    return LWiFi.connect(wifiAP, LWiFiLoginInfo(LWIFI_WPA, wifiPassword));
}

int TwilioMessageClass::connect(void)
{
    return LGPRS.attachGPRS();
}

void TwilioMessageClass::GetLastMessageFromNumber(char *from)
{ 
    Choreo->begin();
    
    Choreo->setAccountName(m_accountName);
    Choreo->setAppKeyName(m_appKeyName);
    Choreo->setAppKey(m_appKeyValue);
    
    Choreo->addInput("AccountSID", m_accountSid);
    Choreo->addInput("AuthToken", m_authToken);
    Choreo->addInput("From", from);
    
    Choreo->setChoreo("/Library/Twilio/SMSMessages/GetLastMessageFromNumber");
    
    Choreo->run();
}

void TwilioMessageClass::SendSMS(char *from, char *to, char *body)
{
    Choreo->begin();
    
    Choreo->setAccountName(m_accountName);
    Choreo->setAppKeyName(m_appKeyName);
    Choreo->setAppKey(m_appKeyValue);
    
    Choreo->addInput("AccountSID", m_accountSid);
    Choreo->addInput("AuthToken", m_authToken);
    Choreo->addInput("From", from);
    Choreo->addInput("To", to);
    Choreo->addInput("Body", body);
    
    Choreo->setChoreo("/Library/Twilio/SMSMessages/SendSMS");
    
    Choreo->run();
}

void TwilioMessageClass::ListMessages(void)
{
    Choreo->begin();
    
    Choreo->setAccountName(m_accountName);
    Choreo->setAppKeyName(m_appKeyName);
    Choreo->setAppKey(m_appKeyValue);
    
    Choreo->addInput("AccountSID", m_accountSid);
    Choreo->addInput("AuthToken", m_authToken);
    
    Choreo->setChoreo("/Library/Twilio/SMSMessages/ListMessages");
    
    Choreo->run();
}

void TwilioMessageClass::GetMessage(char *smsMessageSID)
{
    Choreo->begin();
    
    Choreo->setAccountName(m_accountName);
    Choreo->setAppKeyName(m_appKeyName);
    Choreo->setAppKey(m_appKeyValue);
    
    Choreo->addInput("AccountSID", m_accountSid);
    Choreo->addInput("AuthToken", m_authToken);
    Choreo->addInput("SMSMessageSID", smsMessageSID);
    
    Choreo->setChoreo("/Library/Twilio/SMSMessages/GetMessage");
    
    Choreo->run();
}

void TwilioMessageClass::GetLastMessageThatContains(char *filter)
{
    Choreo->begin();
    
    Choreo->setAccountName(m_accountName);
    Choreo->setAppKeyName(m_appKeyName);
    Choreo->setAppKey(m_appKeyValue);
    
    Choreo->addInput("AccountSID", m_accountSid);
    Choreo->addInput("AuthToken", m_authToken);
    Choreo->addInput("Filter", filter);
    
    Choreo->setChoreo("/Library/Twilio/SMSMessages/GetLastMessageThatContains");
    
    Choreo->run();
}

void TwilioMessageClass::GetMessagesThatContain(char *filter)
{
    Choreo->begin();
    
    Choreo->setAccountName(m_accountName);
    Choreo->setAppKeyName(m_appKeyName);
    Choreo->setAppKey(m_appKeyValue);
    
    Choreo->addInput("AccountSID", m_accountSid);
    Choreo->addInput("AuthToken", m_authToken);
    Choreo->addInput("Filter", filter);
    
    Choreo->setChoreo("/Library/Twilio/SMSMessages/GetMessagesThatContain");
    
    Choreo->run();
}

int TwilioMessageClass::available(void)
{
    return Choreo->available();
}

char TwilioMessageClass::read(void)
{
    return Choreo->read();
}

void TwilioMessageClass::close(void)
{
    return Choreo->close();
}
