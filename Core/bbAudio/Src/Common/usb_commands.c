/*
 * usb_commands.c
 *
 *  Created on: Dec 14, 2020
 *      Author: fil
 */

#include "main.h"
#include <stdio.h>
#include <string.h>

uint8_t		USB_RxBuffer[64];
char		USB_TxBuffer[USB_TXBUF_SIZE];
uint32_t 	USB_RxBufferLen;
uint32_t	usb_packet_ready=0;	/* packet ready flag : defined here, set in usbd_cdc_if.c */
s_rxbuf		usb_rxbuf;

#define	MAX_TEXTPARAM_LEN 8
char 	cmd[MAX_TEXTPARAM_LEN], param[MAX_TEXTPARAM_LEN], c0[MAX_TEXTPARAM_LEN] ,c1[MAX_TEXTPARAM_LEN], c2[MAX_TEXTPARAM_LEN], c3[MAX_TEXTPARAM_LEN], c4[MAX_TEXTPARAM_LEN] ,c5[MAX_TEXTPARAM_LEN], c6[MAX_TEXTPARAM_LEN], c7[MAX_TEXTPARAM_LEN];
int		channel,p0,p1,p2,p3,p4,p5,p6,p7;
#define	FS(x) 240000000/x
void usb_transmit(char *buf)
{
	CDC_Transmit_FS((uint8_t *)buf, strlen(buf));
}

void five_bytes_commands(void)
{
/* cmd  channel  name	 other ...
 * set <channel> param   fs <sampling freq in hertz>
 * example:
 * <set 0 param fs 48000>
 * <storesettings>
 * <loadsettings>
 * <info>
 * <set 0 param fs 96000>
 * */
	if (( strcmp(cmd,"set") == 0 ) && ( strcmp(param,"param") == 0) && ( strcmp(c0,"fs") == 0))
	{
		System.sampling_frequency[channel] = p0;
		sprintf(USB_TxBuffer,"Sampling frequency set to %d : OK\n\r",p0);
		ChangeSampleFrequency(p0,channel);
	}
	else
		sprintf((char *)&USB_TxBuffer[0],"Error\n\r");
}

/* cmd  channel  name	 dir  other ...
 * use <channel> VCO     <wave> <pipe_out> control <control>
 * use <channel> VCA     in <pipe_in>   out <pipe_out> control <control>
 * use <channel> ECHO    in <pipe_in>   out <pipe_out>
 * use <channel> q15FIR  in <pipe_in>   out <pipe_out>
 * use <channel> f32FIR  in <pipe_in>   out <pipe_out>
 * use <channel> IIR     in <pipe_in>   out <pipe_out>
 * use <channel> MIXER   in1 <pipe_in>  in2 <pipe_in> out <pipe_out> control1 <control> control2 <control>
 * use <channel> RING    in1 <pipe_in>  in2 <pipe_in> out <pipe_out> control1 <control> control2 <control>
 * example:
 * pioj èojk <use 0 VCO SINE 30 control 0>
 * <use 0 VCO TRIANGLE 30 control 0>
 * <use 0 VCO TRIANGLE 30 control 0>
 * <use 0 VCO TRIANGLE 30 control 0>
 * <use 0 ECHO in 0 out 1>
 * <use 1 ECHO in 0 out 2>
 * <use 2 ECHO in 0 out 3>
 * <use 0 ECHO in 0 out 30>
 * <use 0 ECHO in 0 out 1>
 * <use 0 ECHO in 1 out 2>
 * <use 0 ECHO in 2 out 3>
 * <use 0 ECHO in 3 out 30>
 * <use 0 q15FIR  in 0   out 30>
 *
 */

void seven_bytes_commands(void)
{
uint16_t	wave;
	if (( strcmp(cmd,"use") == 0 ) && ( strcmp(param,"VCO") == 0) )
	{
		if ( strcmp(c0,"SINE") == 0 )
			wave = SINE;
		if ( strcmp(c0,"TRIANGLE") == 0 )
			wave = TRIANGLE;
		if ( strcmp(c0,"SQUARE") == 0 )
			wave = SQUARE;
		VCOInit     (stage, wave, (uint32_t )&audio_pipe[p0],(uint32_t )&control_buf.ain1,channel);
	}
	if (( strcmp(cmd,"use") == 0 ) && ( strcmp(param,"ECHO") == 0) )
	{
		ECHOInit    (stage, (uint32_t )&audio_pipe[p0], (uint32_t )&audio_pipe[p1],channel);
	}
	sprintf(USB_TxBuffer,"%s %d %s %s %d %s %d: OK\n\r",cmd, channel,param,c0,p0,c1,p1);
}
/* <info> */
void one_byte_commands(void)
{
char		buf[128];
uint32_t	entries,i;
uint32_t	channel;
	if ( strcmp(cmd,"clear") == 0 )
	{
		sprintf(USB_TxBuffer,"All %d channels cleared\n\r",(int )ClearFunnelEntries());
		return;
	}
	if ( strcmp(cmd,"storeprogram") == 0 )
	{
		StoreProgramInFlash();
		sprintf(USB_TxBuffer,"Program stored\n\r");
		return;
	}
	if ( strcmp(cmd,"storesettings") == 0 )
	{
		StoreSettingsInFlash();
		sprintf(USB_TxBuffer,"Settings stored\n\r");
		return;
	}
	if ( strcmp(cmd,"loadprogram") == 0 )
	{
		LoadProgramFromFlash();
		sprintf(USB_TxBuffer,"Program loaded\n\r");
		return;
	}
	if ( strcmp(cmd,"loadsettings") == 0 )
	{
		LoadSettingsFromFlash();
		sprintf(USB_TxBuffer,"Settings loaded\n\r");
		return;
	}
	if (( strcmp(cmd,"info") == 0 ) || ( strcmp(cmd,"?") == 0 ))
	{
		sprintf(USB_TxBuffer,"Version : %s\n\r",bbVERSION);
		sprintf(buf,"Audio samples : %d\n\r",NUMBER_OF_AUDIO_SAMPLES);
		if (( strlen(USB_TxBuffer) + strlen(buf)) < USB_TXBUF_SIZE )
			strcat(USB_TxBuffer,buf);
		sprintf(buf,"Sampling frequency ch0 : %d\n\r",(int )( System.sampling_frequency[0]));
		if (( strlen(USB_TxBuffer) + strlen(buf)) < USB_TXBUF_SIZE )
			strcat(USB_TxBuffer,buf);
		sprintf(buf,"Sampling frequency ch1 : %d\n\r",(int )( System.sampling_frequency[1]));
		if (( strlen(USB_TxBuffer) + strlen(buf)) < USB_TXBUF_SIZE )
			strcat(USB_TxBuffer,buf);
		sprintf(buf,"Number of Stages : %d\n\r",NUMSTAGES);
		if (( strlen(USB_TxBuffer) + strlen(buf)) < USB_TXBUF_SIZE )
			strcat(USB_TxBuffer,buf);
		channel = OUTCHANNEL_0;
		entries = ReportFunnelEntries(channel);
		if ( entries != 0 )
		{
			for(i=0;i<entries;i++)
			{
				sprintf(buf,"Channel = %d , Entry %d : %s\n\r",(int )channel,(int )i,ReportFunnelName(channel,i));
				if (( strlen(USB_TxBuffer) + strlen(buf)) < USB_TXBUF_SIZE )
					strcat(USB_TxBuffer,buf);
			}
		}
		else
		{
			sprintf(buf,"No Entries on Channel %d\n\r",(int )channel);
			if (( strlen(USB_TxBuffer) + strlen(buf)) < USB_TXBUF_SIZE )
				strcat(USB_TxBuffer,buf);
		}
		channel = OUTCHANNEL_1;
		entries = ReportFunnelEntries(channel);
		if ( entries != 0 )
		{
			for(i=0;i<entries;i++)
			{
				sprintf(buf,"Channel = %d , Entry %d : %s\n\r",(int )channel,(int )i,ReportFunnelName(channel,i));
				if (( strlen(USB_TxBuffer) + strlen(buf)) < USB_TXBUF_SIZE )
					strcat(USB_TxBuffer,buf);
			}
		}
		else
		{
			sprintf(buf,"No Entries on Channel %d\n\r",(int )channel);
			if (( strlen(USB_TxBuffer) + strlen(buf)) < USB_TXBUF_SIZE )
				strcat(USB_TxBuffer,buf);
		}
		return;
	}
	sprintf(USB_TxBuffer,"Command error :  %s is not a valid command\n\r",cmd);
}

static void parse_packet(void)
{
char 	*lbuf=(char *)&usb_rxbuf.packet[0];
int		pnum;

		pnum = sscanf(lbuf,"%s %d %s %s %d %s %d %s %d %s %d %s %d %s %d %s %d %s %d",cmd,&channel,param,c0,&p0,c1,&p1,c2,&p2,c3,&p3,c4,&p4,c5,&p5,c6,&p6,c7,&p7);
		switch (pnum)
		{
			case 1 :	one_byte_commands();
						break;
			case 5 :	five_bytes_commands();
						break;
			case 7 :	seven_bytes_commands();
						break;
			default:	sprintf(USB_TxBuffer,"Command error %d\n\r",pnum); break;
		}
		CDC_Transmit_FS((uint8_t* )USB_TxBuffer, strlen(USB_TxBuffer));
		bzero(usb_rxbuf.packet,USBUART_BUFLEN);
}

uint16_t	do_cp=0,pkt_found=0;

void CheckUSB(void)
{
uint16_t	i;
	if ( usb_packet_ready == 1 )
	{
		for(i=0;i<USB_RxBufferLen;i++)
		{
			if ( USB_RxBuffer[i] == '<' )
				do_cp=1;
			else
			{
				if ( do_cp == 1 )
				{
					if ( USB_RxBuffer[i] == '>' )
					{
						do_cp=0;
						pkt_found=1;
					}
					else
					{
						usb_rxbuf.packet[usb_rxbuf.usb_rx_index] = USB_RxBuffer[i];
						usb_rxbuf.usb_rx_index++;
						if ( usb_rxbuf.usb_rx_index >= USBUART_BUFLEN)
							usb_rxbuf.usb_rx_index = 0;
					}
				}
			}
		}
		if ( pkt_found==1 )
		{
			pkt_found=0;
			usb_rxbuf.usb_rx_index = 0;
			parse_packet();
		}
		usb_packet_ready = 0;
	}
}
