#MPU driver & Fault handler for Cortex-M4

##Requirements

* ChibiOS
* Cortex-M4 with Memory Protection Unit

##MPU Usage

By default the MPU prevents access to NULL pointer via memory region 7 (highest priority).
Code execution from RAM is also disabled using lowest priority (memory region).
To enable this you only have call `mpu_init()` to be protected.

You can then further restrict memory by using `mpu_configure_region`, e.g.:

```cpp
    /* Disable code execution from RAM. */
    mpu_configure_region(6, /* Region number. */
                         0x2000 0000, /* Base address */
                         20, /* 1MB (2^20) */
                         AP_RW_RW,
                         false); /* Non executable. */
```

If you want to enable code execution from RAM you should add an executable region with higher priority (1-6).

##Fault Handler Usage

* Implement `void fault_printf(const char *fmt, ...)` for logging.
* Call `fault_init()` to enable the fault IRQs.

The file `fault_tests.c` contains functions to test the MPU and the fault handler.

Note: Depending on the CPU (Cortex-M3/M4 or Cortex-M0) you need to compile fault_v7m.s or fault_v6m.s

##License
This code is released under a BSD license
