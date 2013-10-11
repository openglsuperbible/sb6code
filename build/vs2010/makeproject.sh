#!/bin/sh

sed "s|%PROJECT%|"$1"|g" < template.vcxproj > $1.vcxproj
sed "s|%PROJECT%|"$1"|g" < template.vcxproj.filters > $1.vcxproj.filters
