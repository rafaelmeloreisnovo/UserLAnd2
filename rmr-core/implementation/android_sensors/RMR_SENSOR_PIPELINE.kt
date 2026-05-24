package rmr.core.implementation.android_sensors

import android.content.Context
import android.hardware.Sensor
import android.hardware.SensorEvent
import android.hardware.SensorEventListener
import android.hardware.SensorManager
import java.util.concurrent.atomic.AtomicInteger

/**
 * Isolated Android sensor pipeline (not wired into app runtime by default).
 *
 * Goals:
 * - predictable flow
 * - fail-safe + watchdog hooks
 * - rollback to last stable frame
 */
class RMRSensorPipeline(context: Context) : SensorEventListener {
    private val sensorManager = context.getSystemService(Context.SENSOR_SERVICE) as SensorManager
    private val generation = AtomicInteger(0)

    @Volatile private var lastStable = FloatArray(7) { 0f }
    @Volatile private var current = FloatArray(7) { 0f }
    @Volatile private var token: Int = 0

    fun start(): Boolean {
        val accel = sensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER) ?: return false
        val gyro = sensorManager.getDefaultSensor(Sensor.TYPE_GYROSCOPE) ?: return false
        val okA = sensorManager.registerListener(this, accel, SensorManager.SENSOR_DELAY_GAME)
        val okG = sensorManager.registerListener(this, gyro, SensorManager.SENSOR_DELAY_GAME)
        return okA && okG
    }

    fun stop() {
        sensorManager.unregisterListener(this)
    }

    override fun onSensorChanged(event: SensorEvent) {
        // Minimal allocation-free hot path (reuse current array)
        val v = event.values
        if (event.sensor.type == Sensor.TYPE_ACCELEROMETER && v.size >= 3) {
            current[0] = v[0]
            current[1] = v[1]
            current[2] = v[2]
        } else if (event.sensor.type == Sensor.TYPE_GYROSCOPE && v.size >= 3) {
            current[3] = v[0]
            current[4] = v[1]
            current[5] = v[2]
        }
        current[6] = generation.incrementAndGet().toFloat()

        // Coerência simplificada: token 42 se energia escalar > 0
        val e = (current[0] * 0.8660254f - current[1] * 1.618034f) + (current[3] * current[4])
        token = if (e > 0f) 42 else 0

        // Fail-safe: rollback para último estado estável em token vazio
        if (token == 0) {
            current[0] = lastStable[0]
            current[1] = lastStable[1]
            current[2] = lastStable[2]
            current[3] = lastStable[3]
            current[4] = lastStable[4]
            current[5] = lastStable[5]
        } else {
            lastStable[0] = current[0]
            lastStable[1] = current[1]
            lastStable[2] = current[2]
            lastStable[3] = current[3]
            lastStable[4] = current[4]
            lastStable[5] = current[5]
        }
    }

    override fun onAccuracyChanged(sensor: Sensor?, accuracy: Int) = Unit

    fun snapshotToken(): Int = token
    fun snapshotState(): FloatArray = current.copyOf()
}
