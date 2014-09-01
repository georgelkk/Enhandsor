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
# ACCOUNT_SID = "ACecbbf33c08c3e0ad9e4322c1cb20711d" 
# AUTH_TOKEN = "fdf1f8fd526dacf14015bf4d38531503" 
# client = TwilioRestClient(ACCOUNT_SID, AUTH_TOKEN) 
twilio_para = {   'ACCOUNT_SID' : "ACecbbf33c08c3e0ad9e4322c1cb20711d",
                'AUTH_TOKEN' : "fdf1f8fd526dacf14015bf4d38531503",
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
            #to="4086099523", 
            #from_="+16164266851", 
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
    #SMS.send_message('4086099523','+16164266851','Python test 08/04 night')
    # test get list
    mids = SMS.get_messages_mid()
    print mids
    print len(mids) # 8
    # test get one
    #SMS.get_message('SMf8200712918e42faa759a964db8fc759')
    #for mid in mids: print SMS.get_message(mid)
    curr_SMS = SMS.get_message('SMff3a42b17498950b7b29cdc5fb30dafa')
    print curr_SMS.body
    print curr_SMS.from_
    print type(curr_SMS.from_)
    #print SMS.get_message('SM1368b4b17cb8f4e585dc25911a92b167')


    # do main flow here