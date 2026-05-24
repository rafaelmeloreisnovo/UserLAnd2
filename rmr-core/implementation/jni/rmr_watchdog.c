/*
 * rmr_watchdog.c
 * Isolated C component for watchdog/failsafe ideas.
 * Not linked into production build by default.
 */

#include <stdint.h>

static volatile uint32_t rmr_watchdog_counter = 0;
static volatile uint32_t rmr_watchdog_limit = 120;
static volatile uint32_t rmr_failsafe_trip = 0;

void rmr_watchdog_tick(void) {
    uint32_t c = rmr_watchdog_counter + 1u;
    rmr_watchdog_counter = c;
    if (c > rmr_watchdog_limit) {
        rmr_failsafe_trip = 1u;
    }
}

void rmr_watchdog_kick(void) {
    rmr_watchdog_counter = 0u;
    rmr_failsafe_trip = 0u;
}

uint32_t rmr_watchdog_tripped(void) {
    return rmr_failsafe_trip;
}

void rmr_watchdog_set_limit(uint32_t limit) {
    rmr_watchdog_limit = (limit == 0u) ? 1u : limit;
}
