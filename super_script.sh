#!/usr/bin/env bash

function show_usage {
	echo "Usage: $0 ( ls | count_lines | update_l11n | compile_l11n | add_license_headers | replace_license_headers)"
	exit
}

if [ -z "$1" ]; then
	show_usage
fi

CXX_FILES=`find include/ src/ | grep -P '.*\.(cpp|h)$' | grep -v 'CGUITTFont'`\ main.cpp

if [ "$1" == "ls" ]; then
	for file in $CXX_FILES; do
		echo $file
	done
elif [ "$1" == "count_lines" ]; then
	wc -l $CXX_FILES
elif [ "$1" == "update_l11n" ]; then
	xgettext -k_wp -k_w -k_ $CXX_FILES -o planerunner.pot
	for locale in media/locale/*; do
		msgmerge $locale/LC_MESSAGES/planerunner.po planerunner.pot -U
	done
elif [ "$1" == "compile_l11n" ]; then
	for locale in media/locale/*; do
		msgfmt $locale/LC_MESSAGES/planerunner.po -o $locale/LC_MESSAGES/planerunner.mo
	done
elif [ "$1" == "add_license_headers" ]; then
	for file in $CXX_FILES; do
		LICENSE_LINES=`wc -l < license_header`
		if ! diff license_header <(head -n $LICENSE_LINES "$file") > /dev/null; then
			cat license_header "$file" > "$file.new"
			mv "$file.new" "$file"
		fi
	done
elif [ "$1" == "replace_license_headers" ]; then # use it carefully!!
	for file in $CXX_FILES; do
		LICENSE_LINES=`wc -l < license_header`
		if ! diff license_header <(head -n $LICENSE_LINES "$file") > /dev/null; then
			cat license_header > "$file.new"
			tail -n +`expr $LICENSE_LINES + 1` "$file" >> "$file.new"
			mv "$file.new" "$file"
		fi
	done
else
	show_usage
fi
