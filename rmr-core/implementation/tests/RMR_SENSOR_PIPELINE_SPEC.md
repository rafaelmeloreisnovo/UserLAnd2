# RMR Sensor Pipeline Test Spec (manual/static)

## Objective
Validate rollback/failsafe/watchdog behavior without integrating into UserLAnd runtime.

## Cases
1. Accelerometer + gyroscope publish values and update generation.
2. Positive energy emits token 42 and updates `lastStable`.
3. Non-positive energy emits token 0 and rolls back to `lastStable`.
4. Watchdog trips after `limit + 1` ticks without kick.
5. Watchdog clears after kick.
