#!/usr/bin/python
# -*- coding: utf-8 -*-
#=====================================================
# George   2014.08
# mongo DB as DB
#-------------------
# done: insall{mongodb pymongo}. test basic-usage 
# need: build function{post,update,query,del,backup}
#=====================================================
import pymongo
import datetime

# var

# data
# function
def pymongo_tutorial():
    # new client
    client = pymongo.MongoClient()
    db = client['test-database']
    collection = client['test-collection']

    post = {"author": "George",
            "text": "My first blog post!",
            "tags": ["mongodb", "python", "pymongo"],
            "date": datetime.datetime.utcnow()}


    # test post
    posts = db.posts
    post_id = posts.insert(post)
    print post_id

# init 
    # new user data # profile 

# mantain
    # user stuatus
    # 




#=====================================================
#=====================================================

#=====================================================
if __name__ == "__main__":
    print  '-Start-   Enhandsor db test-'

    #test
    pymongo_tutorial()

    #! test 
    print '-end-      Enhandsor db test-'
