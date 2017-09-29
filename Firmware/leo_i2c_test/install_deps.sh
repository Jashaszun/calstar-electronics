#!/bin/bash
set -e
set -u

if ! command -v brew &>/dev/null; then
  echo "Homebrew not installed, exiting..."
  exit 0
fi

if ! command -v avr-gcc &>/dev/null; then
  echo "Installing avr-gcc with brew"
  brew tap osx-cross/avr
  brew install osx-cross/avr/avr-gcc
else
  echo "avr-gcc already installed"
fi

if ! command -v avrdude &>/dev/null; then
  echo "Installing avrdude with brew"
  brew install avrdude --with-usb
else
  echo "avrdude already installed"
fi
