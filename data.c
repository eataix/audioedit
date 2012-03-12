/*-
 * Copyright (c) 2012, Meitian Huang
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution
 *
 * THIS SOFTWARE IS PROVIDED BY Meitian Huang AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AN ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#include "data.h"
#include "dbg.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

/*
 * Copies the content of the file that has the specified path.
 * Returns a pointer to the content.
 * Aborts the program upon failures.
 */
Data           *
CopyDataFromFileOrDie(const char *fin_path) {
    Data           *data;
    FILE           *fin;
    size_t          return_value;

    data = (Data *) malloc(sizeof(Data));
    check_mem(data);

    fin = fopen(fin_path, "rb");
    check(fin != NULL, "Cannot open the input file");

    /*
     * Get the total size of the file.
     */
    fseek(fin, 0, SEEK_END);
    data->size = ftell(fin);
    rewind(fin);

    data->content = (char *) malloc(data->size);
    check_mem(data->content);

    return_value = fread(data->content, 1, data->size, fin);
    check(return_value == data->size, "Cannot read the entire file");
    /*
     * Content is already copied to memory. No need to use the input file.
     */
    fclose(fin);

    return data;

  error:
    if (data->content) {
        free(data->content);
    }
    if (data) {
        free(data);
    }
    abort();
}

/*
 * A worker function of memcpy().
 */
void
SetData(Data * data, void *new_data, const char size,
        const uint64_t start_address) {
    check(size <= data->size, "Don't be silly");
    memcpy((data->content) + start_address, new_data, size);
    return;
  error:
    abort();
}


/*
 * Writes the data to disk.
 * Aborts upon failure.
 */
void
WriteDataOrDie(const Data * data, const char *fout_path,
               const uint64_t size) {
    FILE           *fout;
    size_t          return_value;

    fout = fopen(fout_path, "wb");
    check(fout != NULL, "Cannot open the output file.");

    return_value = fwrite(data->content, 1, size, fout);
    check(return_value == size, "Cannot write the file.");

    fclose(fout);
    return;

  error:
    abort();
}
