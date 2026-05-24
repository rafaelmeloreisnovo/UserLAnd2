/*
 * rmr_watchdog.c
 * Isolated C component for watchdog/failsafe ideas.
 * Not linked into production build by default.
 */

static volatile unsigned int rmr_watchdog_counter = 0;
static volatile unsigned int rmr_watchdog_limit = 120;
static volatile unsigned int rmr_failsafe_trip = 0;

void rmr_watchdog_tick(void) {
    unsigned int c = rmr_watchdog_counter + 1u;
    rmr_watchdog_counter = c;
    if (c > rmr_watchdog_limit) {
        rmr_failsafe_trip = 1u;
    }
}

void rmr_watchdog_kick(void) {
    rmr_watchdog_counter = 0u;
    rmr_failsafe_trip = 0u;
}

unsigned int rmr_watchdog_tripped(void) {
    return rmr_failsafe_trip;
}

void rmr_watchdog_set_limit(unsigned int limit) {
    rmr_watchdog_limit = (limit == 0u) ? 1u : limit;
}
