#!/usr/bin/env sh
set -e

tac_inplace() {
	tac "$1" > "$1.tmp"
	mv "$1.tmp" "$1"
}

create_base() {
	echo '#!/usr/bin/env sh' > "$1"
	echo 'set -e' >> "$1"
	echo '. ./vars.sh' >> "$1"
}

add_concurrency() {
	sed -i '/^:$/d' "$1"
	tac_inplace "$1"
	sed -i "2a\\
concurrent_wait
" "$1"
	tac_inplace "$1"
	echo "concurrent_wait" >> "$1"
}

produce_sh() {
	SUITE="$1"
	make -f "$SUITE.make" clean
	
	create_base "../build_alt/$SUITE.sh"
	make -f "$SUITE.make" -n >> "../build_alt/$SUITE.sh"
	add_concurrency "../build_alt/$SUITE.sh"
	chmod +x "../build_alt/$SUITE.sh"
	
	create_base "../build_alt/debug_$SUITE.sh"
	make -f "$SUITE.make" config=debug_win32 -n >> "../build_alt/debug_$SUITE.sh"
	add_concurrency "../build_alt/debug_$SUITE.sh"
	chmod +x "../build_alt/debug_$SUITE.sh"
}

wine premake5.exe gmake
./patch-makefiles.sh

export AR='$$AR'
export CC='$$CC'
export CXX='$$CXX'
export RESCOMP='$$RESCOMP'

cd build
produce_sh NFSUnderground.WidescreenFix
produce_sh NFSUnderground2.WidescreenFix
produce_sh NFSMostWanted.WidescreenFix
produce_sh NFSCarbon.WidescreenFix
produce_sh NFSProStreet.FusionFix
produce_sh GTA3.WidescreenFix
produce_sh GTAVC.WidescreenFix
