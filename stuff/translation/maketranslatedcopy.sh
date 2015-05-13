#!/bin/sh

echo "removing old data"
rm -rf "l10n_en"
echo "making new copy"
mkdir "l10n_en"
cp -rua gamemodes/ include/ l10n_en/
cd l10n_en/
echo "applying translation"
../l10n/geninc.pl < ../l10n/en.po
echo "done"
