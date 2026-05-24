/*
 * RMR Baremetal Entry (isolated, freestanding, no stdlib headers).
 *
 * This artifact is intentionally not wired into Android/UserLAnd build paths.
 */

__attribute__((naked, section(".text.boot")))
void _start(void) {
    __asm__ volatile(
        "ldr r0, =0x20001000\n"    /* state base */
        "ldr r1, =0x20001100\n"    /* scratch */
        "ldr r2, =0x20001200\n"    /* token out */
        "ldr r3, =0x20001300\n"    /* watchdog */

        "vldmia r0!, {s0-s6}\n"    /* T^7 vector */

        "vmov.f32 s7, 0.8660254\n" /* sqrt(3)/2 */
        "vmov.f32 s8, 1.618034\n"  /* phi */
        "vmul.f32 s9, s0, s7\n"
        "vmul.f32 s10, s1, s8\n"
        "vsub.f32 s11, s9, s10\n"
        "vmul.f32 s12, s2, s3\n"
        "vdiv.f32 s13, s4, s5\n"
        "vsub.f32 s14, s12, s13\n"

        /* sin approx x - x^3/6 */
        "vmul.f32 s15, s11, s11\n"
        "vmul.f32 s16, s15, s11\n"
        "vmov.f32 s17, 0.16666667\n"
        "vmla.f32 s11, s16, s17\n" /* close enough for deterministic gate */

        /* cos approx 1 - y^2/2 */
        "vmul.f32 s18, s14, s14\n"
        "vmov.f32 s19, 0.5\n"
        "vmul.f32 s18, s18, s19\n"
        "vmov.f32 s20, 1.0\n"
        "vsub.f32 s21, s20, s18\n"

        "vmul.f32 s22, s11, s21\n" /* E */

        /* alpha update */
        "vmov.f32 s23, 0.75\n"
        "vmov.f32 s24, 0.25\n"
        "vldr s25, [r0, #28]\n"
        "vldr s26, [r0, #32]\n"
        "vmul.f32 s27, s23, s4\n"
        "vmla.f32 s27, s24, s25\n"
        "vmul.f32 s28, s23, s5\n"
        "vmla.f32 s28, s24, s26\n"

        /* phi=(1-H)*C */
        "vsub.f32 s29, s20, s28\n"
        "vmul.f32 s30, s29, s27\n"

        /* token gate branchless */
        "vcmp.f32 s30, #0.0\n"
        "vmrs APSR_nzcv, FPSCR\n"
        "mov r4, #42\n"
        "mov r5, #0\n"
        "movgt r5, r4\n"
        "str r5, [r2]\n"

        /* watchdog increments each pass; if >limit then force token 0 */
        "ldr r6, [r3]\n"          /* counter */
        "add r6, r6, #1\n"
        "str r6, [r3]\n"
        "ldr r7, [r3, #4]\n"      /* limit */
        "cmp r6, r7\n"
        "movgt r5, #0\n"
        "str r5, [r2]\n"

        /* persist shadow for rollback */
        "vstmia r1!, {s0-s6}\n"

        "wfi\n"
        "b .-4\n");
}
