#!/bin/bash
#
# Script Name: script_name.sh
# Description: Brief description of the script's purpose.
# Author: Albert Domingo
# GitHub: https://github.com/AlbertDM
#
# Additional Information:
# - Add any additional information or notes about the script here.
# - You can provide details on usage, dependencies, or any other relevant information.
#
# Usage:
# - Provide instructions on how to use the script, including command-line arguments, if any.
#
# Example:
# - Show an example of how to run the script, demonstrating its usage.
#

# Start writing your script here...


# Function to check if a command is available
command_exists() {
  command -v "$1" >/dev/null 2>&1
}

# Detect package manager
if command_exists apt-get; then
  package_manager="apt-get"
elif command_exists dnf; then
  package_manager="dnf"
elif command_exists yum; then
  package_manager="yum"
elif command_exists pacman; then
  package_manager="pacman"
else
  echo "Unable to detect package manager. Please install liburing manually."
  exit 1
fi

# Check if liburing is installed
if command_exists pkg-config && pkg-config --exists liburing; then
  echo "liburing is already installed."
else
  echo "liburing is not installed. Proceeding with installation."

  # Install liburing based on package manager
  case $package_manager in
    "apt-get")
      sudo apt-get install -y liburing-dev
      ;;
    "dnf"|"yum")
      sudo dnf install -y liburing-devel
      ;;
    "pacman")
      sudo pacman -S --needed liburing
      ;;
    *)
      echo "Unsupported package manager. Please install liburing manually."
      exit 1
      ;;
  esac

  echo "liburing has been successfully installed."
fi

