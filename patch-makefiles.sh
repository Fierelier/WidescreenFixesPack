#!/usr/bin/env sh
sed -i 's/^RESCOMP = windres$/ifeq ($(origin RESCOMP), default)\n  RESCOMP = windres\nendif/' build/*.make
