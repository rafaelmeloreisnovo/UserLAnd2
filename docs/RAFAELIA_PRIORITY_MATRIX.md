# RAFAELIA · Matriz de voo vetorial

Aplicação operacional do voo vetorial com pesos, tensores, camadas e blendings neste repositório.

## Vetor de avaliação

Cada item técnico deve ser classificado como:

```text
v = (intencao, observacao, ruido, transmutacao, memoria, coerencia, urgencia, latencia, evidencia, obviedade)
```

## Pesos práticos

```text
valor = 0.20*evidencia + 0.18*urgencia + 0.16*memoria + 0.14*transmutacao + 0.12*latencia + 0.08*obviedade + 0.06*intencao + 0.04*ruido + 0.02*coerencia
```

## Prioridades deste repositório

| Camada | Peso | Aplicação em UserLAnd2 | Critério de prova |
|---|---:|---|---|
| Urgente | 0.18 | sessão Linux/terminal iniciar sem travar | apk instala, sessão abre, log sem crash fatal |
| Evidência | 0.20 | build, testes, logs de runtime | comando reproduzível + saída salva |
| Latente | 0.12 | pontos de integração com sensores, developer mode e debugger runtime | arquivos localizados + hipótese marcada |
| Menosprezado | 0.08 | README, SDK mínimo, permissões, ABI, dependências | instrução curta que outro usuário consegue seguir |
| Ruído fértil | 0.04 | erros SDK 29, permissões Android, falhas de bootstrap | erro vira issue/hotfix rastreável |

## Blending por tipo de dado

| Dado | Processamento correto |
|---|---|
| erro de terminal | reproduzir, salvar log, classificar causa |
| arquivo sem extensão | tentar leitura texto, mapear função |
| build Gradle | registrar SDK/NDK/JDK/ABI |
| sensor/API Android | separar permissão, runtime, serviço e chamada nativa |
| script shell | validar com `sh -n`/`bash -n` e execução mínima |

## Lista inicial de execução

1. Localizar ponto real de criação da sessão.
2. Mapear fluxo install -> first run -> bootstrap -> shell.
3. Registrar SDK mínimo, target SDK e dependências críticas.
4. Criar logs claros para falha de sessão.
5. Separar hipótese RAFAELIA de correção Android comprovada.

## Estados

```text
[PROVADO]     compila/executa com log
[PARCIAL]     existe evidência, mas falta reprodução limpa
[HIPOTESE]    ideia coerente sem teste suficiente
[URGENTE]     bloqueia o próximo passo
[LATENTE]     parece importante, mas ainda não nomeado
[OBVIO]       simples, porém estrutural
```

## Retroalimentar[3]

- F_ok: o repositório agora tem critério de triagem.
- F_gap: ainda falta preencher a matriz com arquivos reais após varredura.
- F_next: rodar inventário de árvore, localizar bootstrap e gerar hotfixes verificáveis.
