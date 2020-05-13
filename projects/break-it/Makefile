CC = gcc
CFLAGS = -g -fno-stack-protector -z execstack -Wall -Iutil -Iatm -Ibank -Irouter -I.
LDFLAGS = -lcrypto -lssl

all: bin bin/init bin/atm bin/bank bin/router

bin:
	mkdir -p bin

bin/init : init.c
	${CC} ${CFLAGS} init.c -o bin/init ${LDFLAGS}

bin/atm : atm/atm-main.c atm/atm.c crypto/crypto.c
	${CC} ${CFLAGS} atm/atm.c atm/atm-main.c crypto/crypto.c -o bin/atm ${LDFLAGS}

bin/bank : bank/bank-main.c bank/bank.c util/hash_table.c util/list.c crypto/crypto.c
	${CC} ${CFLAGS} bank/bank.c bank/bank-main.c util/list.c util/hash_table.c crypto/crypto.c -o bin/bank ${LDFLAGS}

bin/router : router/router-main.c router/router.c
	${CC} ${CFLAGS} router/router.c router/router-main.c -o bin/router

test : util/list.c util/list_example.c util/hash_table.c util/hash_table_example.c
	${CC} ${CFLAGS} util/list.c util/list_example.c -o bin/list-test
	${CC} ${CFLAGS} util/list.c util/hash_table.c util/hash_table_example.c -o bin/hash-table-test

clean:
	cd bin && rm -f atm bank router init list-test hash-table-test list hash_table
