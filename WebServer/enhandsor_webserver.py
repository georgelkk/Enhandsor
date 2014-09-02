#!/usr/bin/python
# -*- coding: utf-8 -*-
#=====================================================
# George   2014.08
# Twilio ###-###-####
# Main interface with user
# Basic function: get SMS / Send SMS
# 
#=====================================================
from datetime import date
import time
import json
import tornado.escape
import tornado.ioloop
import tornado.web
import threading
from SMSTwilio import SMSTwilio
from twilio.rest import TwilioRestClient 
#  globle var
twilio_para = { 'ACCOUNT_SID' : "OOOOOOOOOO",
                'AUTH_TOKEN'  : "XXXXXXXXXX",
}

#words
_WASH_H = 'Good!wash hand! you press:'
_WASH_T = ', here is the recharge XXXX!'
#========================================
# var for demo 
massage_mids = []
Bottle_Num = '+############'
client_Num = '+############'
Server_Num = '+############'
#========================================

#init 
SMS = SMSTwilio()
massage_mids = SMS.get_messages_mid()
# function
def demo_loop(massage_mids):
    print '-------------------------------'
    print '-start new update-'
    print '-------------------------------'
    _no_SMS = True
    # do something here
    # init
    new_mids = SMS.get_messages_mid()
    #print new_mids
    #print len(new_mids)
    old_mids = massage_mids
    #print old_mids
    #print len(old_mids)
    # main loop
    for mid in new_mids:
        if mid not in old_mids:
            _no_SMS = False
            # for each new message
            print 'new meesage!'
            curr_sms = SMS.get_message(mid)
            #print curr_sms.date_sent
            #print curr_sms.body
            if curr_sms.from_ == unicode(Bottle_Num):
                print 'It is from bottle, num: '+ str(Bottle_Num)
                print 'sending timestamp     : '+ str(curr_sms.date_sent)
                print 'Meesage contain       : '+ str(curr_sms.body)
                print 'Send recharge to user : '+ str(client_Num) 
                # send (to,from, SMS)
                curr_message =  _WASH_H +str(curr_sms.body)+ _WASH_T
                SMS.send_message(client_Num,Server_Num,curr_message)
                print '-------------------------------'
            else:
                print 'It is not from bottle'
                print curr_sms.from_
                print curr_sms.date_sent
                #print curr_sms.body
                print '-------------------------------'

            #phase data
            #send SMS
    # print no SMS
    if _no_SMS: print 'no new sms'
    #update old_mids
    massage_mids = new_mids
    print '-------------------------------'
    print '-done update, wait 60 sec--'
    print '-------------------------------'
    print ' '
    time.sleep(20)
    demo_loop(massage_mids)
    #threading.Timer(60, demo_loop ).start()

def info_update():
    print '-start update-'
    # do something here
    print '-done update--'
    # update every 90 sec
    threading.Timer(60, info_update).start()

class VersionHandler(tornado.web.RequestHandler):
    def get(self,id):
        response = {'version':'2014.08.01 v1.0',
                    'SMS': 'send and receive'
        }
        self.write(response)

class GetJobsByIdHandler(tornado.web.RequestHandler):
    def get(self, id):
        response = {} 
        self.write(response)

class GetUsersByIdHandler(tornado.web.RequestHandler):
    def get(self, id):
        response = {} 
        self.write(response)

 
application = tornado.web.Application([
    (r"/getRcmJobposts/([a-zA-Z0-9/]+)", GetJobsByIdHandler),
    (r"/getRcmUsers/([a-zA-Z0-9/]+)", GetUsersByIdHandler),
    (r"/version", VersionHandler)
])
#   test line
 
if __name__ == "__main__":
    print  'Enhandsor  -server-start-'
    # update from penn's api
    #info_update()
    massage_mids = SMS.get_messages_mid()
    demo_loop(massage_mids)

    application.listen(8888)
    tornado.ioloop.IOLoop.instance().start()
