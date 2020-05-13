# Austin's General Bank-ATM Structure Logic

## Bank
* Bank keeps track of name of the current user of the ATM (possible since only one ATM session happening at a time, reduces need for username in each network request packet)

## ATM
* When user enters begin-session command, begin-session-request is sent for bank process to handle
* If bank returns affirmative, ATM checks pin with `<user-name>.card` file and grants access if valid, otherwise sends end-session-request to Bank (since begin-session will cause bank to keep track of this user as the current user)
* withdraw and balance requests fairly straightforward
* end-session request removes bank-side tracking of current user (global string in bank process)
