/*
 * RAFAELIA trickstopathcutter firmware seed (freestanding/baremetal).
 *
 * This file is intentionally isolated from the Android/UserLAnd runtime.
 * It is a standalone baremetal reference artifact and is not linked into
 * the app build.
 *
 * Compliance intent: keep the change non-invasive to production paths,
 * preserve repository licensing boundaries, and support auditable review.
 *
 * Build example (Cortex-M4F):
 * arm-none-eabi-gcc -x c -std=c11 -ffreestanding -nostdlib -nostartfiles \
 *   -nodefaultlibs -fno-builtin -fno-exceptions -fno-stack-protector \
 *   -fomit-frame-pointer -fno-strict-aliasing -fsingle-precision-constant \
 *   -fno-tree-vectorize -fno-unroll-loops -march=armv7e-m -mtune=cortex-m4 \
 *   -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Os -Wl,--gc-sections \
 *   -Wl,-T,linker.ld -o firmware.elf docs/RAFAELIA_trickstopathcutter_firmware.c
 */

__attribute__((naked, section(".text.boot")))
void _start(void) {
    __asm__ volatile(
        /* Base pointers: state in 0x20000000, scratch 0x20000100, token 0x20000200 */
        "ldr r0, =0x20000000\n"
        "ldr r1, =0x20000100\n"
        "ldr r7, =0x20000200\n"

        /* s0..s6 <= (u,v,psi,chi,rho,delta,sigma) */
        "vldmia r0!, {s0-s6}\n"

        /* Toroidal core */
        "vmov.f32 s7, 0.8660254\n"   /* sqrt(3)/2 */
        "vmov.f32 s8, 1.618034\n"    /* phi */
        "vmul.f32 s9, s0, s7\n"      /* u*sqrt(3)/2 */
        "vmul.f32 s10, s1, s8\n"     /* v*phi */
        "vsub.f32 s11, s9, s10\n"    /* dtheta */
        "vmul.f32 s12, s2, s3\n"     /* psi*chi */
        "vdiv.f32 s13, s4, s5\n"     /* rho/delta */
        "vsub.f32 s14, s12, s13\n"   /* dphi */

        /* Branchless approximations:
         * sin(x) ~= x - x^3/6 ; cos(x) ~= 1 - x^2/2
         */
        "vmul.f32 s15, s11, s11\n"   /* x^2 */
        "vmul.f32 s16, s15, s11\n"   /* x^3 */
        "vmov.f32 s17, 0.16666667\n"
        "vmul.f32 s16, s16, s17\n"
        "vsub.f32 s18, s11, s16\n"   /* sin approx */

        "vmul.f32 s19, s14, s14\n"   /* y^2 */
        "vmov.f32 s20, 0.5\n"
        "vmul.f32 s19, s19, s20\n"
        "vmov.f32 s21, 1.0\n"
        "vsub.f32 s22, s21, s19\n"   /* cos approx */

        "vmul.f32 s23, s18, s22\n"   /* E = sin(dtheta)*cos(dphi) */

        /* Coherence update */
        "vmov.f32 s24, 0.75\n"
        "vmov.f32 s25, 0.25\n"
        "vldr s26, [r0, #28]\n"      /* C_in */
        "vldr s27, [r0, #32]\n"      /* H_in */
        "vmul.f32 s28, s24, s4\n"    /* C_t from prior slot */
        "vmla.f32 s28, s25, s26\n"   /* C_{t+1} */
        "vmul.f32 s29, s24, s5\n"    /* H_t */
        "vmla.f32 s29, s25, s27\n"   /* H_{t+1} */

        /* phi=(1-H)*C */
        "vsub.f32 s30, s21, s29\n"
        "vmul.f32 s0, s30, s28\n"

        /* F_{n+1} = F_n*sqrt(3)/2 - pi*sin(279deg), with sin(279deg) precomputed */
        "vmov.f32 s1, -0.98768836\n"
        "vmov.f32 s2, 3.1415927\n"
        "vmul.f32 s3, s2, s1\n"
        "vmla.f32 s3, s28, s7\n"

        /* Persist state image for xor accumulator */
        "vstmia r1!, {s0-s6}\n"
        "mov r2, #0\n"

        /* XOR 28 bytes (7 floats) unrolled branchlessly */
        ".rept 28\n"
        "ldrb r4, [r1], #1\n"
        "eor r2, r2, r4\n"
        ".endr\n"

        /* CRC-like fold */
        "ldr r1, =0x1EDC6F41\n"
        "umull r5, r6, r2, r1\n"
        "eor r2, r6, r5, lsr #1\n"

        /* Token gate: token=42 when phi>0, else 0, without control branch */
        "vcmp.f32 s0, #0.0\n"
        "vmrs APSR_nzcv, FPSCR\n"
        "mov r3, #42\n"
        "mov r4, #0\n"
        "movgt r4, r3\n"
        "str r4, [r7]\n"

        /* Idle forever */
        "wfi\n"
        "b .-4\n");
}
