# rtf2txt
A C sample based on librtf to extract txt from rtf
# structure
![alt text](image.png)

test.c main program to extract txt from rtf

others source code of librtf

# compile
gcc -o test test.c convert.c librtf.c parser.c rtfproc.c

# run
./test

# tips
For chinese characters, we extract GBK from rtf,then you need to decode it and get the content.

For English characters, you can uncomment part->type == PART_TYPE_TEXT in test.c, then you will get English content with some control characters