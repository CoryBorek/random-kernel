{ pkgs ? import (fetchTarball {
  url = "https://github.com/NixOS/nixpkgs/archive/refs/tags/24.05.tar.gz";
}) {} }:
let
  binutilsVersion       = "2.41";
  gccVersion            = "13.2.0";
  target = "i686-elf";
in
pkgs.stdenv.mkDerivation {
  name = "osdev-cross";
  version = "1.0";
  srcs = [
    ( pkgs.fetchurl {
      url = "http://ftpmirror.gnu.org/binutils/binutils-${binutilsVersion}.tar.gz";
      sha256 = "sha256-SNAKjcc6p9I5Sn3Aablhkdlejejw2m3JHaXM5lXCDkU=";
    })
    ( pkgs.fetchurl {
      url = "http://ftpmirror.gnu.org/gcc/gcc-${gccVersion}/gcc-${gccVersion}.tar.gz";
      sha256 = "sha256-jLS+N5ZlGXb5S5NW+gjYM1JPYkINYpLFAzqaJq8xUHg=";
    })
  ];
  buildInputs = [
    pkgs.gcc
    pkgs.wget
    pkgs.which
    pkgs.rsync
    pkgs.gmp
    pkgs.libmpc
    pkgs.mpfr
    pkgs.bison
    pkgs.flex 
    pkgs.gnumake
    pkgs.texinfo
  ];
  # I believe the following prevents gcc from treating "-Werror=format-security"
  # warnings as errors
  hardeningDisable = [ "format" ];
  sourceRoot = ".";
  buildPhase = ''
    echo $PWD
    ls -lah .
    # binutils
    mkdir build-binutils
    cd build-binutils
    ../binutils-${binutilsVersion}/configure \
    --target=${target} \
    --prefix="$out" \
    --with-sysroot \
    --disable-nls \
    --disable-werror
    make -j$(nproc)
    make install
    cd ..
    # gcc stage 1
    mkdir build-gcc
    cd build-gcc
    ../gcc-${gccVersion}/configure \
    --target=${target} \
    --prefix="$out" \
    --disable-nls \
    --enable-languages=c,c++ \
    --without-headers \
    --disable-hosted-libstdcxx
    make -j$(nproc) all-gcc
    make -j$(nproc) all-target-libgcc
    make -j$(nproc) all-target-libstdc++-v3
    make install-gcc
    make install-target-libgcc
    make install-target-libstdc++-v3
    
    cd ..
    mkdir -p $out/${target}/lib
    mkdir -p $out/sysroot/lib
    rsync -a $out/${target}/lib/ $out/sysroot/lib
  '';
  meta = {
    description = "Cross-compilation toolchain for osdev";
    homepage = "https://agentdid127.com/";
    license = pkgs.lib.licenses.gpl2;
    maintainers = with pkgs.stdenv.lib.maintainers; [ ];
  };
}