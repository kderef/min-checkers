checkers.exe: main.c checkers.c
	gcc main.c -o chess -nostdlib -s -luser32 -lkernel32 -lgdi32 -lshell32 -e WinMainCRTStartup -mwindows
