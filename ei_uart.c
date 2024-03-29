#include "stdio.h"
#include "string.h"
#include "ei_uart.h"

#include "stdlib.h"
#include "stdint.h"
#include "uart_drv.h"
#include "ymodem.h"
#include "ble_api.h"
#include "ble_service_common.h"
#include "ei_spi_flash.h"
#include "efl_ble_iot_command.h"

uint8_t value=0;
ei_uart_t uart;

void ei_uart_cnt_rx_data(uint8_t * uart_rx_temp)
{
    if(uart.rx_len > 99){
        uart.rx_len=0;
        value=0;
    }
    
    uart.rx_data[uart.rx_len++]=uart_rx_temp[value];

    if((uart.flag_rx != RX_UNDER_PROGRESS) && (uart.rx_len >= 4) ){
         if((uart.rx_data[uart.rx_len-4]=='E') && (uart.rx_data[uart.rx_len-3]=='I') && (uart.rx_data[uart.rx_len-2]=='P') && (uart.rx_data[uart.rx_len-1]=='L')){
                uart.flag_rx=RX_UNDER_PROGRESS;
                memcpy(&uart.rx_data_to_process[0],&uart.rx_data[uart.rx_len-4],4);
                uart.rx_data_to_process_len=4;
            }
    }else if(uart.flag_rx == RX_UNDER_PROGRESS){
        if((uart.rx_data[uart.rx_len-2]==0x0D) && (uart.rx_data[uart.rx_len-1]==0x0A)){
            uart.flag_rx=RX_COMPLETED;
            memcpy(&uart.rx_data_to_process[0],&uart.rx_data[0],uart.rx_len);
            uart.flag_data_ready_to_process=1;
            uart.rx_len=0;
            efl_uart_cnt_command_process(uart.rx_data_to_process);
        }
    }
    if((uart.flag_rx != RX_UNDER_PROGRESS) && (uart.rx_len >= 10)){
        if( (uart.rx_data[uart.rx_len-4] == 0x43) && \
                (uart.rx_data[uart.rx_len-3] == 0x43) && \
                (uart.rx_data[uart.rx_len-2] == 0x43) && \
                (uart.rx_data[uart.rx_len-1] == 0x43)){
                printf("No app in CNT downloading the application\n");
                ymodem_send("CNTv1.bin");
                uart.rx_len=0;
            }
    }
}
