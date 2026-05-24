# Integration Guardrails (Do Not Break UserLAnd)

- Current implementation is isolated under `rmr-core/implementation/`.
- No changes to:
  - `settings.gradle`
  - `build.gradle` / `app/build.gradle`
  - `app/src/main/**`
  - `termux-app/**`
- Any future wiring must be opt-in behind a feature flag and a dedicated build variant.
- Rollback strategy: keep previous stable sensor frame and emit token 0 on invalid state.
- Fail-safe strategy: watchdog trip disables downstream action path.
