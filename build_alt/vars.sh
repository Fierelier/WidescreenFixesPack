if [ "$AR" = "" ]; then AR=i686-w64-mingw32-ar; fi
if [ "$CC" = "" ]; then CC=i686-w64-mingw32-gcc; fi
if [ "$CXX" = "" ]; then CXX=i686-w64-mingw32-g++; fi
if [ "$J" = "" ]; then J=$(nproc); fi
if [ "$RESCOMP" = "" ]; then
	if command -v i686-w64-mingw32-windres; then
		RESCOMP=i686-w64-mingw32-windres
	else
		RESCOMP=windres
	fi
fi

CONCURRENT_TASKS=0
concurrent() {
	CONCURRENT_TASKS=$((CONCURRENT_TASKS + 1))
	while [ "$CONCURRENT_TASKS" -gt "$J" ]; do
		wait -n
		CONCURRENT_TASKS=$((CONCURRENT_TASKS - 1))
	done
	"$@" &
}

concurrent_wait() {
	if [ $J -gt 1 ]; then
		wait
	fi
}

if [ $J -gt 1 ]; then
	AR="concurrent $AR"
	CC="concurrent $CC"
	CXX="concurrent $CXX"
	RESCOMP="concurrent $RESCOMP"
fi
