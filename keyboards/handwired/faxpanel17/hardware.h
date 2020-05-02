#pragma once

#define LEDOFF 1
#define LEDON 0

void setPortBMode(unsigned mode);
void write374(uint8_t data);
void write259(uint8_t data, uint8_t output);
void initFaxPanel(void);
