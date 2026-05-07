checkers.exe: main.c checkers.c
	gcc main.c -o checkers -nostdlib -s -luser32 -lkernel32 -lgdi32 -lshell32 -e WinMainCRTStartup -mwindows
