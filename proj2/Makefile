CC=gcc
out=ftserver
C_FLAGS=-Wall -Werror -std=gnu99 -MMD
DEBUG_FLAGS=-DDEBUG -ggdb

OUT_FILE=neadwora_proj2.zip

.PHONY: all
all: $(out) ftclient

.SILENT: debug
.PHONY: debug
debug:
	$(eval C_FLAGS += $(DEBUG_FLAGS))
	echo > /dev/null

$(out): server.*
	$(CC) $(C_FLAGS) -o $@ $^

clean:
	$(RM) $(out) *.d core.* $(out) ftclient

ftclient:
	@ln -s ftclient.py ftclient 2>/dev/null || true
	@chmod +x ftclient.py

-include *.d

submit:
	rm -rf $(OUT_FILE)
	zip -r $(OUT_FILE) README.txt Makefile *.c *.h *.py
