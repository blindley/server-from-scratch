CFLAGS=/EHsc /nologo
BIN=bin\win32

all: $(BIN)\server.exe

$(BIN)/server.exe: $(BIN)\sock_init.obj
	cl $(CFLAGS) $(BIN)\sock_init.obj /Fe"$(BIN)\server.exe"

$(BIN)/sock_init.obj: src\sock_init.cpp win32.Makefile
	cl $(CFLAGS) /c src\sock_init.cpp /Fo"$(BIN)\sock_init.obj"

clean:
	del $(BIN)\*
