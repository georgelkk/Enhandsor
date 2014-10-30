#!/usr/bin/python
# -*- coding: utf-8 -*-
#=====================================================
# George   2014.08
#  
# function/class about SMS get/send using Skype4Py
#=====================================================
# 4086099523    Client phone
# +16164266851  Server phone
from twilio.rest import TwilioRestClient 
 
# put your own credentials here 
# ACCOUNT_SID = "XXXXXXXXX" 
# AUTH_TOKEN = "XXXXXXXXX" 
# client = TwilioRestClient(ACCOUNT_SID, AUTH_TOKEN) 
twilio_para = {   'ACCOUNT_SID' : "XXXXXXXX",
                'AUTH_TOKEN' : "XXXXXXX",
}

# send
class SMSTwilio():
    # var
    ACCOUNT_SID = "" 
    AUTH_TOKEN = "" 
    client = None

    # init 
    def __init__(self):
        self.ACCOUNT_SID = twilio_para['ACCOUNT_SID']
        self.AUTH_TOKEN = twilio_para['AUTH_TOKEN']
        self.client = TwilioRestClient(self.ACCOUNT_SID, self.AUTH_TOKEN) 

    def send_message(self,to_Num,from_Num,body):
        self.client.messages.create(
            #to="408XXXXXXXX", 
            #from_="+1616XXXXXXXX", 
            #body="Timestamp:XXX , User: XXX",  
            to=to_Num, 
            from_=from_Num, 
            body=body,  
        )

    def get_messages(self):
        messages = self.client.messages.list()
        #for m in messages:   # for test
            #print m
            #print m.sid     # for test
            #print '------'
        return messages

    def get_messages_mid(self):
        mids= []
        messages = self.client.messages.list()
        for m in messages: mids.append(m.sid)     
        return mids

    def get_message(self,ID):
        message = self.client.messages.get(ID) 
        #print message # <Message SMf82>
        #print message.body # for test
        #print dir(message)
        #print message.date_sent
        #print message.body
        return message

# main
if __name__ == "__main__":
    print '-SMSSkype-testing-'
    SMS = SMSTwilio()
    
    # test send
    #SMS.send_message('408XXXXXXX','+1616XXXXXX','Python test 08/04 night')
    # test get list
    mids = SMS.get_messages_mid()
    print mids
    print len(mids) # 8
    # test get one
    #SMS.get_message('XXXXXXXXXX')
    #for mid in mids: print SMS.get_message(mid)
    curr_SMS = SMS.get_message('XXXXXXXX')
    print curr_SMS.body
    print curr_SMS.from_
    print type(curr_SMS.from_)
    #print SMS.get_message('XXXXXXXX')


    # do main flow here
