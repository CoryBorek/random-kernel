with import <nixpkgs> {};
let
  osdev-cross = callPackage ./tools/osdev-cross.nix {};
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
      osdev-cross
    ];
}
