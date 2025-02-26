# OS Kernel

This is an OS kernel based of off JamesM's kernel development tutorials

I ended up following the tutorial initially, and fixing a bunch of bugs along the way to get it working...

Everything should be functional, provided you use the right toolchain.

You should be able to build a basic system with everything here.


Requirements to build:

 - Nix Package Manager
 - Basic knowledge of how to use nix shell scripts
 - Git

## Build Instructions
Clone this repo, and `cd` into its directory.


Then run the following commands:
```bash
#!/usr/bin/env bash
nix-shell

make clean
make
```

This should make a runnable iso.

You should be able to run the OS in a VM with `make run`

## TODO
 - update VFS to work better
 - Make keyboard/mouse drivers
 - Add another file system (ext2 or fat32)
 - Attempt to get some sort of user program running.