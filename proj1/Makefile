CC=gcc
out=chatclient
C_FLAGS=-Wall -Werror -std=gnu99 -MMD
DEBUG_FLAGS=-DDEBUG -ggdb
OUT_FILE=neadwora_proj1.zip

.PHONY: all
all: $(out) chatserve

.SILENT: debug
.PHONY: debug
debug:
	$(eval C_FLAGS += $(DEBUG_FLAGS))
	echo > /dev/null

chatclient: chatclient.c chatclient.h
	$(CC) $(C_FLAGS) -o $@ $^

clean:
	$(RM) $(out) *.d core.* chatserve

chatserve:
	@ln -s chatserve.py chatserve 2>/dev/null || true
	@chmod +x chatserve.py

-include *.d

submit:
	rm -rf $(OUT_FILE)
	zip -r $(OUT_FILE) README.txt Makefile *.c *.h *.py
