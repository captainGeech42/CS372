OUT_FILE=neadwora_proj1.zip

run:
	(cd client && make)
	@chmod +x server/chatserve.py
	@chmod +x client/chatclient
	@ln -s server/chatserve.py chatserve 2>/dev/null || true
	@ln -s client/chatclient chatclient 2>/dev/null || true

submit:
	rm -rf $(OUT_FILE)
	zip -r $(OUT_FILE) README Makefile client server
