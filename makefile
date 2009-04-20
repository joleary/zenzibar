PROGRAM = zenzibar

CFLAGS += $(shell pkg-config --cflags libmtp gtk+-2.0)

LIBS += $(shell pkg-config --libs libmtp gtk+-2.0)


all: $(PROGRAM)

zenzibar: gui.o debugConsole.o mtpAdapter.o mtpDeviceDetails.o
	g++ $(CFLAGS) $(LIBS)  *.o -ozenzibar

gui.o:
	g++ $(CFLAGS) $(LIBS) -c gui.cpp

debugConsole.o:
	g++ $(CFLAGS) $(LIBS) -c debugConsole.cpp

mtpAdapter.o:
	g++ $(CFLAGS) $(LIBS) -c mtpAdapter.cpp

mtpDeviceDetails.o:
	g++ -c mtpDeviceDetails.cpp


clean:
	@rm -f *.o
	@rm -f $(PROGRAM)
