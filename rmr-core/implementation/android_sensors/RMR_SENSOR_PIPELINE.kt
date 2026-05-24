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
 * - broad Android sensor coverage without touching production runtime
 */
class RMRSensorPipeline(context: Context) : SensorEventListener {
    private val sensorManager = context.getSystemService(Context.SENSOR_SERVICE) as SensorManager
    private val generation = AtomicInteger(0)

    @Volatile private var lastStable = FloatArray(16) { 0f }
    @Volatile private var current = FloatArray(16) { 0f }
    @Volatile private var token: Int = 0

    private var accelReady = false
    private var gyroReady = false

    fun availableSensors(): List<Int> {
        return listOf(
            Sensor.TYPE_ACCELEROMETER,
            Sensor.TYPE_GYROSCOPE,
            Sensor.TYPE_MAGNETIC_FIELD,
            Sensor.TYPE_ROTATION_VECTOR,
            Sensor.TYPE_GRAVITY,
            Sensor.TYPE_LINEAR_ACCELERATION,
            Sensor.TYPE_LIGHT,
            Sensor.TYPE_PROXIMITY,
            Sensor.TYPE_PRESSURE,
            Sensor.TYPE_AMBIENT_TEMPERATURE
        ).filter { sensorManager.getDefaultSensor(it) != null }
    }

    fun start(): Boolean {
        val accel = sensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER) ?: return false
        val gyro = sensorManager.getDefaultSensor(Sensor.TYPE_GYROSCOPE) ?: return false
        val okA = sensorManager.registerListener(this, accel, SensorManager.SENSOR_DELAY_GAME)
        val okG = sensorManager.registerListener(this, gyro, SensorManager.SENSOR_DELAY_GAME)

        sensorManager.getDefaultSensor(Sensor.TYPE_MAGNETIC_FIELD)?.also {
            sensorManager.registerListener(this, it, SensorManager.SENSOR_DELAY_GAME)
        }
        sensorManager.getDefaultSensor(Sensor.TYPE_ROTATION_VECTOR)?.also {
            sensorManager.registerListener(this, it, SensorManager.SENSOR_DELAY_GAME)
        }
        sensorManager.getDefaultSensor(Sensor.TYPE_GRAVITY)?.also {
            sensorManager.registerListener(this, it, SensorManager.SENSOR_DELAY_GAME)
        }
        sensorManager.getDefaultSensor(Sensor.TYPE_LINEAR_ACCELERATION)?.also {
            sensorManager.registerListener(this, it, SensorManager.SENSOR_DELAY_GAME)
        }
        sensorManager.getDefaultSensor(Sensor.TYPE_LIGHT)?.also {
            sensorManager.registerListener(this, it, SensorManager.SENSOR_DELAY_GAME)
        }
        sensorManager.getDefaultSensor(Sensor.TYPE_PROXIMITY)?.also {
            sensorManager.registerListener(this, it, SensorManager.SENSOR_DELAY_GAME)
        }
        sensorManager.getDefaultSensor(Sensor.TYPE_PRESSURE)?.also {
            sensorManager.registerListener(this, it, SensorManager.SENSOR_DELAY_GAME)
        }
        sensorManager.getDefaultSensor(Sensor.TYPE_AMBIENT_TEMPERATURE)?.also {
            sensorManager.registerListener(this, it, SensorManager.SENSOR_DELAY_GAME)
        }

        return okA && okG
    }

    fun stop() {
        sensorManager.unregisterListener(this)
    }

    override fun onSensorChanged(event: SensorEvent) {
        val v = event.values
        when (event.sensor.type) {
            Sensor.TYPE_ACCELEROMETER -> if (v.size >= 3) {
                current[0] = v[0]; current[1] = v[1]; current[2] = v[2]; accelReady = true
            }
            Sensor.TYPE_GYROSCOPE -> if (v.size >= 3) {
                current[3] = v[0]; current[4] = v[1]; current[5] = v[2]; gyroReady = true
            }
            Sensor.TYPE_MAGNETIC_FIELD -> if (v.size >= 3) {
                current[6] = v[0]; current[7] = v[1]; current[8] = v[2]
            }
            Sensor.TYPE_ROTATION_VECTOR -> if (v.isNotEmpty()) {
                current[9] = v[0]
            }
            Sensor.TYPE_GRAVITY -> if (v.isNotEmpty()) {
                current[10] = v[0]
            }
            Sensor.TYPE_LINEAR_ACCELERATION -> if (v.isNotEmpty()) {
                current[11] = v[0]
            }
            Sensor.TYPE_LIGHT -> if (v.isNotEmpty()) {
                current[12] = v[0]
            }
            Sensor.TYPE_PROXIMITY -> if (v.isNotEmpty()) {
                current[13] = v[0]
            }
            Sensor.TYPE_PRESSURE -> if (v.isNotEmpty()) {
                current[14] = v[0]
            }
            Sensor.TYPE_AMBIENT_TEMPERATURE -> if (v.isNotEmpty()) {
                current[15] = v[0]
            }
        }

        current[15] = generation.incrementAndGet().toFloat()

        if (!accelReady || !gyroReady) {
            token = 0
            return
        }

        val e =
            (current[0] * 0.8660254f - current[1] * 1.618034f) +
                (current[3] * current[4]) -
                (current[6] * 0.125f)

        token = if (e > 0f) 42 else 0

        if (token == 0) {
            for (i in 0 until current.size) current[i] = lastStable[i]
        } else {
            for (i in 0 until current.size) lastStable[i] = current[i]
        }
    }

    override fun onAccuracyChanged(sensor: Sensor?, accuracy: Int) = Unit

    fun snapshotToken(): Int = token
    fun snapshotState(): FloatArray = current.copyOf()
}
