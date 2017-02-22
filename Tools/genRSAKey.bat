openssl genrsa -out rsa.key 2048
openssl rsa -in rsa.key -pubout -out rsa_pub.key 