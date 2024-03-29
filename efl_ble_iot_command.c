#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "stdint.h"
#include "ei_uart.h"
#include "ei_main.h"
#include "uart_drv.h"
#include "ble_api.h"
#include "ble_service_common.h"
#include "ei_spi_flash.h"
#include "ymodem.h"
#include "ble_app.h"
#include "efl_ble_iot_command.h"

st_IOT_PARAM iot_param;

uint8_t cnt_hw_ver_data[10];
uint8_t cnt_sw_ver_data[10];

uint8_t prepare_ble_iot_data_packet_(st_IOT_PARAM *param, uint8_t queue_id)
{
    uint8_t ble_proto_data[100];
    uint8_t packetid=0x01;
    memset(&ble_proto_data[0],0,100);
    memcpy(&ble_proto_data[0],VEN,sizeof(VEN));
    ble_proto_data[4]=PACKET_PAYLOAD_LEN;
    ble_proto_data[5]=BLE_TX_CMD_ID;
    ble_proto_data[6]=queue_id;
	ble_proto_data[24]=0x0D;
	ble_proto_data[25]=0x0A;

    switch(packetid)
    {
        case 1:    
            printf("Packet1_Sent\n");
	        ble_proto_data[7] = packetid++;
            ble_proto_data[8]= iot_param.model.na; ble_proto_data[9]= iot_param.model.ty;
            ble_proto_data[10]= iot_param.model.va; ble_proto_data[11]= iot_param.model.pm;
            ble_proto_data[12]= iot_param.product_slno;
            trsp_data_send_from_isr(ble_proto_data,PACKET_COMMAND_LEN);
            
        case 2:
            printf("Packet2_Sent\n");
            ble_proto_data[7] = packetid++;
            ble_proto_data[8]= iot_param.rltrs; ble_proto_data[12]= iot_param.flow;
            ble_proto_data[16]= iot_param.pcs; ble_proto_data[20]= iot_param.dlyltrs;
            trsp_data_send_from_isr(ble_proto_data,PACKET_COMMAND_LEN);

        case 3:
            printf("Packet3_Sent\n");
            ble_proto_data[7] = packetid++;
            ble_proto_data[8]= iot_param.filter.life.sed; ble_proto_data[12]= iot_param.filter.life.pre;
            ble_proto_data[16]= iot_param.filter.life.ro; ble_proto_data[20]= iot_param.filter.life.post;
            trsp_data_send_from_isr(ble_proto_data,PACKET_COMMAND_LEN);

        case 4:
            printf("Packet4_Sent\n");
            ble_proto_data[7] = packetid++;
            ble_proto_data[8]= iot_param.tds.in; ble_proto_data[12]= iot_param.tds.out;
            ble_proto_data[16]= iot_param.cltrs.in; ble_proto_data[20]= iot_param.cltrs.out;
            trsp_data_send_from_isr(ble_proto_data,PACKET_COMMAND_LEN);

        case 5:
            printf("Packet5_Sent\n");
            ble_proto_data[7] = packetid++;
            ble_proto_data[8]= iot_param.function.hot;  ble_proto_data[9]= iot_param.function.buzzer;
            ble_proto_data[10]=  iot_param.function.clean; ble_proto_data[11]= iot_param.status.nm; 
            ble_proto_data[12]= iot_param.status.pu;  ble_proto_data[13]= iot_param.status.atds; 
            ble_proto_data[14]= iot_param.status.tl;  ble_proto_data[15]= iot_param.status.iw;
            ble_proto_data[16]=iot_param.status.wd; ble_proto_data[17]= iot_param.status.hwd; 
            ble_proto_data[18]= iot_param.status.af; ble_proto_data[19]= iot_param.qtest; 
            trsp_data_send_from_isr(ble_proto_data,PACKET_COMMAND_LEN);

        case 6:
            printf("Packet6_Sent\n");
            ble_proto_data[7] = packetid++;
            ble_proto_data[8]= iot_param.error_codes;
            ble_proto_data[12]= IOT_HW_VERSION_MAJOR; ble_proto_data[13]= IOT_HW_VERSION_MINOR;
            ble_proto_data[14]= IOT_SW_VERSION_MAJOR; ble_proto_data[15]= IOT_SW_VERSION_MINOR;
            ble_proto_data[16]= cnt_hw_ver_data[0]; ble_proto_data[17]= cnt_hw_ver_data[1]; 
            ble_proto_data[18]= cnt_sw_ver_data[0]; ble_proto_data[19]= cnt_sw_ver_data[1]; 
            ble_proto_data[20]= DISP_HW_VERSION_MAJOR; ble_proto_data[21]= DISP_HW_VERSION_MINOR;
            ble_proto_data[22]= DISP_SW_VERSION_MAJOR; ble_proto_data[23]= DISP_SW_VERSION_MINOR;
            trsp_data_send_from_isr(ble_proto_data,PACKET_COMMAND_LEN);

        case 7:
            printf("Packet7_Sent\n");
            ble_proto_data[7] = packetid++;
            ble_proto_data[8]= BLE_IOT_CONNECTION_TECH; ble_proto_data[9]= iot_param.vendor;
            trsp_data_send_from_isr(ble_proto_data,PACKET_COMMAND_LEN);
        
        default:
            break;
    }

}

void efl_uart_cnt_command_process(uint8_t * data)
{
    uint8_t payload_len=data[4];
	uint8_t queue_id=data[6];
	uint8_t commandid=data[7]; 
    uint32_t temp;  
    uint8_t iot_proto_data[100];

    memset(&iot_proto_data[0],0,100);
    memcpy(&iot_proto_data[0],VEN,sizeof(VEN));
    iot_proto_data[4]=payload_len;
    iot_proto_data[5]=BLE_TX_CMD_ID;
    iot_proto_data[6]=queue_id;
	iot_proto_data[7]=commandid;
	iot_proto_data[8]=SUCCESS;
    iot_proto_data[12]=0x0D;
	iot_proto_data[13]=0x0A;

    uart_tx(0, iot_proto_data, payload_len+5);

	switch(commandid)
	{
        case CMD_IOT_MODEL_PARAM:
            printf("cmd CMD_IOT_MODEL_PARAM received\r\n");
            iot_param.model.na = (data[6] << 8) | data[7];
            iot_param.model.ty = data[8];
            iot_param.model.va = (data[9] << 8) | data[10];
            iot_param.model.pm = data[11];

        break;
        case CMD_IOT_RLTRS_FLOW_PCS:
            printf("cmd CMD_IOT_RLTRS_FLOW_PCS received\r\n");
            iot_param.rltrs = (data[6] << 8) | data[7];
            iot_param.flow = (data[8] << 24) | (data[9] << 16) | (data[10] << 8) | (data[11]);
            iot_param.pcs = (data[12] << 8) | data[13];
        
        break;
        case CMD_IOT_FILTER:
            printf("cmd CMD_IOT_FILTER received\r\n");
            iot_param.filter.life.sed= (data[6] << 24) | (data[7] << 16) | (data[8] << 8) | (data[9]);
            iot_param.filter.life.pre= (data[10] << 24) | (data[11] << 16) | (data[12] << 8) | (data[13]);
            iot_param.filter.life.ro= (data[14] << 24) | (data[15] << 16) | (data[16] << 8) | (data[17]);
            iot_param.filter.life.post= (data[18] << 24) | (data[19] << 16) | (data[20] << 8) | (data[21]);
        
        break;
        case CMD_IOT_TDS:
            printf("cmd CMD_IOT_TDS received\r\n");
            iot_param.tds.in= (data[6] << 8) | data[7];
            iot_param.tds.out= (data[8] << 8) | data[9];
        
        break;
        case CMD_IOT_CLTRS:
            printf( "cmd CMD_IOT_CLTRS received\r\n");
            iot_param.cltrs.in= (data[6] << 24) | (data[7] << 16) | (data[8] << 8) | (data[9]);
            iot_param.cltrs.out= (data[10] << 24) | (data[11] << 16) | (data[12] << 8) | (data[13]);
        
        break;
        case CMD_IOT_DLYLTRS:
            printf( "cmd CMD_IOT_DLYLTRS received\r\n");
            iot_param.dlyltrs= (data[6] << 24) | (data[7] << 16) | (data[8] << 8) | (data[9]);
        
        break;
        case CMD_IOT_FOURTEEN_DAY_DATA:
            printf( "cmd CMD_IOT_FOURTEEN_DAY_DATA received\r\n");
            iot_param.epoch_stamp= (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | (data[3]);
            iot_param.tds.in= (data[4] << 8) | data[5];
            iot_param.tds.out= (data[6] << 8) | data[7];
            iot_param.cltrs.out= (data[8] << 24) | (data[9] << 16) | (data[10] << 8) | (data[11]);
            // prepare_fourteen_day_packet_json_array(&iot_param);

        break;
        case CMD_IOT_STATUS_FUNCTION:
            printf( "cmd CMD_IOT_STATUS_FUNCTION received\r\n");
            temp = (data[6] << 24) | (data[7] << 16) | (data[8] << 8) | (data[9]);

            iot_param.function.hot =  (temp >> BIT_POS_FUNCTION_HOT) & 0x1;
            iot_param.function.buzzer =  (temp >> BIT_POS_FUNCTION_BUZZER) & 0x1;
            iot_param.function.clean =  (temp >> BIT_POS_FUNCTION_CLEAN) & 0x1;

            iot_param.status.nm =  (temp >> BIT_POS_STATUS_NM) & 0x1;
            iot_param.status.pu =  (temp >> BIT_POS_STATUS_PU) & 0x1;
            iot_param.status.atds =  (temp >> BIT_POS_STATUS_ATDS) & 0x1;
            iot_param.status.tl =  (temp >> BIT_POS_STATUS_TL) & 0x1;
            iot_param.status.iw =  (temp >> BIT_POS_STATUS_IW) & 0x1;
            iot_param.status.wd =  (temp >> BIT_POS_STATUS_WD) & 0x1;
            iot_param.status.hwd =  (temp >> BIT_POS_STATUS_HWD) & 0x1;
            iot_param.status.af =  (temp >> BIT_POS_STATUS_AF) & 0x1;
            iot_param.qtest =  (temp >> BIT_POS_QTEST) & 0x1;
        
        break;
        case CMD_IOT_VERSION:
            printf( "cmd CMD_IOT_VERSION received\r\n");
            memcpy(&cnt_hw_ver_data[0],&data[6],2);
            ei_print_hex_array("HW",cnt_hw_ver_data,2);
            memcpy(&cnt_sw_ver_data[0],&data[8],2);
            ei_print_hex_array("SW",cnt_sw_ver_data,2);
        
        break;
        case CMD_IOT_ERROR_CODE:
            printf( "cmd CMD_IOT_ERROR_CODE received\r\n");
            iot_param.error_codes = (data[6] << 24) | (data[7] << 16) | (data[8] << 8) | (data[9]);
        
        break;    
        case CMD_IOT_POST_DATA_TO_SERVER:
            printf("cmd CMD_IOT_POST_DATA_TO_SERVER received\r\n");
            prepare_ble_iot_data_packet_(&iot_param, queue_id);
        break;
        case CMD_GET_READY_FOR_OTA:
            printf("cmd CMD_GET_READY_FOR_OTA received\r\n");
            if(data[6]==0x01){
                printf("CNT board ready to download OTA file");
                ymodem_send("CNTv3.bin");
                printf("=====OTA file sent successfully=====");    
            }else if(data[6]==0x00){
                printf("OTA file not sent CNT board not ready");
            }

        break;
        case CMD_WP_PRODUCT_SLNO: 
            printf("cmd CMD_WP_PRODUCT_SLNO received\r\n");
            iot_param.product_slno = (data[6] << 24) | (data[7] << 16) | (data[8] << 8) | (data[9]);
        break;
        case CMD_VENDOR_ID:
            printf("cmd CMD_VENDOR_ID received\r\n");
            iot_param.vendor = data[6];
        break;

        default: 
            printf( "cmd NO PROPER CMD received\r\n");
        break;
    }
}

void ei_print_hex_array(const char * input, uint8_t *data, uint16_t data_len)
{
	uint16_t buf_pos=0,i;
	char str_data[100];

	if(data_len>248) data_len=248;

	for(i=0;i<(data_len);i++)
	{
		buf_pos += sprintf((char *)(str_data + buf_pos),"%02X",data[i]); //,data[i+1]);

	}
	if(i<data_len) str_data[i]=0; //just stting null pointer
	printf("%s=%s",input,str_data);
}