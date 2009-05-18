PROGRAM = zenzibar

LOCAL_LIB_DIR = lib

LOCAL_BUILD_DIR = build

CFLAGS += $(shell pkg-config --cflags libmtp gtk+-2.0)

LIBS += $(shell pkg-config --libs libmtp gtk+-2.0)

all: pre-build $(PROGRAM)

zenzibar: main mtpWrapper gtkzenui
	g++ -g $(CFLAGS) $(LIBS) $(LOCAL_LIB_DIR)/main.o $(LOCAL_LIB_DIR)/mtpWrapper.o $(LOCAL_LIB_DIR)/gtkzenui.o -o $(LOCAL_BUILD_DIR)/zenzibar

pre-build:
	@mkdir -p build
	@mkdir -p lib

main:
	g++ $(CFLAGS) $(LIBS) -g -c main.c -o $(LOCAL_LIB_DIR)/main.o

mtpWrapper:
	g++ $(CFLAGS) $(LIBS) -g -c mtpWrapper.c -o $(LOCAL_LIB_DIR)/mtpWrapper.o

gtkzenui:
	g++ $(CFLAGS) $(LIBS) -g -c gtkzenui.c -o $(LOCAL_LIB_DIR)/gtkzenui.o

.PHONY: clean
clean:
	@rm -f $(LOCAL_LIB_DIR)/*.o
	@rm -f $(LOCAL_BUILD_DIR)/$(PROGRAM)
