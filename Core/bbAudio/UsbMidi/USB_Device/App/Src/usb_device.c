#include <usbd_midi_if.h>
//#include <usbd_cdc_if.h>
#include "usb_device.h"
#include "usbd_core.h"

#include "usbd_midi.h"
#include "usbd_desc.h"

extern void Error_Handler(void);
USBD_HandleTypeDef hUsbDeviceFS;
extern USBD_DescriptorsTypeDef MIDI_Desc;
extern USBD_DescriptorsTypeDef CDC_Desc;

/**
  * Init USB device Library, add supported class and start the library
  * @retval None
  */
void MX_USB_Device_Init(uint8_t midi_cdc)
{

		/* Init Device Library, add supported class and start the library. */
		if (USBD_Init(&hUsbDeviceFS, &MIDI_Desc, DEVICE_FS) != USBD_OK)
		{
			Error_Handler();
		}
		if (USBD_RegisterClass(&hUsbDeviceFS, &USBD_MIDI) != USBD_OK)
		{
			Error_Handler();
		}
		if (USBD_MIDI_RegisterInterface(&hUsbDeviceFS, &USBD_MIDI_Interface_fops_FS) != USBD_OK)
		{
			Error_Handler();
		}
		if (USBD_Start(&hUsbDeviceFS) != USBD_OK)
		{
			Error_Handler();
		}
}

void MX_USB_DEVICE_Init(void)
{
	MX_USB_Device_Init(0);
}

