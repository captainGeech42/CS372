CC=gcc
out=chatclient
C_FLAGS=-Wall -Werror -std=gnu99 -MMD
DEBUG_FLAGS=-DDEBUG -ggdb

.PHONY: all
all: $(out)

.SILENT: debug
.PHONY: debug
debug:
	$(eval C_FLAGS += $(DEBUG_FLAGS))
	echo > /dev/null

chatclient: chatclient.c chatclient.h
	$(CC) $(C_FLAGS) -o $@ $^

clean:
	$(RM) $(out) *.d

-include *.d
