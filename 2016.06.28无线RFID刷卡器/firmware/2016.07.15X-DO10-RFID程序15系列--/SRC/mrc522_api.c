#define MRC522_API_GLOBAL
#include "include.h" 
////////////////////////////////////////////////////////////////////////////////
// �������� ���������������
// ������� : block:Ҫ����һ��
//  		  rc_au: �������� 0x60 A 0x61 B
//            byte_num: �����ֽڸ���
//            sec_key :��������
// ���ز��� : ��
// ˵��     : 
////////////////////////////////////////////////////////////////////////////////
void M1_Write(unsigned char rc_au,unsigned char rc_block,unsigned char *sec_key)
{
	 unsigned char status,i,j;
	 unsigned int tem;
	 for(j=0;j<5;j++)
	 {
		 status = PcdRequest(PICC_REQALL, g_ucTempbuf);//Ѱ��
	     if (status == MI_OK)
	     {   
		     kk_con=0;//һ���п��ӽ�������޿�״̬����
//			 printf("��������:"); 
//		     for(i=0;i<2;i++)
//			 {
//			    tem=g_ucTempbuf[i];
//			    printf("%X",tem);	
//			 }
			 if(ccon_flag==0)
			 {  
				 status = PcdAnticoll(g_ucTempbuf);//����ײ
		 		 if (status == MI_OK)
				 {
				 	 printf("�����к�:");	//�����ն���ʾ,
					 for(i=0;i<4;i++)
				 	 {
					   tem=g_ucTempbuf[i];
					   printf("%X",tem);			
				 	 }
					printf("\n");
					status = PcdSelect(g_ucTempbuf);//ѡ����Ƭ
				 	if (status == MI_OK)
					{
					    status = PcdAuthState(rc_au, rc_block, sec_key, g_ucTempbuf);//��֤��Ƭ����
						if (status == MI_OK)
						{
						   status = PcdWrite(rc_block, rc_data);//д������0��
						   if (status == MI_OK)	
						   {
						   	   status = PcdWrite(rc_block+1,rc_data);//д��
							   if (status == MI_OK)	
						       {
							       status = PcdWrite(rc_block+2, rc_data);//д��
							       if (status == MI_OK)	
						   	       printf("�ɹ�д�룡\n");	//�����ն���ʾ,	  					 		         
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
// �������� ���������������
// ������� : block:Ҫ����һ��
//  		  rc_au: �������� 0x60 A 0x61 B
//            byte_num: �����ֽڸ���
//            sec_key :��������
// ���ز��� : ��
// ˵��     : 
////////////////////////////////////////////////////////////////////////////////
void M1_Read(unsigned char rc_au,unsigned char rc_block,unsigned char byte_num,unsigned char *sec_key)
{
         unsigned char status,i;
	     unsigned int tem;

		 status = PcdRequest(PICC_REQALL, g_ucTempbuf);//Ѱ��
	     if (status == MI_OK)
	     {   
		     k_con=0;//һ���п��ӽ�������޿�״̬����
//			 printf("��������:"); 
//		     for(i=0;i<2;i++)
//			 {
//			    tem=g_ucTempbuf[i];
//			    printf("%X",tem);	
//			 }
			 if(con_flag==0)
			 {  
				 status = PcdAnticoll(g_ucTempbuf);//����ײ
		 		 if (status == MI_OK)
				 {
//				 	 printf("�����к�:");	//�����ն���ʾ,
//					 for(i=0;i<4;i++)
//				 	 {
//					   tem=g_ucTempbuf[i];
//					   printf("%X",tem);			
//				 	 }
//					printf("\n");
					status = PcdSelect(g_ucTempbuf);//ѡ����Ƭ
				 	if (status == MI_OK)
					{
					    status = PcdAuthState(rc_au, rc_block, sec_key, g_ucTempbuf);//��֤��Ƭ����
						if (status == MI_OK)
						{
						   status = PcdRead(rc_block, g_ucTempbuf);//����
						   if (status == MI_OK)	
						   {
//						   	   printf("�л�������ӹ��ţ�");	//�����ն���ʾ,	
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
// �������� : Ѱ��
// ������� : req_code[IN]:Ѱ����ʽ
//                0x52 = Ѱ��Ӧ�������з���14443A��׼�Ŀ�
//                0x26 = Ѱδ��������״̬�Ŀ�
//            pTagType[OUT]����Ƭ���ʹ���
//                0x4400 = Mifare_UltraLight
//                0x0400 = Mifare_One(S50)
//                0x0200 = Mifare_One(S70)
//                0x0800 = Mifare_Pro(X)
//                0x4403 = Mifare_DESFire
// ���ز��� : �ɹ�����MI_OK
// ˵��     :       
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
// �������� : ����ײ 
// ������� : pSnr[OUT]:��Ƭ���кţ�4�ֽ�
// ���ز��� : �ɹ�����MI_OK
// ˵��     :       
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
// �������� : ѡ����Ƭ
// ������� : pSnr[IN]:��Ƭ���кţ�4�ֽ�
// ���ز��� : �ɹ�ѡ����Ƭ����MI_OK
// ˵��     :       
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
// �������� : ���Ƭ��������״̬ 
// ������� : auth_mode[IN]: ������֤ģʽ
//                 0x60 = ��֤A��Կ
//                 0x61 = ��֤B��Կ 
//            addr[IN]�����ַ
//            pKey[IN]������
//            pSnr[IN]����Ƭ���кţ�4�ֽ�
// ���ز��� : �ɹ�����MI_OK
// ˵��     :       
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
// �������� : ��ȡM1��һ������ 
// ������� : addr[IN]�����ַ
//            pData[OUT]�����������ݣ�16�ֽ�
// ���ز��� : �ɹ�����MI_OK
// ˵��     :       
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
// �������� : д���ݵ�M1��һ��
// ������� : addr[IN]�����ַ
//            pData[IN]��д������ݣ�16�ֽ�
// ���ز��� : �ɹ�����MI_OK
// ˵��     :       
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
// �������� : �ۿ�ͳ�ֵ
// ������� : dd_mode[IN]��������
//               0xC0 = �ۿ�
//               0xC1 = ��ֵ
//            addr[IN]��Ǯ����ַ
//            pValue[IN]��4�ֽ���(��)ֵ����λ��ǰ
// ���ز��� : �ɹ�����MI_OK
// ˵��     :       
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
// �������� : ����Ǯ��
// ������� : sourceaddr[IN]��Դ��ַ    goaladdr[IN]��Ŀ���ַ           
// ���ز��� : �ɹ�����MI_OK
// ˵��     :       
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
// �������� : ���Ƭ��������״̬ 
// ������� : 
// ���ز��� : �ɹ�����MI_OK
// ˵��     :       
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
// �������� : ��MF522����CRC16���� 
// ������� : 
// ���ز��� : 
// ˵��     :       
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
// �������� : ��λRC522 
// ������� : 
// ���ز��� : �ɹ�����MI_OK
// ˵��     :       
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
	
    
    WriteRawRC(ModeReg,0x3D);            //��Mifare��ͨѶ��CRC��ʼֵ0x6363
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
// �������� : ͨ��RC522��ISO14443��ͨѶ
// ������� : 
// ���ز��� : 
// ˵��     : Command[IN]:RC522������
//            pInData[IN]:ͨ��RC522���͵���Ƭ������
//            InLenByte[IN]:�������ݵ��ֽڳ���
//         	  pOutData[OUT]:���յ��Ŀ�Ƭ��������
//            *pOutLenBit[OUT]:�������ݵ�λ����      
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
    
    i = 600;//����ʱ��Ƶ�ʵ���������M1�����ȴ�ʱ��25ms
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
// �������� : ��������  
// ������� : 
// ���ز��� : 
// ˵��     : ÿ��������ر����շ���֮��Ӧ������1ms�ļ��      
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
// �������� : �ر����� 
// ������� : 
// ���ز��� : 
// ˵��     :       
////////////////////////////////////////////////////////////////////////////////
void PcdAntennaOff()
{
    ClearBitMask(TxControlReg, 0x03);
}