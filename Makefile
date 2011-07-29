stfuify.dylib: stfuify.c
	gcc stfuify.c -Wall -Werror -framework AudioToolbox -lobjc -shared -arch i386 -o stfuify.dylib

clean:
	rm -f stfuify.dylib

run: stfuify.dylib
	sh stfuify.sh
