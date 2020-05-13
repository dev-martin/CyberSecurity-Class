## Design Document:

### Security Protocol
#### Specific Attacks considered:
1. Integer overflow/underflow
    - Doesn't accept negative numbers (underflow) and checks whether integer string can be represented in a uint before conversion (also checks strtol wraparound thorugh errno).
2. Buffer overflow
    - Only receives up to MAX\_MSG\_SIZE (largest valid message that can be input) bytes to store in buffer of that same size. Discards the rest of the input in stdin.
3. Eavesdropping
    - Data sent over the wire is encrypted with AES-CBC to prevent a passive wiretapper from easily reading messages sent over the wire.
4. Masquerade attack
    - Pin/card never sent over the network. Implies attacker can't gain access to either of these through bank/atm communications, even if they somehow are able to decode AES-CBC. This leaves examing the bank computer or the atm memory for this info, which is assumed impossible by the threat model.
5. Reading another user's PIN from the `.card` files
    - When a new user is created, the PIN is hashed with sha256 before it is stored in the card file. When the ATM validates a PIN, it compares the value in the card file with the hashed input from the user. This ensures that a user's PIN cannot be read from a user's card.

#### Threats Ignored:
1. Active wiretapping
    - Did not have the time to digitally sign messages to ensure message integrity.

### ATM/Bank Communication Protocol

#### ATM Requests

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

#### Bank Responses

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
