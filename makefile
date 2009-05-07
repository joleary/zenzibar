PROGRAM = zenzibar

LOCAL_LIB_DIR = lib

LOCAL_BUILD_DIR = build

CFLAGS += $(shell pkg-config --cflags libmtp gtk+-2.0)

LIBS += $(shell pkg-config --libs libmtp gtk+-2.0)

all: pre-build $(PROGRAM)

zenzibar: gui.o debugConsole.o mtpAdapter.o mtpDeviceDetails.o libraryWidget.o
	g++ $(CFLAGS) $(LIBS)  $(LOCAL_LIB_DIR)/*.o -o$(LOCAL_BUILD_DIR)/zenzibar

pre-build:
	@mkdir -p build
	@mkdir -p lib

gui.o:
	g++ $(CFLAGS) $(LIBS) -c gui.cpp -o $(LOCAL_LIB_DIR)/gui.o

debugConsole.o:
	g++ $(CFLAGS) $(LIBS) -c debugConsole.cpp -o $(LOCAL_LIB_DIR)/debugConsole.o

mtpAdapter.o:
	g++ $(CFLAGS) $(LIBS) -c mtpAdapter.cpp -o $(LOCAL_LIB_DIR)/mtpAdapter.o

mtpDeviceDetails.o:
	g++ -c mtpDeviceDetails.cpp -o $(LOCAL_LIB_DIR)/mtpDeviceDetails.o

libraryWidget.o:
	g++ $(CFLAGS) $(LIBS) -c libraryWidget.cpp -o $(LOCAL_LIB_DIR)/libraryWidget.o

.PHONY: clean
clean:
	@rm -f $(LOCAL_LIB_DIR)/*.o
	@rm -f $(LOCAL_BUILD_DIR)/$(PROGRAM)
