CXXFLAGS =	-O2 -g -Wall -fmessage-length=0

OBJS =		os1shell.o

LIBS =

TARGET =	os1shell

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
