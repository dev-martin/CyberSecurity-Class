#! /usr/bin/env python

import json
import BaseHTTPServer
import pymysql
from SocketServer import ThreadingMixIn
import urlparse


PORT = 8000

class Q4RequestHandler(BaseHTTPServer.BaseHTTPRequestHandler):

   def do_GET(self):
      pathels = self.path.split('?')
      query_params = dict()
      if len(pathels) > 1:
         for qp in pathels[1].split('&'):
            qpels = qp.split('=')
            if len(qpels) > 1:
               qp_name = qpels[0]
               qp_val = qpels[1]
               query_params[qp_name] = qp_val
      response = json.dumps(query_params)
      self.send_response(200)
      self.send_header('Content-type', 'application/json')
      self.end_headers()
      self.wfile.write(response)
      self.wfile.close()

   def do_POST(self):
      length = int(self.headers['Content-Length'])
      request_data = self.rfile.read(length).decode('utf-8')
      self.rfile.close()
      print(request_data)
      response = "OK"
      self.send_response(200)
      self.send_header('Content-type', 'application/json')
      self.end_headers()
      print("writing response")
      self.wfile.write(response)
      self.wfile.close()
      print("done")

   def do_HEAD(self):
      self.send_response(200)
      self.send_header('Content-type', 'application/json')
      self.end_headers()

class ThreadedHTTPServer(ThreadingMixIn, BaseHTTPServer.HTTPServer):
    """ This class allows to handle requests in separated threads.
        No further content needed, don't touch this. """

# Example code for connecting to the database. Place where appropriate.
conn = pymysql.connect(host='localhost',
                       user='root',
                       password='seedubuntu',
                       db='sql_collabtive_db',
                       cursorclass=pymysql.cursors.DictCursor)

try:
    with conn.cursor() as cursor:
        # Get all the users
        sql = "SELECT ID, name FROM user"
        cursor.execute(sql)
        result = cursor.fetchall()
        print(result)

    # connection is not autocommit by default. So you must commit to save
    # your changes, if you've made any.
    conn.commit()

finally:
    conn.close()


# Start the server
Handler = Q4RequestHandler

httpd = BaseHTTPServer.HTTPServer(("", PORT), Handler)

print("serving at port {port}".format(port=PORT))
httpd.serve_forever()

