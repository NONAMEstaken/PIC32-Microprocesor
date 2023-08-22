#include "TFT.h"
#include "SPI.h"
#include "Timer.h"
#include "UART.h"

void TFT_SendCmd(uchar regval){
    DC_ON;
    SPI1_Send_u8(regval);
}

void TFT_SendData8(uchar data){
    DC_OFF;
    SPI1_Send_u8(data);
}


void TFT_SendData16(uint data){
    DC_OFF;
    SPI1_Send_u16(data);
}

void TFT_WriteReg(uint TFT_Reg, uint TFT_RegValue){
    TFT_SendCmd(TFT_Reg);
    TFT_SendData16(TFT_RegValue);
}

void TFT_SOFT_RESET(){
    DelayMsec(100);
    TFT_SendCmd(ILI9341_CMD_SOFTWARE_RESET);
    DelayMsec(200);
}

void TFT_DisplayOn(){
    TFT_SendCmd(ILI9341_CMD_DISPLAY_ON);
}

void TFT_DisplayOff(){
    TFT_SendCmd(ILI9341_CMD_DISPLAY_OFF);
}

void TFT_SetCursor(uint x, uint y){
    TFT_SendCmd(ILI9341_CMD_COLUMN_ADDRESS_SET);
    TFT_SendData16(x);
    TFT_SendData16(x);
    TFT_SendCmd(ILI9341_CMD_PAGE_ADDRESS_SET);
    TFT_SendData16(y);
    TFT_SendData16(y);
}

void TFT_SetWindow(uint xStar, uint yStar, uint width, uint height){
    width = xStar+width-1;
    height = yStar+height-1;

    TFT_SendCmd(ILI9341_CMD_COLUMN_ADDRESS_SET);
    TFT_SendData16(xStar);
    TFT_SendData16(width);
    TFT_SendCmd(ILI9341_CMD_PAGE_ADDRESS_SET);
    TFT_SendData16(yStar);
    TFT_SendData16(height);
}

void TFT_WriteRAM_Prepare(){
    TFT_SendCmd(ILI9341_CMD_MEMORY_WRITE);
}

void TFT_Clear(uint color){
    int index=0;
	int totalpoint;
	
	TFT_SetWindow(0,0,TFT_WIDTH,TFT_HEIGHT);
	
	totalpoint=TFT_WIDTH * TFT_HEIGHT; 			
 
	TFT_SetCursor(0x00,0x00);	
	
	TFT_WriteRAM_Prepare();     		
	
	for(index=0;index<totalpoint;index++)
	{
		TFT_SendData16(color);
        //U2_Print("write ");
        //U2_Print_uchar(index/100);
        //U2_Print_uchar(index-100*(index/100));
        //U2_Print("\r\n");
	}
}

void TFT_Init(){

    U2_Print("Soft Reset\r\n");
    TFT_SOFT_RESET();

    U2_Print("set power control B\r\n");
    TFT_SendCmd(0xCF);  
    TFT_SendData8(0x00);
    TFT_SendData8(0xD9); 
    TFT_SendData8(0x30);

    TFT_SendCmd(0xED);  
    TFT_SendData8(0x64); 
    TFT_SendData8(0x03); 
    TFT_SendData8(0x12); 
    TFT_SendData8(0x81);

    TFT_SendCmd(0xE8);  
    TFT_SendData8(0x85); 
    TFT_SendData8(0x10); 
    TFT_SendData8(0x78);

    U2_Print("set power control A\r\n");
    TFT_SendCmd(0xCB);  
    TFT_SendData8(0x39); 
    TFT_SendData8(0x2C); 
    TFT_SendData8(0x00); 
    TFT_SendData8(0x34); 
    TFT_SendData8(0x02);

    TFT_SendCmd(0xF7);  
    TFT_SendData8(0x20);

    TFT_SendCmd(0xEA);
    TFT_SendData8(0x00); 
    TFT_SendData8(0x00);

    TFT_SendCmd(0xC0);     //Power control 
    TFT_SendData8(0x21);   //VRH[5:0] 

    TFT_SendCmd(0xC1);     //Power control 
    TFT_SendData8(0x12);   //SAP[2:0];BT[3:0]

    TFT_SendCmd(0xC5);      //VCM control 
    TFT_SendData8(0x32); 	 //3F
    TFT_SendData8(0x3C); 	 //3C

    TFT_SendCmd(0xC7);     //VCM control2 
    TFT_SendData8(0xC1);

    TFT_SendCmd(0x36);     // Memory Access Control 
    TFT_SendData8(0x08);

    TFT_SendCmd(0x3A);   
    TFT_SendData8(0x55);

    TFT_SendCmd(0xB1);     //֡���ʿ���
    TFT_SendData8(0x00);
    TFT_SendData8(0x18); 

    TFT_SendCmd(0xB6);    // Display Function Control 
    TFT_SendData8(0x0A); 
    TFT_SendData8(0xA2);

    TFT_SendCmd(0xF2);    // 3Gamma Function Disable 
    TFT_SendData8(0x00); 

    TFT_SendCmd(0x26);    //Gamma curve selected 
    TFT_SendData8(0x01);

    TFT_SendCmd(0xE0);    //Set Gamma 
    TFT_SendData8(0x0F); 
    TFT_SendData8(0x20); 
    TFT_SendData8(0x1E); 
    TFT_SendData8(0x09); 
    TFT_SendData8(0x12); 
    TFT_SendData8(0x0B); 
    TFT_SendData8(0x50); 
    TFT_SendData8(0xBA); 
    TFT_SendData8(0x44); 
    TFT_SendData8(0x09); 
    TFT_SendData8(0x14); 
    TFT_SendData8(0x05); 
    TFT_SendData8(0x23); 
    TFT_SendData8(0x21); 
    TFT_SendData8(0x00); 


    TFT_SendCmd(0xE1);    //Set Gamma 
    TFT_SendData8(0x00); 
    TFT_SendData8(0x19); 
    TFT_SendData8(0x19); 
    TFT_SendData8(0x00); 
    TFT_SendData8(0x12); 
    TFT_SendData8(0x07); 
    TFT_SendData8(0x2D); 
    TFT_SendData8(0x28); 
    TFT_SendData8(0x3F); 
    TFT_SendData8(0x02); 
    TFT_SendData8(0x0A); 
    TFT_SendData8(0x08); 
    TFT_SendData8(0x25); 
    TFT_SendData8(0x2D); 
    TFT_SendData8(0x0F);


    TFT_SendCmd(0x11); //Exit Sleep

    DelayMsec(120);

    TFT_SendCmd(0x29); //display on	

    TFT_Clear(GREEN);

}




