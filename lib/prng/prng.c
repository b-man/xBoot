/* Simple PRNG using SHA1 on a clock source.
 *
 * This implementation was lifted from:
 *
 * http://mista.nu/research/early_random-paper.pdf
 * http://mista.nu/research/early_random-slides.pdf
 */

#include <stdlib.h>
#include <string.h>

#include <prng.h>

#include <crypto/sha1.h>

#include <device/timer.h>

/* TODO: put this in sys/param.h */
#define MIN(a,b) (((a) < (b)) ? (a) : (b))

uint32_t prng_get_random_uint32(void)
{
    int i, j;
    uint32_t result = 0;

    for (i = 0; i < 32; i++) {
        j = i + 1;

        do {
            j--;
        } while (j);

        result = ((timer_read() & 1) | (result << 1));
    }

    return result;
}

void prng_get_random_bytes(char *buf, uint32_t len)
{
    char hash_bytes[SHA_DIGEST_LENGTH];
    uint32_t entropy_data[ENTROPY_SIZE];
    uint32_t copylen, copylen_remaining, hash_bytes_remaining;

    copylen_remaining = len;
    hash_bytes_remaining = 0;

    if (len) {
        do {
            if(!hash_bytes_remaining) {
                for (uint32_t i = 0; i < (ENTROPY_SIZE / sizeof(uint32_t)); i++) {
                    *(entropy_data + i) = prng_get_random_uint32();
                }

                SHA1(hash_bytes, (char *)entropy_data, ENTROPY_SIZE);
                hash_bytes_remaining = SHA_DIGEST_LENGTH;
            }
            copylen = MIN(hash_bytes_remaining, copylen_remaining);
            memcpy(buf, hash_bytes + SHA_DIGEST_LENGTH - hash_bytes_remaining, copylen);

            buf += copylen;
            copylen_remaining -= copylen;
            hash_bytes_remaining -= copylen;
        } while (copylen_remaining);

        bzero(entropy_data, ENTROPY_SIZE);
    }
}
