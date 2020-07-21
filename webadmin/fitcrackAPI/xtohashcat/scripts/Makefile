
all:
	$(CC) -static -g -O2 zip2john.c -o zip2john
	$(CC) -static -g -O2 -D_GNU_SOURCE -DARCH_LITTLE_ENDIAN=1 rar2john.c base64_convert.c -o rar2john
