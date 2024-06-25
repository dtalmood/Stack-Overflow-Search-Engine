#!/bin/bash

# Function to check if a command exists
command_exists() {
  command -v "$1" >/dev/null 2>&1
}

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

# Install MongoDB C++ Driver (mongocxx)
echo "Installing MongoDB C++ Driver (mongocxx)..."
brew install mongocxx

# Check if cmake is installed, if not, install it
echo "Checking if cmake is installed..."
if ! command_exists cmake; then
  echo "Installing cmake..."
  brew install cmake
fi

# Clone libbcrypt (requires sudo or appropriate permissions)
echo "Cloning libbcrypt..."
sudo git clone https://github.com/trusch/libbcrypt /usr/local/libbcrypt

# Build libbcrypt
echo "Building libbcrypt..."
cd /usr/local/libbcrypt
mkdir build && cd build
cmake ..
make
sudo make install

# Inform user about next steps
echo "Installation completed successfully."
echo "You can now proceed with building and running your Stack Overflow Search Engine project."
