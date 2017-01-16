////////////////////////////////////////////////////////////////////////////////
//  Compiler          : IAR C/C++ Compiler for ARM, 6.50
//  Microcontroller		: Cortex M4 STM32F407ZET6
//  Revision          : V1.00
//  Project           : CECS_M4 2016
//  Create Date       : 2016-11-30
//  File              : M4_CECS_ISOIN.C
////////////////////////////////////////////////////////////////////////////////
#include "m4_evkit_define.h"

#include "m4_cecs_io_base.h"
#include "m4_cecs_uart1.h"
#include "m4_cecs_uart3.h"
#include "m4_cecs_uart6.h"
#include "m4_cecs_can.h"

#include "m4_cecs_isoin.h"

void InitISOINModule( void )
{
  GPIO_InitTypeDef          GPIO_InitStructure;

  GPIOF->ODR = 0x0000;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL ;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_0  | GPIO_Pin_1  | GPIO_Pin_2  | GPIO_Pin_3  \
                                | GPIO_Pin_4  | GPIO_Pin_5  | GPIO_Pin_6  | GPIO_Pin_7  \
                                | GPIO_Pin_8  | GPIO_Pin_9  | GPIO_Pin_10 | GPIO_Pin_11 \
                                | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_Init( GPIOF, &GPIO_InitStructure );
}

bool CheckInput( u16 nPort )
{
  if( GPIOF->IDR & nPort ) { return true; } else { return false; }
}

void ProcedureISOINModule( u16 nTypeID )
{
  OnBuzzer();   Delay( 100 );
  OffBuzzer();  Delay( 400 );
  OnBuzzer();   Delay( 100 );
  OffBuzzer();  Delay( 400 );

  InitISOINModule();
  
	while( 1 ) {
 /*
      if( USART1_PacketStreaming( &RxPacket ) ) {
      if( (RxPacket.wCanID & 0x00ff) == nTypeID ) {
        switch( RxPacket.wCanID & 0xff00 ) {
          case tagCAN_GET_COUNTER :
            TxPacket.wCanID    = RxPacket.wCanID;
            TxPacket.dwData[0] = TIM3_GetCaptureCFreq();
            TxPacket.dwData[1] = TIM4_GetCaptureCFreq();
            USART1_SendPacket( &TxPacket, true );
            break;
            
          case tagCAN_IN_ISOLATION  :
            TxPacket.wCanID   = RxPacket.wCanID;
            TxPacket.wData[0] = 0xaaaa;//GPIOF->IDR;
            USART1_SendPacket( &TxPacket, true );
            break;

          case tagCAN_ID_SCANNING :
            TxPacket.wCanID  = RxPacket.wCanID;
            USART1_SendPacket( &TxPacket, true );
            break;
            
          default :
            TxPacket.wCanID  = RxPacket.wCanID;
            USART1_SendPacket( &TxPacket, false );
            break;
        }
      } else {
        if( CANTrasmitMessage( &RxPacket ) ) {
          if( GetMessage((LPMESSAGE)&TxPacket) ) {
            if(  (TxPacket.nHeader==ACK_PACKET_HEADER)
              && (TxPacket.wCanID==RxPacket.wCanID) ) {
              USART1_SendPacket( &TxPacket, true );
            }
          }
        } else {
          TxPacket.wCanID = RxPacket.wCanID;
          USART1_SendPacket( &TxPacket, false );
        }
      }
    }
 */   
    if( GetMessage((LPMESSAGE)&RxPacket) ) {
      if( (RxPacket.wCanID & 0x00ff) == nTypeID ) {
        switch( RxPacket.wCanID & 0x0f00 ) {
          case tagCAN_GET_COUNTER :
            TxPacket.wCanID    = RxPacket.wCanID;
            TxPacket.dwData[0] = TIM3_GetCaptureCFreq();
            TxPacket.dwData[1] = TIM4_GetCaptureCFreq();
            CANTrasmitMessage( &TxPacket );
            break;
            
          case tagCAN_IN_ISOLATION  :
            TxPacket.wCanID   = RxPacket.wCanID;
            TxPacket.wData[0] = GPIOF->IDR;
            CANTrasmitMessage( &TxPacket );
            break;

          case tagCAN_ID_SCANNING :
            TxPacket.wCanID  = RxPacket.wCanID;
            CANTrasmitMessage( &TxPacket );
            break;
            
          default :   break;
        }
      }
    }
  }
}
