SBPI - Source Based Package Installer

https://img.shields.io/badge/SBPI-Source_Based_Package_Installer-blue
https://img.shields.io/badge/License-MIT-green
https://img.shields.io/badge/Language-C-orange

SBPI is a fast, lightweight, source-based package manager inspired by FreeBSD Ports. Designed for advanced users who want complete control over their software installation process.
🚀 Philosophy

SBPI follows the Unix philosophy: simple tools that do one thing well. Unlike traditional package managers, SBPI:

    Does NOT handle dependencies automatically - You control what gets installed

    Does NOT manage file locations - Software installs where its build system decides

    Is NOT a software setup program - It's just a software installer

    Gives YOU complete control - From source to installation

✨ Features

    Extremely fast - Written in C, minimal resource usage

    Extremely customizable - Modify any port's build process

    User-controlled - You decide dependencies and installation paths

    Source-based - Build software from source, not binaries

    Ports system - Inspired by FreeBSD's renowned ports collection

    Lightweight - The entire binary is just a few kilobytes

📦 Quick Start
Installation
Prerequisites:
bash

# Ubuntu/Debian
sudo apt install git gcc libcurl4-openssl-dev

# Fedora/RHEL/CentOS
sudo dnf install git gcc libcurl-devel

# Arch Linux
sudo pacman -S git gcc curl

Build and Install:
bash

# Clone the repository
git clone https://github.com/orshane/sbpi
cd sbpi

# Build SBPI
make

# Install to system
sudo cp sbpi /usr/local/bin/

# Verify installation
sbpi --help

One-line Installer:
bash

curl -sSL https://raw.githubusercontent.com/orshane/sbpi/main/install.sh | bash

🛠️ Usage
Basic Commands:
bash

# Sync port definitions from repository
sbpi sync

# List available ports
sbpi list

# Install a port
sbpi install editors/micro

# Remove a port
sbpi remove editors/micro

# Update a port
sbpi update editors/micro

Example Workflow:
bash

# First, sync the port repository
sbpi sync

# See what's available
sbpi list

# Install some software
sbpi install editors/vim
sbpi install utils/htop
sbpi install lang/python

📁 Repository Structure

SBPI uses a ports system similar to FreeBSD. Ports are organized in a repository:
text

SbpRepo/
├── editors/          # Text editors
│   ├── micro/Sbp
│   └── vim/Sbp
├── lang/             # Programming languages
│   ├── python/Sbp
│   └── nodejs/Sbp
├── utils/            # Utilities
│   ├── htop/Sbp
│   └── tmux/Sbp
└── apps/             # Applications
    └── myapp/Sbp

📝 Sbp File Format

Each port has a Sbp file that defines how to build and install it:
bash

PORTNAME=package-name
VERSION=1.0.0
FETCH_CMD=command_to_download_source
BUILD_CMD=command_to_build
INSTALL_CMD=command_to_install
UNINSTALL_CMD=command_to_uninstall

Example Sbp file (editors/micro):
bash

PORTNAME=micro
VERSION=2.0.12
FETCH_CMD=curl -L https://github.com/zyedidia/micro/archive/v2.0.12.tar.gz -o /usr/local/var/sbpi/distfiles/micro-2.0.12.tar.gz
BUILD_CMD=tar -xzf /usr/local/var/sbpi/distfiles/micro-2.0.12.tar.gz -C /usr/local/var/sbpi/work && cd /usr/local/var/sbpi/work/micro-2.0.12 && make
INSTALL_CMD=cd /usr/local/var/sbpi/work/micro-2.0.12 && sudo make install
UNINSTALL_CMD=sudo rm -f /usr/local/bin/micro

⚠️ Important Notes
1. Dependency Management

SBPI does not handle dependencies automatically. You must install build dependencies before installing ports.

Example for htop:
bash

# Install ncurses development libraries first
sudo apt install libncursesw6-dev    # Debian/Ubuntu
sudo dnf install ncurses-devel       # Fedora/RHEL

2. Installation Locations

SBPI runs the commands in the Sbp file. Where software installs depends on its build system:

    Some install to /usr/local/bin

    Some install to $HOME/go/bin (Go programs)

    Some install to custom locations

3. For Advanced Users

SBPI is designed for users who:

    Want complete control over software installation

    Don't mind installing dependencies manually

    Understand build systems and compilation

    Prefer source-based installations

🔧 Building from Source
Directory Structure:
text

sbpi/
├── main.c          # Main program entry point
├── cli.c           # CLI command implementations
├── port.c          # Port management functions
├── exec.c          # Command execution helpers
├── cli.h           # CLI function declarations
├── port.h          # Port function declarations
├── exec.h          # Execution function declarations
└── Makefile        # Build configuration

Build Options:
bash

# Debug build
make debug

# Release build (optimized)
make

# Clean build files
make clean

# Install system-wide
sudo make install

🎯 Why SBPI?
Feature	SBPI	Traditional Package Managers
Control	✅ Full user control	❌ Limited by maintainers
Speed	✅ Extremely fast	⚠️ Varies
Transparency	✅ Full source visibility	⚠️ Binary packages only
Customization	✅ Modify any build process	❌ Fixed packages
Dependencies	⚠️ User manages	✅ Automatic
Ease of Use	⚠️ Requires knowledge	✅ Beginner-friendly
🤝 Contributing

Want to add a port? It's easy!

    Fork the SbpRepo

    Create a new directory for your port (e.g., apps/myapp/)

    Add a Sbp file with build instructions

    Submit a pull request

📄 License

MIT License - see LICENSE file for details.
🐛 Troubleshooting
Common Issues:

    "Failed to sync repository"

        Check internet connection

        Ensure git is installed

        Verify repository URL in cli.c

    "Failed to build port"

        Install required build dependencies

        Check the Sbp file for errors

        Try building manually to debug

    "Command not found" after installation

        Software may have installed to non-standard location

        Check $HOME/go/bin, $HOME/.local/bin, etc.

        Add appropriate directories to your PATH

🌟 Supported Systems

    Linux (primary)

    Should work on BSD systems with minor modifications

    macOS with Homebrew-installed dependencies

SBPI: Because sometimes you want to build it yourself. 🛠️

"Give me the source, and I'll build the world." - SBPI Philosophy
