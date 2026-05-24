# RMR Baremetal Freestanding Slice

Este diretório implementa uma trilha **freestanding/baremetal** isolada:
- sem `#include`
- sem libc/stdlib
- sem malloc/heap/GC
- foco em registradores + endereços fixos + fluxo determinístico

## Arquivos
- `rmr_baremetal_entry.c`: `_start` naked com asm inline.
- `linker.ld`: layout mínimo para execução em RAM.
- `Makefile`: flags de compilação para minimizar símbolos/fricção.

## Observação
Não integrado ao build Android/UserLAnd. É experimental e isolado.
