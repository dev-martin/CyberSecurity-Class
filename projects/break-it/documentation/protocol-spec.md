** Packet field format = fieldSize: fieldName - fieldValue **

## ATM Requests

1. begin-session-request
    - Command: `begin-session <user-name>`
    - Purpose: Establish a session on the ATM, allow bank to keep track of current session (since only need to support one ATM session at a time).
    - Packet Structre:
        * 1 byte: Message Type - 0
        * 1 byte: Username Length - strlen(`<user-name>`)
        * strlen(`<user-name>`) bytes: Username - `<user-name>`

2. withdraw-request
    - Command: `withdraw <amt>`
    - Purpose: Request a withdraw from ATM current user bank acct and deduct `<amt>` from that bank acct if sufficient funds available
    - Packet structure:
        * 1 byte: Message Type - 1
        * sizeof(int) bytes: Amount Requested - `<amt>`

3. balance-request
    - Command: `balance`
    - Purpose: Retreive balance of ATM current user.
        * 1 byte: Message Type - 2

4. end-session-request
    - Command: `end-session`
    - Purpose: End session currently tracked by bank (sent when `end-session` command called by user or when user enters invalid pin)
    - Packet Structrue:
        * 1 byte: Message Type - 3

## Bank Responses

Note: All bank commmands handled locally by bank, so no network exchange needed

1. begin-session-response
    - Purpose: Inicate whether user in begin-session-request is registered with the bank.
    - Packet Structure:
        * 1 byte: Message Type - 4
        * 1 byte: User Registered
            - 0 (user **not** registered with bank)
            - 1 (user **is** registered with bank)
            - 2 (another user is already logged in)

2. withdraw-response
    - Purpose: Indicate whether current user has suffiecient funds to satisfy withdraw-request
    - Packet Structure:
        * 1 byte: Message Type - 5
        * 1 byte: Sufficient Funds
            - 0 (insufficient funds)
            - 1 (sufficient funds)
            - 2 (no user logged in)

3. balance-response
    - Purpose: Respond to balance-request with how much is in current user's account.
    - Packet structure:
        * 1 byte: Message Type - 6
        * 1 byte: User logged in
            - 0 (no user logged in)
            - 1 (user logged in)
        * sizeof(int) bytes: Amount - amount in current user's account
            - note: only present if user logged in byte is 1
