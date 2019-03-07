TARGET = oss user

all: $(TARGET)

oss: oss.o
	gcc -o$@ $^

user: user.o
	gcc -o$@ $^

%.o: %.c
	gcc -c -o$@ $<

clean:
	rm -f *.o $(TARGET)