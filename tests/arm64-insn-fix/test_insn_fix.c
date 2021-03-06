#include "hookzz.h"
#include <stdio.h>
#include <unistd.h>

static void arm64_insn_need_fix() {
    __asm__ volatile("bl #40\n"
                     "nop");
}

#include "platforms/backend-arm64/interceptor-arm64.h"
#include <stdlib.h>

#if 1
__attribute__((constructor)) void test_insn_fix_arm64() {

    ZzInterceptorBackend *backend  = (ZzInterceptorBackend *)malloc(sizeof(ZzInterceptorBackend));
    char temp_code_slice_data[256] = {0};

    zz_arm64_writer_init(&backend->arm64_writer, NULL);
    zz_arm64_relocator_init(&backend->arm64_relocator, NULL, &backend->arm64_writer);
    zz_arm64_writer_init(&backend->arm64_writer, NULL);
    zz_arm64_relocator_init(&backend->arm64_relocator, NULL, &backend->arm64_writer);

    ZzARM64Relocator *arm64_relocator;
    ZzARM64Writer *arm64_writer;
    arm64_relocator = &backend->arm64_relocator;
    arm64_writer    = &backend->arm64_writer;

    zz_arm64_writer_reset(arm64_writer, temp_code_slice_data);

    zz_arm64_relocator_reset(arm64_relocator, ((zz_addr_t)arm64_insn_need_fix & ~(zz_addr_t)1), arm64_writer);
    zz_size_t tmp_relocator_insn_size = 0;

    do {
        zz_arm64_relocator_read_one(arm64_relocator, NULL);
        zz_arm64_relocator_write_one(arm64_relocator);
        tmp_relocator_insn_size = arm64_relocator->input_cur - arm64_relocator->input_start;
    } while (tmp_relocator_insn_size < 36);
}
#endif
