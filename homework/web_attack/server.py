#! /usr/bin/env python

import SimpleHTTPServer
import SocketServer

PORT = 8000

class Q4RequestHandler(SimpleHTTPServer.SimpleHTTPRequestHandler):
   def create_page(self,skin):
      skin_file = open(skin)
      page_list = [
         '<html>',
         '<head>',
         skin_file.read(),
         '</head>',
         '<body>',
         '<p class="styled">hello!</p>',
         '</body>',
         '</html>'
      ]
      return '\n'.join(page_list)

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
      skin = 'light'
      if 'skin' in query_params:
         skin = query_params['skin']
      print skin
      self.send_response(200)
      self.end_headers()
      self.wfile.write(self.create_page(skin))

Handler = Q4RequestHandler

httpd = SocketServer.TCPServer(("", PORT), Handler)

print "serving at port", PORT
httpd.serve_forever()

