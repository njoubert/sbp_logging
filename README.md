# Swift Binary Protocol: Logging and Decoding

## Contents

- **decoder**: Decodes raw SBP data
- **arduino_galileo_datalogger**: Arduino sketch for Intel Galileo, serial datalogger 

## Decoder

This runs through a file byte by byte, looking for SBP messages.
It finds a potential SBP message by the SBP_PREAMBLE byte (0x55).
It then attempts to decode it.

Example usage:
	
	cd decoder
	make
	decoder -d ../data/simple.dat

simple.dat contains exactly one SBP heartbeat message (type 0xFFFF, 12 bytes long with header). The above output will be:

	> ./decoder -d ../data/simple.dat 
	Message: type = 0xffff, sender = 0x043f, len = 0x04, crc = 0xd976
	Read 12 bytes.

If you want to generate JSON data of these messages, you can:

	> ./decoder -j ../data/simple.dat
	
