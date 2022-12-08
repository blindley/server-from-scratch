CFLAGS=/EHsc /nologo
BIN=bin\win32

all: $(BIN)\server.exe

$(BIN)/server.exe: $(BIN)\server.obj
	cl $(CFLAGS) $(BIN)\server.obj /Fe"$(BIN)\server.exe"

$(BIN)/server.obj: src\server.cpp src\socket.h win32.Makefile
	cl $(CFLAGS) /c src\server.cpp /Fo"$(BIN)\server.obj"

clean:
	del $(BIN)\*
