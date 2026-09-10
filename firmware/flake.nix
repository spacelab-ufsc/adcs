{
  description = "Host toolchain for building the RTEMS 6.2 RSB toolchain (see scripts/install-toolchain.sh)";

  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";

  outputs = { self, nixpkgs }:
    let
      system = "x86_64-linux";
      pkgs = import nixpkgs { inherit system; };
    in {
      devShells.${system}.default = pkgs.mkShell {
        buildInputs = with pkgs; [
          gcc13
          gnumake
          bison
          flex
          texinfo
          python3
          ncurses
          zlib
          git
          curl
          gnutar
          gzip
          bzip2
          xz
        ];
      };
    };
}
