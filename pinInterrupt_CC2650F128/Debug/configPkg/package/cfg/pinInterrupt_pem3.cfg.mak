# invoke SourceDir generated makefile for pinInterrupt.pem3
pinInterrupt.pem3: .libraries,pinInterrupt.pem3
.libraries,pinInterrupt.pem3: package/cfg/pinInterrupt_pem3.xdl
	$(MAKE) -f C:\Users\cristian\CC2650\pinInterrupt_CC2650F128/src/makefile.libs

clean::
	$(MAKE) -f C:\Users\cristian\CC2650\pinInterrupt_CC2650F128/src/makefile.libs clean

