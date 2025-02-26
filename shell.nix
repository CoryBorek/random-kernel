with import <nixpkgs> {};
let
  osdev-toolchain = callPackage ./tools/osdev-toolchain.nix {};
in
mkShell {
  buildInputs = [ 
      bochs
      qemu
      lazygit
      nasm
      xorriso
      grub2
      wget
      gnumake
      valgrind
      gdb
      parted
      osdev-toolchain
    ];
}
