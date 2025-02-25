with import <nixpkgs> {};
mkShell {
  buildInputs = [ 
      bochs
      qemu
    ];
}
