#define MRC522_API_GLOBAL
#include "include.h" 
////////////////////////////////////////////////////////////////////////////////
// 功能描述 ：读卡里面的数据
// 输入参数 : block:要读哪一块
//  		  rc_au: 密码类型 0x60 A 0x61 B
//            byte_num: 读出字节个数
//            sec_key :所需密码
// 返回参数 : 无
// 说明     : 
////////////////////////////////////////////////////////////////////////////////
void M1_Write(unsigned char rc_au,unsigned char rc_block,unsigned char *sec_key)
{
	 unsigned char status,i,j;
	 unsigned int tem;
	 for(j=0;j<5;j++)
	 {
		 status = PcdRequest(PICC_REQALL, g_ucTempbuf);//寻卡
	     if (status == MI_OK)
	     {   
		     kk_con=0;//一旦有卡接近就清楚无卡状态计数
//			 printf("卡的类型:"); 
//		     for(i=0;i<2;i++)
//			 {
//			    tem=g_ucTempbuf[i];
//			    printf("%X",tem);	
//			 }
			 if(ccon_flag==0)
			 {  
				 status = PcdAnticoll(g_ucTempbuf);//防冲撞
		 		 if (status == MI_OK)
				 {
				 	 printf("卡序列号:");	//超级终端显示,
					 for(i=0;i<4;i++)
				 	 {
					   tem=g_ucTempbuf[i];
					   printf("%X",tem);			
				 	 }
					printf("\n");
					status = PcdSelect(g_ucTempbuf);//选定卡片
				 	if (status == MI_OK)
					{
					    status = PcdAuthState(rc_au, rc_block, sec_key, g_ucTempbuf);//验证卡片密码
						if (status == MI_OK)
						{
						   status = PcdWrite(rc_block, rc_data);//写扇区第0块
						   if (status == MI_OK)	
						   {
						   	   status = PcdWrite(rc_block+1,rc_data);//写块
							   if (status == MI_OK)	
						       {
							       status = PcdWrite(rc_block+2, rc_data);//写块
							       if (status == MI_OK)	
						   	       printf("成功写入！\n");	//超级终端显示,	  					 		         
					               PcdHalt();
					               con_flag=1;
							       rc_blink_read=LED_ON_TIME;
							       break;
								}
							}	   	
						  }
						 
					 }
					
			}
		 }
		}
	     
		 else
		 {

		 	kk_con++;
			if(kk_con==1)
			{
			   PcdReset();
               PcdAntennaOff(); 
               PcdAntennaOn(); 	
			}
			if( kk_con> MAX_K_CON)
			{
				kk_con= MAX_K_CON;
				ccon_flag=0;
			}	 
		 }
	 }	
}
////////////////////////////////////////////////////////////////////////////////
// 功能描述 ：读卡里面的数据
// 输入参数 : block:要读哪一块
//  		  rc_au: 密码类型 0x60 A 0x61 B
//            byte_num: 读出字节个数
//            sec_key :所需密码
// 返回参数 : 无
// 说明     : 
////////////////////////////////////////////////////////////////////////////////
void M1_Read(unsigned char rc_au,unsigned char rc_block,unsigned char byte_num,unsigned char *sec_key)
{
         unsigned char status,i;
	     unsigned int tem;

		 status = PcdRequest(PICC_REQALL, g_ucTempbuf);//寻卡
	     if (status == MI_OK)
	     {   
		     k_con=0;//一旦有卡接近就清楚无卡状态计数
//			 printf("卡的类型:"); 
//		     for(i=0;i<2;i++)
//			 {
//			    tem=g_ucTempbuf[i];
//			    printf("%X",tem);	
//			 }
			 if(con_flag==0)
			 {  
				 status = PcdAnticoll(g_ucTempbuf);//防冲撞
		 		 if (status == MI_OK)
				 {
//				 	 printf("卡序列号:");	//超级终端显示,
//					 for(i=0;i<4;i++)
//				 	 {
//					   tem=g_ucTempbuf[i];
//					   printf("%X",tem);			
//				 	 }
//					printf("\n");
					status = PcdSelect(g_ucTempbuf);//选定卡片
				 	if (status == MI_OK)
					{
					    status = PcdAuthState(rc_au, rc_block, sec_key, g_ucTempbuf);//验证卡片密码
						if (status == MI_OK)
						{
						   status = PcdRead(rc_block, g_ucTempbuf);//读块
						   if (status == MI_OK)	
						   {
//						   	   printf("中华人民军队工号：");	//超级终端显示,	
//							   for(i=0;i<byte_num;i++)
//					   		   {
//							      tem=g_ucTempbuf[i];
//						      	  printf("%X",tem);
//								  	
//					           }
//							   printf("\n");
				 		         
					           PcdHalt();
					           con_flag=1;
							   rc_blink_read=LED_ON_TIME;
							   Send485SerialByte(g_ucTempbuf,20);	
							 //  break;
						   }
						}	
					}
				 }
			 }
	     }
		 else
		 {

		 	k_con++;
			if(k_con==1)
			{
			   PcdReset();
               PcdAntennaOff(); 
			  // DelayMs(1);
               PcdAntennaOn(); 	
			   //DelayMs(1);
			}
			if( k_con> MAX_K_CON)
			{
				k_con= MAX_K_CON;
				con_flag=0;
			}
		 }
		 	
       
}          
//////////////////// ////////////////////////////////////////////////////////////
// 功能描述 : 寻卡
// 输入参数 : req_code[IN]:寻卡方式
//                0x52 = 寻感应区内所有符合14443A标准的卡
//                0x26 = 寻未进入休眠状态的卡
//            pTagType[OUT]：卡片类型代码
//                0x4400 = Mifare_UltraLight
//                0x0400 = Mifare_One(S50)
//                0x0200 = Mifare_One(S70)
//                0x0800 = Mifare_Pro(X)
//                0x4403 = Mifare_DESFire
// 返回参数 : 成功返回MI_OK
// 说明     :       
////////////////////////////////////////////////////////////////////////////////
char PcdRequest(unsigned char req_code,unsigned char *pTagType)
{
   char status;  
   unsigned int  unLen;
   unsigned char ucComMF522Buf[MAXRLEN]; 

   ClearBitMask(Status2Reg,0x08);
   WriteRawRC(BitFramingReg,0x07);
   SetBitMask(TxControlReg,0x03);
 
   ucComMF522Buf[0] = req_code;

   status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,1,ucComMF522Buf,&unLen);
   
   if ((status == MI_OK) && (unLen == 0x10))
   {    
       *pTagType     = ucComMF522Buf[0];
       *(pTagType+1) = ucComMF522Buf[1];
   }
   else
   {   status = MI_ERR;  
 


	}
   
   return status;
}
////////////////////////////////////////////////////////////////////////////////
// 功能描述 : 防冲撞 
// 输入参数 : pSnr[OUT]:卡片序列号，4字节
// 返回参数 : 成功返回MI_OK
// 说明     :       
//////////////////////////////////////////////////////////////////////////////// 
char PcdAnticoll(unsigned char *pSnr)
{
    char status;
    unsigned char i,snr_check=0;
    unsigned int  unLen;
    unsigned char ucComMF522Buf[MAXRLEN]; 
    

    ClearBitMask(Status2Reg,0x08);
    WriteRawRC(BitFramingReg,0x00);
    ClearBitMask(CollReg,0x80);
 
    ucComMF522Buf[0] = PICC_ANTICOLL1;
    ucComMF522Buf[1] = 0x20;

    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,2,ucComMF522Buf,&unLen);

    if (status == MI_OK)
    {
    	 for (i=0; i<4; i++)
         {   
             *(pSnr+i)  = ucComMF522Buf[i];
             snr_check ^= ucComMF522Buf[i];

         }
         if (snr_check != ucComMF522Buf[i])
         {   status = MI_ERR;    }
    }
    
    SetBitMask(CollReg,0x80);
    return status;
}
////////////////////////////////////////////////////////////////////////////////
// 功能描述 : 选定卡片
// 输入参数 : pSnr[IN]:卡片序列号，4字节
// 返回参数 : 成功选定卡片返回MI_OK
// 说明     :       
////////////////////////////////////////////////////////////////////////////////
char PcdSelect(unsigned char *pSnr)
{
    char status;
    unsigned char i;
    unsigned int  unLen;
    unsigned char ucComMF522Buf[MAXRLEN]; 
    
    ucComMF522Buf[0] = PICC_ANTICOLL1;
    ucComMF522Buf[1] = 0x70;
    ucComMF522Buf[6] = 0;
    for (i=0; i<4; i++)
    {
    	ucComMF522Buf[i+2] = *(pSnr+i);
    	ucComMF522Buf[6]  ^= *(pSnr+i);
    }
    CalulateCRC(ucComMF522Buf,7,&ucComMF522Buf[7]);
  
    ClearBitMask(Status2Reg,0x08);

    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,9,ucComMF522Buf,&unLen);
    
    if ((status == MI_OK) && (unLen == 0x18))
    {   status = MI_OK;  }
    else
    {   status = MI_ERR;    }

    return status;
}     
////////////////////////////////////////////////////////////////////////////////
// 功能描述 : 命令卡片进入休眠状态 
// 输入参数 : auth_mode[IN]: 密码验证模式
//                 0x60 = 验证A密钥
//                 0x61 = 验证B密钥 
//            addr[IN]：块地址
//            pKey[IN]：密码
//            pSnr[IN]：卡片序列号，4字节
// 返回参数 : 成功返回MI_OK
// 说明     :       
////////////////////////////////////////////////////////////////////////////////         
char PcdAuthState(unsigned char auth_mode,unsigned char addr,unsigned char *pKey,unsigned char *pSnr)
{
    char status;
    unsigned int  unLen;
    unsigned char i,ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = auth_mode;
    ucComMF522Buf[1] = addr;
    for (i=0; i<6; i++)
    {    ucComMF522Buf[i+2] = *(pKey+i);   }
    for (i=0; i<6; i++)
    {    ucComMF522Buf[i+8] = *(pSnr+i);   }
 //   memcpy(&ucComMF522Buf[2], pKey, 6); 
 //   memcpy(&ucComMF522Buf[8], pSnr, 4); 
    
    status = PcdComMF522(PCD_AUTHENT,ucComMF522Buf,12,ucComMF522Buf,&unLen);
    if ((status != MI_OK) || (!(ReadRawRC(Status2Reg) & 0x08)))
    {   status = MI_ERR;   }
    
    return status;
}
////////////////////////////////////////////////////////////////////////////////
// 功能描述 : 读取M1卡一块数据 
// 输入参数 : addr[IN]：块地址
//            pData[OUT]：读出的数据，16字节
// 返回参数 : 成功返回MI_OK
// 说明     :       
////////////////////////////////////////////////////////////////////////////////
char PcdRead(unsigned char addr,unsigned char *pData)
{
    char status;
    unsigned int  unLen;
    unsigned char i,ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = PICC_READ;
    ucComMF522Buf[1] = addr;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
   
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);
    if ((status == MI_OK) && (unLen == 0x90))
 //   {   memcpy(pData, ucComMF522Buf, 16);   }
    {
        for (i=0; i<16; i++)
        {    *(pData+i) = ucComMF522Buf[i];   }
    }
    else
    {   status = MI_ERR;   }
    
    return status;
}   
////////////////////////////////////////////////////////////////////////////////
// 功能描述 : 写数据到M1卡一块
// 输入参数 : addr[IN]：块地址
//            pData[IN]：写入的数据，16字节
// 返回参数 : 成功返回MI_OK
// 说明     :       
////////////////////////////////////////////////////////////////////////////////            
char PcdWrite(unsigned char addr,unsigned char *pData)
{
    char status;
    unsigned int  unLen;
    unsigned char i,ucComMF522Buf[MAXRLEN]; 
    
    ucComMF522Buf[0] = PICC_WRITE;
    ucComMF522Buf[1] = addr;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

    if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
    {   status = MI_ERR;   }
        
    if (status == MI_OK)
    {
        //memcpy(ucComMF522Buf, pData, 16);
        for (i=0; i<16; i++)
        {    ucComMF522Buf[i] = *(pData+i);   }
        CalulateCRC(ucComMF522Buf,16,&ucComMF522Buf[16]);

        status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,18,ucComMF522Buf,&unLen);
        if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
        {   status = MI_ERR;   }
    }
    
    return status;
} 
////////////////////////////////////////////////////////////////////////////////
// 功能描述 : 扣款和充值
// 输入参数 : dd_mode[IN]：命令字
//               0xC0 = 扣款
//               0xC1 = 充值
//            addr[IN]：钱包地址
//            pValue[IN]：4字节增(减)值，低位在前
// 返回参数 : 成功返回MI_OK
// 说明     :       
////////////////////////////////////////////////////////////////////////////////           
char PcdValue(unsigned char dd_mode,unsigned char addr,unsigned char *pValue)
{
    char status;
    unsigned int  unLen;
    unsigned char i,ucComMF522Buf[MAXRLEN]; 
    
    ucComMF522Buf[0] = dd_mode;
    ucComMF522Buf[1] = addr;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

    if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
    {   status = MI_ERR;   }
        
    if (status == MI_OK)
    {
       // memcpy(ucComMF522Buf, pValue, 4);
        for (i=0; i<16; i++)
        {    ucComMF522Buf[i] = *(pValue+i);   }
        CalulateCRC(ucComMF522Buf,4,&ucComMF522Buf[4]);
        unLen = 0;
        status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,6,ucComMF522Buf,&unLen);
        if (status != MI_ERR)
        {    status = MI_OK;    }
    }
    
    if (status == MI_OK)
    {
        ucComMF522Buf[0] = PICC_TRANSFER;
        ucComMF522Buf[1] = addr;
        CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]); 
   
        status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

        if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
        {   status = MI_ERR;   }
    }
    return status;
}
////////////////////////////////////////////////////////////////////////////////
// 功能描述 : 备份钱包
// 输入参数 : sourceaddr[IN]：源地址    goaladdr[IN]：目标地址           
// 返回参数 : 成功返回MI_OK
// 说明     :       
////////////////////////////////////////////////////////////////////////////////
char PcdBakValue(unsigned char sourceaddr, unsigned char goaladdr)
{
    char status;
    unsigned int  unLen;
    unsigned char ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = PICC_RESTORE;
    ucComMF522Buf[1] = sourceaddr;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

    if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
    {   status = MI_ERR;   }
    
    if (status == MI_OK)
    {
        ucComMF522Buf[0] = 0;
        ucComMF522Buf[1] = 0;
        ucComMF522Buf[2] = 0;
        ucComMF522Buf[3] = 0;
        CalulateCRC(ucComMF522Buf,4,&ucComMF522Buf[4]);
 
        status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,6,ucComMF522Buf,&unLen);
        if (status != MI_ERR)
        {    status = MI_OK;    }
    }
    
    if (status != MI_OK)
    {    return MI_ERR;   }
    
    ucComMF522Buf[0] = PICC_TRANSFER;
    ucComMF522Buf[1] = goaladdr;

    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

    if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
    {   status = MI_ERR;   }

    return status;
}
////////////////////////////////////////////////////////////////////////////////
// 功能描述 : 命令卡片进入休眠状态 
// 输入参数 : 
// 返回参数 : 成功返回MI_OK
// 说明     :       
////////////////////////////////////////////////////////////////////////////////
char PcdHalt(void)
{
    char status;
    unsigned int  unLen;
    unsigned char ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = PICC_HALT;
    ucComMF522Buf[1] = 0;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

    return MI_OK;
}
////////////////////////////////////////////////////////////////////////////////
// 功能描述 : 用MF522计算CRC16函数 
// 输入参数 : 
// 返回参数 : 
// 说明     :       
////////////////////////////////////////////////////////////////////////////////
void CalulateCRC(unsigned char *pIndata,unsigned char len,unsigned char *pOutData)
{
    unsigned char i,n;
    ClearBitMask(DivIrqReg,0x04);
    WriteRawRC(CommandReg,PCD_IDLE);
    SetBitMask(FIFOLevelReg,0x80);
    for (i=0; i<len; i++)
    {   WriteRawRC(FIFODataReg, *(pIndata+i));   }
    WriteRawRC(CommandReg, PCD_CALCCRC);
    i = 0xFF;
    do 
    {
        n = ReadRawRC(DivIrqReg);
        i--;
    }
    while ((i!=0) && !(n&0x04));
    pOutData[0] = ReadRawRC(CRCResultRegL);
    pOutData[1] = ReadRawRC(CRCResultRegM);
}
////////////////////////////////////////////////////////////////////////////////
// 功能描述 : 复位RC522 
// 输入参数 : 
// 返回参数 : 成功返回MI_OK
// 说明     :       
////////////////////////////////////////////////////////////////////////////////
char PcdReset(void)
{
	unsigned char i;
    MF522_RST=1;

		_nop_();                  

    MF522_RST=0;

		_nop_();                   

    MF522_RST=1;

		DelayMs(1);               
	
    WriteRawRC(CommandReg,PCD_RESETPHASE);

		_nop_();                  
	
    
    WriteRawRC(ModeReg,0x3D);            //和Mifare卡通讯，CRC初始值0x6363
    WriteRawRC(TReloadRegL,30);           
    WriteRawRC(TReloadRegH,0);
    WriteRawRC(TModeReg,0x8D);
    WriteRawRC(TPrescalerReg,0x3E);
    WriteRawRC(TxAutoReg,0x40);
	i=ReadRawRC(TReloadRegL);
	//while((i!=30));
    return MI_OK;
}
////////////////////////////////////////////////////////////////////////////////
// 功能描述 : 通过RC522和ISO14443卡通讯
// 输入参数 : 
// 返回参数 : 
// 说明     : Command[IN]:RC522命令字
//            pInData[IN]:通过RC522发送到卡片的数据
//            InLenByte[IN]:发送数据的字节长度
//         	  pOutData[OUT]:接收到的卡片返回数据
//            *pOutLenBit[OUT]:返回数据的位长度      
////////////////////////////////////////////////////////////////////////////////
char PcdComMF522(unsigned char Command, 
                 unsigned char *pInData, 
                 unsigned char InLenByte,
                 unsigned char *pOutData, 
                 unsigned int  *pOutLenBit)
{
    char status = MI_ERR;
    unsigned char irqEn   = 0x00;
    unsigned char waitFor = 0x00;
    unsigned char lastBits;
    unsigned char n;
    unsigned int i;
    switch (Command)
    {
       case PCD_AUTHENT:
          irqEn   = 0x12;
          waitFor = 0x10;
          break;
       case PCD_TRANSCEIVE:
          irqEn   = 0x77;
          waitFor = 0x30;
          break;
       default:
         break;
    }
   
    WriteRawRC(ComIEnReg,irqEn|0x80);
    ClearBitMask(ComIrqReg,0x80);
    WriteRawRC(CommandReg,PCD_IDLE);
    SetBitMask(FIFOLevelReg,0x80);
    
    for (i=0; i<InLenByte; i++)
    {   WriteRawRC(FIFODataReg, pInData[i]);    }
    WriteRawRC(CommandReg, Command);
   
    
    if (Command == PCD_TRANSCEIVE)
    {    SetBitMask(BitFramingReg,0x80);  }
    
    i = 600;//根据时钟频率调整，操作M1卡最大等待时间25ms
    do 
    {
         n = ReadRawRC(ComIrqReg);
         i--;
    }
    while ((i!=0) && !(n&0x01) && !(n&waitFor));
    ClearBitMask(BitFramingReg,0x80);
	      
    if (i!=0)
    {    
         if(!(ReadRawRC(ErrorReg)&0x1B))
         {
             status = MI_OK;
             if (n & irqEn & 0x01)
             {   status = MI_NOTAGERR;   }
             if (Command == PCD_TRANSCEIVE)
             {
               	n = ReadRawRC(FIFOLevelReg);
              	lastBits = ReadRawRC(ControlReg) & 0x07;
                if (lastBits)
                {   *pOutLenBit = (n-1)*8 + lastBits;   }
                else
                {   *pOutLenBit = n*8;   }
                if (n == 0)
                {   n = 1;    }
                if (n > MAXRLEN)
                {   n = MAXRLEN;   }
                for (i=0; i<n; i++)
                {   pOutData[i] = ReadRawRC(FIFODataReg);    }
            }
         }
         else
         {   status = MI_ERR;   }
        
   }
   

   SetBitMask(ControlReg,0x80);           // stop timer now
   WriteRawRC(CommandReg,PCD_IDLE); 
   return status;
}
////////////////////////////////////////////////////////////////////////////////
// 功能描述 : 开启天线  
// 输入参数 : 
// 返回参数 : 
// 说明     : 每次启动或关闭天险发射之间应至少有1ms的间隔      
////////////////////////////////////////////////////////////////////////////////
void PcdAntennaOn()
{
    unsigned char i;
    i = ReadRawRC(TxControlReg);
    if (!(i & 0x03))
    {
        SetBitMask(TxControlReg, 0x03);
    }
}
////////////////////////////////////////////////////////////////////////////////
// 功能描述 : 关闭天线 
// 输入参数 : 
// 返回参数 : 
// 说明     :       
////////////////////////////////////////////////////////////////////////////////
void PcdAntennaOff()
{
    ClearBitMask(TxControlReg, 0x03);
}