# RMR Core Deployment Plan (Safe, Non-Breaking)

## Fase 0 — isolamento (concluída)
- Criar diretório dedicado `rmr-core/`.
- Definir licença separada e notice.
- Definir especificações e registro de módulos.

## Fase 1 — protótipo local
- Implementar cada módulo em diretório próprio sem conectar ao build principal.
- Criar scripts de validação locais opcionais.

## Fase 2 — integração opcional
- Adicionar integração via flag explícita (opt-in), nunca default.
- Validar compilação Android existente antes/depois.

## Fase 3 — readiness
- Auditoria técnica de segurança, privacidade, governança.
- Revisão legal antes de distribuição externa.
