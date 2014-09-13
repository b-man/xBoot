/*
 * Minimal sbrk() emulation required for malloc support.
 */

#include <panic.h>
#include <stdlib.h>
#include <sys/types.h>

caddr_t sbrk(int incr)
{
    /* defined in xboot.ld */
    extern uint32_t __heap_start;
    extern uint32_t __heap_end;

    static uint32_t *heap_end = 0;
    uint32_t *prev_heap_end;

    if (heap_end == 0) {
        heap_end = &__heap_start;

        if (heap_end > &__heap_end)
            panic("sbrk: illegal values for heap start and heap end!!\n");
    }

    prev_heap_end = heap_end;

    if ((heap_end + incr) > &__heap_end)
        panic("sbrk: heap memory not available\n");

    heap_end += incr;

    return (caddr_t)prev_heap_end;
}
