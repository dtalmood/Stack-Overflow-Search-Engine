#!/bin/bash

# Function to check if a command exists
command_exists() {
  command -v "$1" >/dev/null 2>&1
}

# Check if Homebrew is installed, if not, install it
echo "Checking If homebrew is Already Installed..."
if ! command_exists brew; then
  echo "Installing Homebrew..."
  /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
fi

# Install MongoDB and start it as a service
echo "Installing MongoDB..."
brew tap mongodb/brew
brew install mongodb-community
brew services start mongodb/brew/mongodb-community

# Verify MongoDB is running
echo "Verifying MongoDB status..."
if ! ps aux | grep -v grep | grep mongod >/dev/null; then
  echo "MongoDB failed to start. Please check the installation."
  exit 1
fi

# Install Bcrypt
echo "Installing Bcrypt..."
brew install bcrypt

# Install MongoDB C++ Driver (mongocxx)
echo "Installing MongoDB C++ Driver (mongocxx)..."
brew install mongocxx

# Inform user about next steps
echo "Installation completed successfully."
echo "You can now proceed with building and running your Stack Overflow Search Engine project."
