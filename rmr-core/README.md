# RMR Core (Autoral Plugin Kernel)

Este diretório contém um **núcleo separado e autoral** para experimentação de arquitetura low-level/modular, sem integração automática com o build Android atual.

## Objetivos
- Separar fisicamente os módulos do núcleo RMR.
- Preservar compilação existente do repositório principal.
- Manter documentação + código fonte juntos.
- Definir fronteiras de legalidade/licença e governança técnica.

## Não interferência
- Não altera `settings.gradle`, `build.gradle`, `app/`, `termux-app/`.
- Não altera bootstrap/kernel/userland atual do app.
- É um espaço independente para evolução modular.

## Estrutura
- `legal/` → licença e aviso de separação.
- `docs/` → arquitetura, conformidade, processo de implantação.
- `core/` → especificação do núcleo e interfaces base.
- `modules/` → módulos plugáveis separados por responsabilidade.

## Estado
- Fase atual: **design e documentação executável**.
- Próxima fase: implementação incremental por módulo com gates de validação.
