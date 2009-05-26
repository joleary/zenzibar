PROGRAM = zenzibar

LOCAL_LIB_DIR = ./lib

LOCAL_BUILD_DIR = ./build

CFLAGS += $(shell pkg-config --cflags libmtp gtk+-2.0)

LIBS += $(shell pkg-config --libs libmtp gtk+-2.0)

all: pre-build $(PROGRAM)

zenzibar: main mtpWrapper gtkzenui handyStructures musicLibrary
	@echo -e "\nlinking targets\n"
	gcc -pthread -g $(CFLAGS) $(LIBS) $(LOCAL_LIB_DIR)/*.o -o $(LOCAL_BUILD_DIR)/zenzibar

pre-build:
	@echo -e "\nrunning pre-build target\n"
	@mkdir -p build
	@mkdir -p lib

main:
	@echo -e "\ncompiling main\n"
	gcc $(CFLAGS) $(LIBS) -g -c main.c -o $(LOCAL_LIB_DIR)/main.o

mtpWrapper: handyStructures
	@echo -e "\ncompliling mtpWrapper\n"
	gcc $(CFLAGS) $(LIBS) -g -c mtpWrapper.c -o $(LOCAL_LIB_DIR)/mtpWrapper.o

gtkzenui:
	@echo -e "\ncompiling gtkzenui\n"
	gcc $(CFLAGS) $(LIBS) -g -c gtkzenui.c -o $(LOCAL_LIB_DIR)/gtkzenui.o

handyStructures:
	@echo -e "\ncompiling handyStructures\n"
	gcc $(CFLAGS) $(LIBS) -g -c handyStructures.c -o $(LOCAL_LIB_DIR)/handyStructures.o

musicLibrary:
	@echo -e "\ncompiling musicLibrary\n"
	gcc -pthread $(CFLAGS) $(LIBS) -g -c musicLibrary.c -o $(LOCAL_LIB_DIR)/musicLibrary.o

.PHONY: clean
clean:
	@rm -f $(LOCAL_LIB_DIR)/*.o
	@rm -f $(LOCAL_BUILD_DIR)/$(PROGRAM)
