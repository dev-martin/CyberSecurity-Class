* Establish crypto handshake during creation of init files
    - each init file will contain private key of self, public key and modulus of other and shared secret key (eliminates need for network handshake protocol)
* Get shared key from init file
* digitally sign the data before sending
    - provides integrity and authenticity
    - certificate not needed since each will have the other's public keys at init
