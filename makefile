PROGRAM = zenzibar

LOCAL_LIB_DIR = ./lib

LOCAL_BUILD_DIR = ./build

CFLAGS += $(shell pkg-config --cflags libmtp gtk+-2.0 glib-2.0 gthread id3tag)

LIBS += $(shell pkg-config --libs libmtp gtk+-2.0 glib-2.0 gthread id3tag)

all: pre-build $(PROGRAM)

zenzibar: main musicLibrary
	@echo -e "\nlinking targets\n"
	gcc -g $(CFLAGS) $(LIBS) $(LOCAL_LIB_DIR)/*.o -o $(LOCAL_BUILD_DIR)/zenzibar

pre-build:
	@echo -e "\nrunning pre-build target\n"
	@mkdir -p build
	@mkdir -p lib

main: mtpWrapper gtkzenui
	@echo -e "\ncompiling main\n"
	gcc $(CFLAGS) $(LIBS) -g -c main.c -o $(LOCAL_LIB_DIR)/main.o

mtpWrapper: handyStructures
	@echo -e "\ncompliling mtpWrapper\n"
	gcc $(CFLAGS) $(LIBS) -g -c mtpWrapper.c -o $(LOCAL_LIB_DIR)/mtpWrapper.o

gtkzenui: handyStructures 
	@echo -e "\ncompiling gtkzenui\n"
	gcc $(CFLAGS) $(LIBS) -g -c gtkzenui.c -o $(LOCAL_LIB_DIR)/gtkzenui.o

handyStructures:
	@echo -e "\ncompiling handyStructures\n"
	gcc $(CFLAGS) $(LIBS) -g -c handyStructures.c -o $(LOCAL_LIB_DIR)/handyStructures.o

musicLibrary: threadManager
	@echo -e "\ncompiling musicLibrary\n"
	gcc $(CFLAGS) $(LIBS) -g -c musicLibrary.c -o $(LOCAL_LIB_DIR)/musicLibrary.o

threadManager:
	@echo -e "\ncompiling thread manager\n"
	gcc $(CFLAGS) $(LIBS) -g -c threadManager.c -o $(LOCAL_LIB_DIR)/threadManager.o

.PHONY: clean
clean:
	@rm -f $(LOCAL_LIB_DIR)/*.o
	@rm -f $(LOCAL_BUILD_DIR)/$(PROGRAM)
