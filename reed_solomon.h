/*
 * reed_solomon.h
 *
 *  Created on: Sep 13, 2019
 *      Author: emekcan
 */

#ifndef REED_SOLOMON_H_
#define REED_SOLOMON_H_

/*
 *
 * Copyright 2018 Jason M. Sachs
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdint.h>
#include <stddef.h>
#include <string.h>

/**
 * Reed-Solomon log-table-based implementation
 */
typedef struct {
    uint8_t log[256];
    uint8_t antilog[256];
    uint8_t symbol_polynomial;
    const uint8_t *gpowers;
    uint8_t generator_degree;
} rslogtable_t;


/**
 * Initialize log table
 *
 * @param ptables - points to log tables
 * @param symbol_polynomial: lower 8 bits of symbol polynomial
 *         (for example, 0x1d for 0x11d = x^8 + x^4 + x^3 + x^2 + 1
 * @param gpowers - points to generator polynomial coefficients,
 *         except for the leading monomial,
 *         in descending order, expressed as powers of x in the symbol field.
 *         This requires all generator polynomial coefficients to be nonzero.
 * @param generator_degree - degree of the generator polynomial,
 *         which is also the length of the gpowers array.
 */
void rslogtable_init(
    rslogtable_t *ptables,
    uint8_t symbol_polynomial,
    const uint8_t *gpowers,
    uint8_t generator_degree
)
{
    ptables->symbol_polynomial = symbol_polynomial;
    ptables->gpowers = gpowers;
    ptables->generator_degree = generator_degree;

    int k;
    uint8_t c = 1;
    for (k = 0; k < 255; ++k)
    {
        ptables->antilog[k] = c;
        ptables->log[c] = k;
        if (c >> 7)
        {
            c <<= 1;
            c ^= symbol_polynomial;
        }
        else
        {
            c <<= 1;
        }
    }
    ptables->log[0] = 0;
    ptables->antilog[255] = 0;
}

/**
 * Encode a message
 *
 * @param ptables - pointer to table-based lookup info
 * @param pmessage - message to be encoded
 * @param message_length - length of the message
 * @param premainder - buffer which will be filled in with the remainder.
 *     This must have a size equal to ptables->generator_degree
 */
void rslogtable_encode(
    const rslogtable_t *ptables,
    const uint8_t *pmessage,
    size_t message_length,
    uint8_t *premainder
)
{
    int i, j, k, n;
    int highbyte;
    const int t2 = ptables->generator_degree;
    const int lastindex = t2-1;

    // Zero out the remainder
    for (i = 0; i < t2; ++i)
    {
        premainder[i] = 0;
    }

    n = message_length + t2;
    for (i = 0; i < n; ++i)
    {
        uint8_t c = (i < message_length) ? pmessage[i] : 0;
        highbyte = premainder[0];
        if (highbyte == 0)
        {
            // No multiplication, just a simple shift
            for (k = 0; k < lastindex; ++k)
            {
                premainder[k] = premainder[k+1];
            }
            premainder[lastindex] = c;
        }
        else
        {
            uint8_t log_highbyte = ptables->log[highbyte];
            for (k = 0; k < lastindex; ++k)
            {
                j = ptables->gpowers[k] + (int)log_highbyte;
                if (j >= 255)
                    j -= 255;
                premainder[k] = premainder[k+1] ^ ptables->antilog[j];
            }
            j = ptables->gpowers[lastindex] + (int)log_highbyte;
            if (j >= 255)
                j -= 255;
            premainder[lastindex] = c ^ ptables->antilog[j];
        }
    }
}

uint8_t rsremainder[16];
void rslogtable_test(void)
{
    const uint8_t gpowers[] = {
        120, 104, 107, 109,
        102, 161, 76, 3,
        91, 191, 147, 169,
        182, 194, 225, 120
    };
    rslogtable_t rsinfo;
    rslogtable_init(&rsinfo, 0x1d, gpowers, 16);

    const char message[] = "Ernie, you have a banana in your ear!";
    rslogtable_encode(&rsinfo, (const uint8_t *)message, strlen(message), rsremainder);
}

#endif /* REED_SOLOMON_H_ */
