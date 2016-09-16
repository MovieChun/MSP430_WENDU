#ifndef _PC_h_
#define _PC_h_

#include <stdlib.h>
#include <stdio.h>

#include "Clock.h"
#include "flash.h"
#include "SCI.h"
#include "SPI.h"
#include "IIC.h"
#include "uart.h"
#include "uart2.h"


#include "mlx90614.h"
#include "WIFI_INIT.h"
#include "GU900_INIT.h"
#include "WIFI_RUN.h"
#include "GU900_RUN.h"
#include "PC.h"

#include "Fstring.h"


void READ(void);
void SAVE(void);

void PC_control(void);

#endif