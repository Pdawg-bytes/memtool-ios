# memtool-ios
A custom tool to check basic statistics about your iDevices RAM. Targeted for older iOS that doesn't support vm_stat through the shell.

# Building
To build, either use Xcode on macOS, or clang on Linux. Make sure you build cctools-port and use those llvm extensions.

# Usage
Place the binary anywhere on your device, preferably somewhere in the PATH. Then just run `memtool`! For commands, run `memtool --help`.
