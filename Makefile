chess.exe: chess.c
	gcc chess.c -o chess -nostdlib -s -luser32 -lkernel32 -e WinMainCRTStartup -mwindows
