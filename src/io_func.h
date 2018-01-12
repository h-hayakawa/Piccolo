#ifndef IO_FUNC_H
#define IO_FUNC_H

#include<stdint.h>
#include<stdio.h>

int32_t read_command(uint8_t *buf, int32_t buf_size, uint8_t *arg[], int32_t max_n_arg);

#endif
