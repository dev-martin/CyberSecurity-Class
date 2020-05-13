#! /bin/bash

make decrypt1

python ngrams.py test.txt

./decrypt1 keyword1.txt cipher1.txt 

echo "Decrypted message is:"
cat message1.txt
echo ""
