if [ "$AR" = "" ]; then AR=i686-w64-mingw32-gcc-ar; fi
if [ "$CC" = "" ]; then CC=i686-w64-mingw32-gcc; fi
if [ "$CXX" = "" ]; then CXX=i686-w64-mingw32-g++; fi
if [ "$RESCOMP" = "" ]; then
	if command -v i686-w64-mingw32-windres; then
		RESCOMP=i686-w64-mingw32-windres
	else
		RESCOMP=windres
	fi
fi
if [ "$J" = "" ]; then J=$(nproc); fi

CONCURRENT_BROKEN=0
if ! [ "$WINDIR" = "" ]; then
	echo "WINDIR set, assuming broken jobcontrol (busybox-w32)"
	CONCURRENT_BROKEN=1
fi

concurrent_nowaitn() {
	echo "'wait -n' not available, using file-based jobcontrol."
	CONCURRENT_BROKEN=1
}

if [ $CONCURRENT_BROKEN = 1 ]; then
	CONCURRENT_FILE="$PWD/jobs$$.tmp"
fi

if [ $J -gt 1 ]; then
	if [ $CONCURRENT_BROKEN = 0 ]; then
		: &
		wait -n || concurrent_nowaitn
	fi
fi

concurrent_jc_dump() {
	jobs > "$CONCURRENT_FILE"
}

concurrent_jc_get() {
	wc -l < "$CONCURRENT_FILE"
}

concurrent_jc_rm() {
	rm "$CONCURRENT_FILE" || true
}

CONCURRENT_TASKS=0
concurrent() {
	if [ $CONCURRENT_BROKEN = 0 ]; then
		CONCURRENT_TASKS=$((CONCURRENT_TASKS + 1))
		while [ $CONCURRENT_TASKS -gt $J ]; do
			wait -n
			CONCURRENT_TASKS=$((CONCURRENT_TASKS - 1))
		done
	else
		concurrent_jc_dump # this first dump is to get rid of uncollected jobs
		concurrent_jc_dump
		JOBS=$(concurrent_jc_get)
		while [ $JOBS -gt $J ]; do
			sleep 1
			concurrent_jc_dump
			JOBS=$(concurrent_jc_get)
		done
	fi
	"$@" &
}

concurrent_wait() {
	if [ $J -gt 1 ]; then
		wait
		if [ $CONCURRENT_BROKEN = 1 ]; then
			concurrent_jc_rm
		else
			CONCURRENT_TASKS=0
		fi
	fi
}

if [ $J -gt 1 ]; then
	AR="concurrent $AR"
	CC="concurrent $CC"
	CXX="concurrent $CXX"
	RESCOMP="concurrent $RESCOMP"
fi
