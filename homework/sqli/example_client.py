#! /usr/bin/env python

import urllib2
import json

print 'Testing GET'
get_req = urllib2.Request('http://localhost:8000/?a=b&c=d')
f = urllib2.urlopen(get_req)
data = f.read()
f.close()
print data

print 'Testing POST'
req_data = json.dumps( { 'a': 'b', 'c': 'd' } )
post_req = urllib2.Request('http://localhost:8000/', req_data)
f = urllib2.urlopen(post_req)
data = f.read()
f.close()
print data

