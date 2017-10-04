#!/bin/sh

OS=$(uname -s)
AVR_GCC_PATH=$(which avr-gcc)
AVRDUDE_PATH=$(which avrdude)

if [ "$AVR_GCC_PATH" = "" ] || [ "$AVRDUDE_PATH" = "" ]; then
	printf "Resolving missing dependencies\n" >&2
else
	exit 0
fi

if [ "$OS" = "Darwin" ]; then
	which brew || (printf "Homebrew not installed, exiting...\n" >&2 && exit 1)
	
	if [ "$AVR_GCC_PATH" = "" ]; then
		brew tap osx-cross/avr && brew install osx-cross/avr/avr-gcc || exit 1
	fi
	
	if [ "$AVRDUDE_PATH" = "" ]; then
		brew install avrdude --with-usb || exit 1
	fi
	exit 0
elif [ "$OS" = "Linux" ]; then
	printf "Install the following packages:\n" >&2
	printf "gcc-avr gdb-avr avrdude avr-libc"
	printf "\n" >&2	
	printf "Try:\n> apt install \`./install_deps.sh\`\n"
	exit 1
else
	printf "Could not determine operating system\nI hope it's not windows...\n"
fi
