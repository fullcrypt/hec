gen secret eval
enc32 12 secret 12
enc32 15 secret 15
enc 0 secret zero

enc32 2147483640 secret 2147483640
enc32 120 secret 120

enc32 4294967290 secret 4294967290

./executing.sh
./decrypting_results.sh
