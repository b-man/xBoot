/*
 * Copyright 2013, winocm. <winocm@icloud.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *   Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 *   Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 *   If you are going to use this software in any form that does not involve
 *   releasing the source to this project or improving it, let me know beforehand.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdbool.h>

#include <assert.h>

#include <sys/align.h>
#include <sys/types.h>

#include <boot/image3.h>

#define image3_off(head, off) ((uint32_t)((uintptr_t)off - (uintptr_t)head))

extern uint8_t _binary_dtre_img3_start[];

int image3_fast_get_type(void *buffer, uint32_t *type)
{
    Image3RootHeader *head = (Image3RootHeader *) buffer;
    if (head->header.magic != kImage3Magic) {
        printf("image3_fast_get_type: invalid header magic 0x%08x\n",
               (unsigned int)head->header.magic);
        return false;
    }

    *type = head->shshExtension.imageType;

    return true;
}

Image3Header *image3_find_tag(void *image, uint32_t find_tag)
{
    Image3Header *root = (Image3Header *) image;
    Image3Header *tag = (Image3Header *) ((Image3RootHeader *) image + 1);

    while (1) {
        if (tag->magic == find_tag)
            return tag;
        tag = (Image3Header *) add_ptr2(tag, tag->size);
        if (image3_off(root, tag) > root->size)
            break;
    }

    return NULL;
}

int image3_get_tag_data(void *image, uint32_t tag, void **out_data,
                        uint32_t *out_size)
{
    Image3Header *imageTag = image3_find_tag(image, tag);

    if (imageTag) {
        assert(out_data != NULL);

        *out_size = imageTag->dataSize;
        *out_data = (void *)(imageTag + 1);

        return true;
    }

    return false;
}

void *image3_get_image(uint32_t type)
{
    Image3RootHeader *img3 = (Image3RootHeader *)&_binary_dtre_img3_start;

    while (img3 != NULL) {
        if (img3->header.magic != kImage3Magic) {
            printf("IMG3 Error: Invalid header magic!\n");
            return NULL;
        }
        if (img3->shshExtension.imageType == type)
            return img3;
        img3 = (Image3RootHeader *) ((uint8_t *)(img3) + img3->header.size);
    }

    return img3;
}
