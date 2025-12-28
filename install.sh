#!/bin/bash
echo "Installing SBPI dependencies..."
sudo apt install git gcc libcurl4-openssl-dev  # For Debian/Ubuntu
# or sudo dnf install git gcc libcurl-devel    # For Fedora/RHEL

echo "Cloning SBPI..."
git clone https://github.com/orshane/sbpi.git
cd sbpi/src

echo "Building SBPI..."
make

echo "Installing SBPI..."
sudo cp sbpi /usr/local/bin/

echo "SBPI installed successfully!"
