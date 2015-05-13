#!/bin/sh

xgettext --from-code=UTF-8 gamemodes/fs.pwn include/fullserver/*.inc -d xyzzydm -C --keyword="__" -p l10n/

msgmerge --no-wrap -U l10n/en.po l10n/xyzzydm.po
