set -e

command -v "xcode-select" >/dev/null 2>&1 || { echo >&2 "You must install Xcode"; exit 1; }

xcode-select --install && { echo >&2 "FOLLOW ONSCREEN INSTRUCTIONS THEN RE-RUN MAKE"; exit 1; }

command -v brew >/dev/null 2>&1 || { echo >&2 "Installing Homebrew"; /usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"; }
HASTAP=$(brew tap | grep osx-cross/avr | head -n 1)
if [[ ! $HASTAP ]]; then
	echo "Tapping osx-cross/avr"
	brew tap osx-cross/avr
fi
command -v "brew ls --versions avr-gcc" >/dev/null 2>&1 || { echo >&2 "Installing avr-gcc"; brew install avr-gcc; }
command -v "brew ls --versions avrdude" >/dev/null 2>&1 || { echo >&2 "Installing avrdude"; brew install avrdude; }

BASEDIR=$(dirname "$0")
touch $BASEDIR/INSTALLED
