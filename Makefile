all: emulate assemble

emulate: 
	$(MAKE) -C emulator

assemble:
	$(MAKE) -C assembler

clean:
	$(MAKE) -C emulator clean
	$(MAKE) -C assembler clean
