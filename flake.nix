{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
  };
  outputs = { self, nixpkgs }: 
  {
    devShells.x86_64-linux.default = 
      let
        pkgs = import nixpkgs { system = "x86_64-linux"; };
      in
      pkgs.mkShell {
        buildInputs = with pkgs; [
          gcc
          gdb
        ];
        
        shellHook = ''
          cat > .clangd << EOF
CompileFlags:
  Add:
    - "-I${pkgs.glibc.dev}/include"
    - "-I${pkgs.gcc.cc.lib}/lib/gcc/x86_64-unknown-linux-gnu/${pkgs.gcc.cc.version}/include"
EOF
        '';
      };
  };
}
