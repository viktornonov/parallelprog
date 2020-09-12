.SUFFIXES: .class .java
.java.class:
	javac $(JFLAGS) $<

JFLAGS=

all: Consumer.class Program.class Producer.class BoundedBuffer.class BoundedBuffer.class
	java Program
clean :
	rm *.class
