# RMR Sensor Pipeline Test Spec (manual/static)

## Objective
Validate rollback/failsafe/watchdog behavior without integrating into UserLAnd runtime.

## Cases
1. Required sensors (accelerometer + gyroscope) gate pipeline start.
2. Optional sensors register when available: magnetic field, rotation vector, gravity,
   linear acceleration, light, proximity, pressure, ambient temperature.
3. Positive energy emits token 42 and updates `lastStable`.
4. Non-positive energy emits token 0 and rolls back to `lastStable`.
5. Startup phase before accel+gyro readiness must emit token 0.
6. Watchdog trips after `limit + 1` ticks without kick.
7. Watchdog clears after kick.
