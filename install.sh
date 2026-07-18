#!/bin/bash

# Exit immediately if any step fails
set -e

echo "🚀 Starting installation of 'tch'..."

# 1. Compile the binary using the optimized Makefile setup
echo "📦 Compiling high-performance C++ binary..."
make clean
make

# 2. Install the binary globally into /usr/local/bin
echo "🔧 Installing binary to /usr/local/bin (requires sudo privileges)..."
sudo make install

# 3. Detect the user's current shell configuration file
SHELL_RC=""
if [[ "$SHELL" == */zsh ]]; then
    SHELL_RC="$HOME/.zshrc"
elif [[ "$SHELL" == */bash ]]; then
    SHELL_RC="$HOME/.bashrc"
fi

# 4. Inject the shell fallback function if it doesn't already exist
if [ -n "$SHELL_RC" ] && [ -f "$SHELL_RC" ]; then
    if ! grep -q "tch() {" "$SHELL_RC"; then
        echo "📝 Injecting fallback shell wrapper function into $SHELL_RC..."
        cat << 'EOF' >> "$SHELL_RC"

# Custom high-performance 'tch' routing mechanism
tch() {
    local use_native=false
    for arg in "$@"; do
        case "$arg" in
            -c|--no-create|-d|--date|-h|--no-dereference|-r|--reference|-t)
                use_native=true
                break
                ;;
        esac
    done

    if [ "$use_native" = true ]; then
        command touch "$@"
    else
        /usr/local/bin/tch "$@"
    fi
}
EOF
        echo " Wrapper added successfully."
    else
        echo " Shell wrapper already exists in $SHELL_RC. Skipping injection."
    fi
else
    echo "  Could not automatically detect a standard .bashrc or .zshrc file."
    echo "Please manually add the 'tch()' shell function to your environment profile."
fi

echo "Installation complete! Run 'source $SHELL_RC' or restart your terminal to begin using 'tch'."
