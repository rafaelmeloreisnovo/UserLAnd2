# RMR Kernel Specification (Modular Authorial Core)

## Princípios
1. Separação física de módulos.
2. Interface estável entre camadas.
3. Token vazio preferível a saída sem verdade verificável.
4. Compatibilidade com build principal preservada por isolamento.

## Camadas
- L0: Memory/Address Contract.
- L1: Math/Topology Contract (T^7 mapping + coherence gates).
- L2: Integrity Contract (xor/crc-like verification).
- L3: Execution Contract (branch-minimal deterministic flow).
- L4: Compliance Metadata Contract (proveniência, trilha de auditoria).

## Contrato mínimo de módulo
Cada módulo deve publicar:
- `module_id`
- `input_contract`
- `output_contract`
- `failure_mode` (incluindo TOKEN_VOID)
- `compliance_tags`
