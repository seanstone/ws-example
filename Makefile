bin/ws_example: *.c
	mkdir -p $(@D)
	$(CC) $^ -O2 -lwebsockets -o $@

.PHONY: clean
clean:
	rm -rf bin