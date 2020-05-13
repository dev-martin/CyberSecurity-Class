#! /bin/bash

# These two commands will create our root certificate. There are two steps
# to this: (1) creating a certificate request, and (2) signing the request.
# For the root, this will be self-signed. We have a hard-coded password,
# which you can remove if you like. For a real CA that you'd want to use,
# you absolutely don't want this hard-coded!
openssl req -newkey rsa:1024 -keyout root.priv -out root.req -subj "/CN=root" -passout pass:asdf
openssl x509 -req -in root.req -signkey root.priv -out root.pem -passin pass:asdf -extfile ca.cnf -extensions root

# Now we do something very similar to create a *delegate* CA certificate, but
# it's signed by our root.
openssl req -newkey rsa:1024 -keyout delegate.priv -out delegate.req -subj "/CN=delegate" -passout pass:asdf
openssl x509 -req -in delegate.req -CAcreateserial -CA root.pem -CAkey root.priv -passin pass:asdf -out delegate.pem -extfile ca.cnf -extensions delegate

# ==> Create a server certificate <==

# ==> Create a user certificate <==

