#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "stdint.h"

#include <stdbool.h>
#include "FreeRTOS.h"
#include "task.h"
#include "task_hci.h"
#include "lpm.h"
#include "ble_event.h"
#include "ble_printf.h"
#include "ble_fota.h"
#include "ei_uart.h"

#include "uart_drv.h"
#include "ymodem.h"
#include "ei_main.h"
#include "ble_api.h"
#include "ble_app.h"
#include "ble_service_common.h"
#include "ei_spi_flash.h"
#include "efl_ble_iot_command.h"
#include "ei_ble_process.h"
#include "ble_service_trsps.h"
#include "ble_profile.h"


uint8_t iot_proto_data[100];
uint8_t header_proto_data[15];
uint8_t ble_data_process[10];
extern uint8_t host_id;
extern uint16_t data_num;
ble_gatt_data_param_t efl_ble_data_param;
TimerHandle_t  g_fota_timer;

/*void efl_ble_header_packet(uint8_t payload_len, uint8_t queue_id, uint8_t commandid)
{
    memset(&header_proto_data[0],0,15);
    memcpy(&header_proto_data[0],SOF,sizeof(SOF));
    header_proto_data[4]=payload_len;
    header_proto_data[5]=BLE_TX_CMD_ID;
    header_proto_data[6]=queue_id;
    header_proto_data[7]=commandid;
    header_proto_data[8]=BYTES_PER_PACKET;
    header_proto_data[9]=NO_OF_PACKET;
    header_proto_data[10]=BLE_FW_VERSION_MAJOR;
    header_proto_data[11]=BLE_FW_VERSION_MINOR;
    header_proto_data[12]=0x0D;
    header_proto_data[13]=0x0A;
    trsp_data_send_from_isr(header_proto_data,COMMAND_LEN);
    printf("cmd header packet sent\r\n");
}*/

void kishore_dec1(uint8_t payload_len, uint8_t queue_id, uint8_t commandid)
{
    memset(&header_proto_data[0],0,15);
    memcpy(&header_proto_data[0],VEN,sizeof(VEN));
    header_proto_data[4]=payload_len;
    header_proto_data[5]=BLE_TX_CMD_ID;
    header_proto_data[6]=queue_id;
    header_proto_data[7]=commandid;
    header_proto_data[8]=BYTES_PER_PACKET;
    header_proto_data[9]=NO_OF_PACKET;
    header_proto_data[10]=BLE_FW_VERSION_MAJOR;
    header_proto_data[11]=BLE_FW_VERSION_MINOR;
    header_proto_data[12]=0x0D;
    header_proto_data[13]=0x0A;
    trsp_data_send_from_isr(header_proto_data,COMMAND_LEN);
    //printf("cmd header packet sent\r\n");
}

/*void efl_ble_data_packet(uint8_t queue_id)
{
    uint8_t packet_id=0x01;
    memset(&iot_proto_data[0],0,100);
    memcpy(&iot_proto_data[0],SOF,sizeof(SOF));
    iot_proto_data[4]=PACKET_PAYLOAD_LEN;
    iot_proto_data[5]=BLE_TX_CMD_ID;
    iot_proto_data[6]=queue_id;
    iot_proto_data[24]=0x0D;
    iot_proto_data[25]=0x0A;
    for(packet_id=1;packet_id<=7;packet_id++){
       iot_proto_data[7]=packet_id;
        for(int i=0xA0, j=8;j<24;j++, i++){
            iot_proto_data[j]=i;
        }
        trsp_data_send_from_isr(iot_proto_data,PACKET_COMMAND_LEN);
        printf( "cmd PACKET [0x%x]sent\r\n",packet_id);
    }
}*/

void kishore_dec2(uint8_t queue_id)
{
    //uint8_t packet_id=0x01;
    memset(&iot_proto_data[0],0,100);
    memcpy(&iot_proto_data[0],VEN,sizeof(VEN));
    iot_proto_data[4]=PACKET_PAYLOAD_LEN;
    iot_proto_data[5]=BLE_TX_CMD_ID;
    iot_proto_data[6]=queue_id;
    iot_proto_data[24]=0x0D;
    iot_proto_data[25]=0x0A;
    /*for(packet_id=1;packet_id<=7;packet_id++){
       iot_proto_data[7]=packet_id;
        for(int i=0xA0, j=8;j<24;j++, i++){
            iot_proto_data[j]=i;
        }
        trsp_data_send_from_isr(iot_proto_data,PACKET_COMMAND_LEN);
        //printf( "cmd PACKET [0x%x]sent\r\n",packet_id);
    }*/
}




void efl_ble_read_response(uint8_t data[])
{
    ble_err_t status;
    efl_ble_data_param.host_id = 0; //host_id;
    efl_ble_data_param.handle_num = data_num;
    efl_ble_data_param.length = sizeof(data);
    efl_ble_data_param.p_data = (uint8_t *)data;
    printf("ble %s\n", data);
    status = ble_svcs_data_send(TYPE_BLE_GATT_READ_RSP, &efl_ble_data_param);
    if (status != BLE_ERR_OK)
    {
        printf("ble_gatt_read_rsp status: %d\n", status);
    }                                         
}

/*void ei_ble_rx_data(uint8_t *ble_rx_temp)
{
    memcpy(&ble_data_process[0],&ble_rx_temp[0],10);
    if((ble_data_process[0]==0x45) && (ble_data_process[1]==0x49) && (ble_data_process[2]==0x50) && (ble_data_process[3]==0x4C)) {
        efl_ble_data_command_process(ble_data_process);
    }
}*/

void kishore_dec3(uint8_t *ble_rx_temp)
{
    memcpy(&ble_data_process[0],&ble_rx_temp[0],10);
     if((ble_data_process[0]==0x4B) && (ble_data_process[1]==0x49) && (ble_data_process[2]==0x53) && (ble_data_process[3]==0x48) && (ble_data_process[4]==0x4F) && (ble_data_process[5]==0x52) && (ble_data_process[5]==0x45)){
        kishore_declaration(ble_data_process);
     }
}

/*void efl_ble_data_command_process(uint8_t * data)
{
    uint8_t payload_len=data[4];
	uint8_t queue_id=data[6];
	uint8_t commandid=data[7];
    
    memset(&iot_proto_data[0],0,100);
    memcpy(&iot_proto_data[0],SOF,sizeof(SOF));
    iot_proto_data[4]=payload_len;
    iot_proto_data[5]=BLE_TX_CMD_ID;
    iot_proto_data[6]=queue_id;
	iot_proto_data[7]=commandid;
	iot_proto_data[8]=SUCCESS;
    iot_proto_data[12]=0x0D;
	iot_proto_data[13]=0x0A;
    trsp_data_send_from_isr(iot_proto_data,COMMAND_LEN);
    printf("cmd acknowledgement sent\r\n");

	switch(commandid)
	{
        case CMD_BLE_REQUEST_PACKET:
            printf("cmd CMD_REQUEST_PACKET received\r\n");
            efl_ble_header_packet(payload_len, queue_id, commandid);
            efl_ble_data_packet(queue_id);
        break;
        case CMD_BLE_BLE_BOARD_OTA:
            printf( "cmd CMD_BLE_BOARD_OTA received\r\n");
            g_fota_timer = xTimerCreate("FOTA_Timer", pdMS_TO_TICKS(1000), pdTRUE, ( void * ) 0, fota_timer_handler);

          
        break;
        case CMD_BLE_CNT_BOARD_OTA:
            printf( "cmd CMD_CNT_BOARD_OTA received\r\n");
            // ei_spi_flash_test();
        break;
        default: 
            printf( "cmd NO PROPER CMD received\r\n");
        break;
    }
}*/

void kishore_declaration(uint8_t * data)
{
    uint8_t payload_len=data[4];
	uint8_t queue_id=data[6];
	uint8_t commandid=data[7];
    
    memset(&iot_proto_data[0],0,100);
    memcpy(&iot_proto_data[0],VEN,sizeof(VEN));
    iot_proto_data[4]=payload_len;
    iot_proto_data[5]=BLE_TX_CMD_ID;
    iot_proto_data[6]=queue_id;
	iot_proto_data[7]=commandid;
	iot_proto_data[8]=SUCCESS;
    iot_proto_data[12]=0x0D;
	iot_proto_data[13]=0x0A;
    trsp_data_send_from_isr(iot_proto_data,COMMAND_LEN);
    printf("Kishore commands is received\r\n");

	switch(commandid)
	{
        case CMD_BLE_REQUEST_PACKET:
            //printf("cmd CMD_REQUEST_PACKET received\r\n");
            kishore_dec1(payload_len, queue_id, commandid);
            kishore_dec2(queue_id);
        break;
        default:
            //printf("No output\r\n");
        break;
    }
}