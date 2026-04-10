#include "bsp.h"

uint8_t TCMQTTRCVPUB[40];

uint8_t rx_app_timer_power_on_flag;

typedef enum _CLOUD_STATE{
   OPEN_OFF_ITEM=0x01,
   OPEN_ON_ITEM,
   PTC_OFF_ITEM,
   PTC_ON_ITEM,
   ANION_OFF_ITEM,
   ANION_ON_ITEM,
   SONIC_OFF_ITEM,
   SONIC_ON_ITEM,
   STATE_AI_MODEL_ITEM,
   STATE_TIMER_MODEL_ITEM,
   TEMPERATURE_ITEM,
   FAN_ITEM,
   APP_TIMER_POWER_ON_REF,
   APP_TIMER_POWER_ON_REF_TWO,
}cloud_state;


static void Wifi_Rx_InputInfo_Handler(void);
//void Subscribe_Rx_Interrupt_Handler(void);




typedef struct PROCESS_T{

   uint8_t  rx_data_array[150];
  
   uint8_t  rx_numbers;
   uint8_t  rx_inputBuf[1];
   uint8_t  linking_tencent_cloud_doing;
   uint8_t  soft_ap_config_success;
   uint8_t  once_rx_data_done;
   uint8_t  rx_data_state;
   uint8_t  rx_counter;
   uint8_t  received_data_from_tencent_cloud ;
   uint8_t  rx_data_success;
   uint8_t  response_wifi_signal_label;
   
 }process_t;

process_t wifi_t;

/**
*@brief :
*
*
*
**/

/*******************************************************************************
   **
   *Function Name:void usart2_rx_callback_invoke(uint8_t data)
   *Function: receive data 
   *Input Ref: module , str ->data
   *Return Ref:NO
   *
********************************************************************************/
void usart2_rx_callback_invoke(uint8_t data)
{

     wifi_t.rx_inputBuf[0] =data;
     if(wifi_t.linking_tencent_cloud_doing ==1){

			wifi_t.rx_data_array[wifi_t.rx_numbers] =wifi_t.rx_inputBuf[0];
			wifi_t.rx_numbers++;

			if(*wifi_t.rx_inputBuf==0x0A) // 0x0A = "\n"
			{
				
				Wifi_Rx_InputInfo_Handler();
				wifi_t.rx_numbers=0;
			}

	 } 
	 else
		  Subscribe_Rx_Interrupt_Handler();
}
	 #if 0
     else{

		    if(wifi_t.get_rx_beijing_time_enable==1){
					wifi_t.rx_data_array[gpro_t.wifi_rx_data_counter] = wifi_t.rx_inputBuf[0];
					gpro_t.wifi_rx_data_counter++;
					
			}
			else
			    Subscribe_Rx_Interrupt_Handler();
	 }
	 #endif 



/*******************************************************************************
**
*Function Name:void Publish_Data_ToCloud(void)
*Function: dy
*Input Ref: 
*Return Ref:NO
*
********************************************************************************/
void Subscriber_Data_FromCloud_Handler(void)
{
  // uint8_t *device_massage;

          uint8_t  device_massage[128] ={0} ;//(uint8_t *)malloc(128);
         // gctl_t.randomName[0]=HAL_GetUIDw0();
      
        // sprintf((char *)device_massage,"AT+TCMQTTSUB=\"$thing/down/property/%s/UYIJIA01-%d\",0\r\n", PRODUCT_ID, gctl_t.randomName[0]);
//         HAL_UART_Transmit(&huart2, device_massage, strlen((const char *)device_massage), 5000); 
         //at_send_data(device_massage, strlen((const char *)device_massage));
         delay_ms(200);
        // free(device_massage);
       //  USART2_DMA_Send(device_massage, sizeof(device_massage));
         delay_ms(300);
}
/*******************************************************************************
**
*Function Name:void Subscribe_Rx_IntHandler(void)
*Function: interrupt USART2 receive data fun
*Input Ref: 
*Return Ref:NO
*
********************************************************************************/
void Subscribe_Rx_Interrupt_Handler(void)
{
     
    switch(wifi_t.rx_data_state)
      {
      case 0: //#0

	  	if((wifi_t.rx_inputBuf[0] == 'p') ||wifi_t.rx_inputBuf[0]=='E' || wifi_t.rx_inputBuf[0] =='T') //hex :54 - "T" -fixed
            wifi_t.rx_data_state=1; //=1
          else{
               wifi_t.rx_counter=0;
               wifi_t.rx_data_state=0; 
            }
        break;

      case 1:
      
         if((wifi_t.rx_inputBuf[0] == 'a')  ||wifi_t.rx_inputBuf[0]=='R' || wifi_t.rx_inputBuf[0] =='C')//hex :54 - "T" -fixed
            wifi_t.rx_data_state=2; //=1
          else{
               wifi_t.rx_counter=0;
               wifi_t.rx_data_state=1;
            }
            
         break;
      case 2: //#1
             if((wifi_t.rx_inputBuf[0] == 'r')||wifi_t.rx_inputBuf[0]=='R'|| wifi_t.rx_inputBuf[0] =='M')  //hex :4B - "K" -fixed
            wifi_t.rx_data_state=3; //=1
         else{
            wifi_t.rx_data_state =0;
             wifi_t.rx_counter=0;
         }
         break;
            
        case 3:
            if((wifi_t.rx_inputBuf[0] == 'a')||wifi_t.rx_inputBuf[0]=='o'|| wifi_t.rx_inputBuf[0] =='Q')    //hex :4B - "K" -fixed
            wifi_t.rx_data_state=4; //=1
         else{
           wifi_t.rx_data_state =0;
             wifi_t.rx_counter=0;
         }
        
        break;
        
        case 4:
            if((wifi_t.rx_inputBuf[0] == 'm')  ||wifi_t.rx_inputBuf[0]=='R'|| wifi_t.rx_inputBuf[0] =='T')  //hex :4B - "K" -fixed
            wifi_t.rx_data_state=5; //=1
         else{
            wifi_t.rx_data_state =0;
             wifi_t.rx_counter=0;
         }
        
        break;

      case 5:
       if((wifi_t.rx_inputBuf[0] == 's') ||wifi_t.rx_inputBuf[0]==':'|| wifi_t.rx_inputBuf[0] =='T')   //hex :4B - "K" -fixed
         wifi_t.rx_data_state=6; //=1
         else{
           wifi_t.rx_data_state=0;
            wifi_t.rx_counter=0;
         }
            
      break;

      
      case 6:
       if((wifi_t.rx_inputBuf[0] == '"')||wifi_t.rx_inputBuf[0]=='2' || wifi_t.rx_inputBuf[0] =='S')    //hex :4B - "K" -fixed
         wifi_t.rx_data_state=7; //=1
         else{
           wifi_t.rx_data_state =0;
            wifi_t.rx_counter=0;
         }
            
      break;

      case 7:
       if((wifi_t.rx_inputBuf[0] == ':')||wifi_t.rx_inputBuf[0]=='0' ||wifi_t.rx_inputBuf[0]=='T' ){  //hex :4B - "K" -fixed
         wifi_t.rx_data_state=8; //=1
    	}
		else{
           wifi_t.rx_data_state =0;
            wifi_t.rx_counter=0;
         }
           
      break;

       case 8:
       if((wifi_t.rx_inputBuf[0] == '{') ||wifi_t.rx_inputBuf[0]=='8' ||wifi_t.rx_inputBuf[0]=='A' ){ //hex :4B - "K" -fixed
         if(wifi_t.rx_inputBuf[0]=='8')  wifi_t.rx_data_state =11;
         else if(wifi_t.rx_inputBuf[0]=='A')  wifi_t.rx_data_state =12;
         else
             wifi_t.rx_data_state =9; //=1

        }
         else{
           wifi_t.rx_data_state =0;
            wifi_t.rx_counter=0;
         }

		
            
      break;


      case 9:

       if(wifi_t.rx_data_success ==0){

            wifi_t.rx_data_array[wifi_t.rx_counter] = wifi_t.rx_inputBuf[0];
            wifi_t.rx_counter++ ;

            if(wifi_t.rx_inputBuf[0]=='}' || wifi_t.rx_inputBuf[0]==0x0A) //0x7D='}', 0x0A = line feed // end
            {
                wifi_t.rx_data_success=1;
                wifi_t.rx_data_state=0;
                wifi_t.received_data_from_tencent_cloud = wifi_t.rx_counter;
                wifi_t.rx_counter=0;


            }


       }
       else{
            wifi_t.rx_data_success=0;
            wifi_t.rx_data_state =0;
            wifi_t.rx_counter=0;
            wifi_t.received_data_from_tencent_cloud =0;


       }

     break;

     
      case 11:
		 //net_t.wifi_link_net_success =0; //wifi_t.esp8266_login_cloud_success =0;
         //gpro_t.get_beijing_time_success = 0;
         wifi_t.rx_data_state =0;
         wifi_t.rx_counter=0;

      break;

      case 12:
		if(wifi_t.rx_inputBuf[0]== 'T'){   //hex :4B - "K" -fixed
         wifi_t.rx_data_state=13; //=1
        }
         else{
            wifi_t.rx_data_state =0;
            wifi_t.rx_counter=0;
         }

      break;

	  case 13:

       if(wifi_t.rx_inputBuf[0]== 'E'){   //hex :4B - "K" -fixed
          wifi_t.rx_data_state=14; //=1
        }
        else{
             wifi_t.rx_data_state =0;
             wifi_t.rx_counter=0;
         }

      break;

	  case 14:
		if(wifi_t.rx_inputBuf[0]== ':'){   //hex :4B - "K" -fixed
          wifi_t.rx_data_state=15; //=1
         }
         else{
             wifi_t.rx_data_state =0;
             wifi_t.rx_counter=0;
         }

      break;

	  case 15:
		if(wifi_t.rx_inputBuf[0]== '0'){   //hex :4B - "K" -fixed
            wifi_connected_f =0;//net_t.wifi_link_net_success = 0;//wifi_t.esp8266_login_cloud_success =0;
            //gpro_t.get_beijing_time_success = 0;
            wifi_t.rx_data_state =0;
            wifi_t.rx_counter=0;
        }
        else if(wifi_t.rx_inputBuf[0]== '1'){

            wifi_connected_f =1;//net_t.wifi_link_net_success = 1;//net_t.esp8266_login_cloud_success =1;
       
             wifi_t.rx_data_state =0;
             wifi_t.rx_counter=0;
        }
        else{

           wifi_t.rx_data_state =0;
           wifi_t.rx_counter=0;


        }

      break;

	

	  default:
      break;
      }
 

}
 
/*******************************************************************************
**
*Function Name:void Subscribe_Rx_IntHandler(void)
*Function: interrupt USART2 receive data fun
*Input Ref: +TCMQTTCONN:OK
*Return Ref:NO
*
********************************************************************************/
static void Wifi_Rx_InputInfo_Handler(void)
{
    
  if(strstr((const char*)wifi_t.rx_data_array,"+TCSAP:WIFI_CONNECT_SUCCESS")){
    wifi_t.soft_ap_config_success=1;

  }

  if(strstr((const char*)wifi_t.rx_data_array,"+TCMQTTCONN:OK")){
    wifi_connected_f =1 ;//net_t.wifi_link_net_success=1;
    wifi_t.linking_tencent_cloud_doing=0;
   
  }
  else if(strstr((char*)wifi_t.rx_data_array,"+CME ERROR:208")){

    wifi_t.linking_tencent_cloud_doing=0; //release this flag. usart

    wifi_connected_f=0;//net_t.wifi_link_net_success=0;

  }
  else if(strstr((char*)wifi_t.rx_data_array,"+TCPRDINFOSET:1,\"EHQB1P53IH\"")){ //WT.EDIT 2024.07.22

  // wifi_t.serch_for_wifi_flag=1;
  //  wifi_t.gTime_link_time_start =0;

  }
  else  if(strstr((char*)wifi_t.rx_data_array,"+TCMQTTCONN:FAIL,202")){

    //  wifi_t.esp8266_login_cloud_success =0;
    wifi_connected_f=0;
    key_net_config_f =0;//gpro_t.wifi_led_fast_blink_flag=0;   //WT.EDIT .2024.07.31

    //wifi_t.linking_tencent_cloud_doing=1; //release this flag. usart
    wifi_t.linking_tencent_cloud_doing=0;
    //wifi_t.get_rx_beijing_time_enable=0;
  }
  else if(strstr((const char*)wifi_t.rx_data_array,"+TCMQTTCONN:OK")){
    wifi_connected_f=1;
    wifi_t.linking_tencent_cloud_doing=0;
   
   
  }
  else if(strstr((char*)wifi_t.rx_data_array,"+CME ERROR:208")){


    wifi_connected_f =0;//net_t.wifi_link_net_success=0;//wifi_t.esp8266_login_cloud_success =0;
    wifi_t.linking_tencent_cloud_doing=1;//wifi_t.linking_tencent_cloud_doing=1; //release this flag. usart
   // wifi_t.get_rx_beijing_time_enable=0;

  }
  else if(strstr((char*)wifi_t.rx_data_array,"+TCMQTTCONN:FAIL,202")){

    wifi_connected_f=0;//net_t.wifi_link_net_success=0; //wifi_t.esp8266_login_cloud_success =0;
    key_net_config_f =0;//gpro_t.wifi_led_fast_blink_flag=0;   //WT.EDIT .2024.07.31

    wifi_t.linking_tencent_cloud_doing=1;//wifi_t.linking_tencent_cloud_doing=1; //release this flag. usart

    
  }
  else if(strstr((char*)wifi_t.rx_data_array,"+TCMQTTDISCON")){

    wifi_connected_f=0;//net_t.wifi_link_net_success=0;//wifi_t.esp8266_login_cloud_success =0;
    //wifi_t.get_rx_beijing_time_enable=0;
    wifi_t.linking_tencent_cloud_doing=1;//wifi_t.linking_tencent_cloud_doing=1; //release this flag. usart
  }
  else if(strstr((char*)wifi_t.rx_data_array,"+TCSAP:WIFI_CONNECT_FAILED")){

    wifi_connected_f=0;//wifi_t.esp8266_login_cloud_success =0;
    //wifi_t.get_rx_beijing_time_enable=0;
    wifi_t.linking_tencent_cloud_doing=1;//wifi_t.linking_tencent_cloud_doing=1; //release this flag. usart
    }

  wifi_t.rx_numbers=0;
  wifi_t.once_rx_data_done = 1;
         
}
/*******************************************************************************
    **
    *Function Name:void Tencent_Cloud_Rx_Handler(void)
    *Function: 
    *Input Ref: +TCMQTTCONN:OK
    *Return Ref:NO
    *
********************************************************************************/
void Tencent_Cloud_Rx_Handler(void)
{
    
   
    if(wifi_t.rx_data_success==1){
            wifi_t.rx_data_success=0;
        
	    
	if(wifi_t.received_data_from_tencent_cloud > 22){
	    wifi_t.received_data_from_tencent_cloud=0;
		//wifi_t.get_rx_beijing_time_enable=0;
		wifi_t.response_wifi_signal_label = APP_TIMER_POWER_ON_REF;
       
	   // __HAL_UART_CLEAR_OREFLAG(&huart2);
		strncpy((char*)TCMQTTRCVPUB,(char *)wifi_t.rx_data_array,40);
	   // time_wifi_send_counter=0;
	
	}
	else{
	if(strstr((char *)wifi_t.rx_data_array,"open\":0")){
		  wifi_t.response_wifi_signal_label = OPEN_OFF_ITEM;
	    //  time_wifi_send_counter=0;
		 
	}
	else if(strstr((char *)wifi_t.rx_data_array,"open\":1")){
	   
	   wifi_t.response_wifi_signal_label = OPEN_ON_ITEM;
	   // time_wifi_send_counter=0;
	}


	
	if(strstr((char *)wifi_t.rx_data_array,"ptc\":0")){
            if(discharge_f == 1){
				PTC_heat_open_f =0;  //gpro_t.rx_ptc_flag = 0;//gctl_t.gDry=0;
                ptc_prohibit_off_f = 1;//WT.EDIT 2026.03.30
	           wifi_t.response_wifi_signal_label = PTC_OFF_ITEM;
			  // time_wifi_send_counter=0;
	         
             }
			
    }
    else if(strstr((char *)wifi_t.rx_data_array,"ptc\":1")){
            if(discharge_f == 1){
	          PTC_heat_open_f = 1;//gpro_t.rx_ptc_flag =1;//gctl_t.gDry=1;
              ptc_prohibit_off_f = 0;
			  wifi_t.response_wifi_signal_label = PTC_ON_ITEM;
			 // time_wifi_send_counter=0;
				
            }

    }
	
    if(strstr((char *)wifi_t.rx_data_array,"Anion\":0")){
          if(discharge_f == 1){
	         plasma_open_f =0; //  gctl_t.gPlasma=0;
			wifi_t.response_wifi_signal_label = ANION_OFF_ITEM;
		  //time_wifi_send_counter=0;
		    
             }
		 
    }
    else if(strstr((char *)wifi_t.rx_data_array,"Anion\":1")){
            if(discharge_f == 1){
            plasma_open_f =1;//gctl_t.gPlasma=1;
			wifi_t.response_wifi_signal_label = ANION_ON_ITEM;
			//time_wifi_send_counter=0;
		
            }
    }
	
    if(strstr((char *)wifi_t.rx_data_array,"sonic\":0")){
            if(discharge_f == 1){
            Ultra_Sound_open_f =0;// gctl_t.gUlransonic=0;
			wifi_t.response_wifi_signal_label = SONIC_OFF_ITEM;
			//time_wifi_send_counter=0;
        
                
            }
		
    }
    else if(strstr((char *)wifi_t.rx_data_array,"sonic\":1")){
            if(discharge_f == 1){
            Ultra_Sound_open_f = 1;//gctl_t.gUlransonic=1;
			wifi_t.response_wifi_signal_label = SONIC_ON_ITEM;
			//time_wifi_send_counter=0;
       
           }
			
    }

	
    if(strstr((char *)wifi_t.rx_data_array,"state\":1")){
           if(discharge_f == 1){
            AI_timing_open_f = 1;//gctl_t.gModel=1;
			wifi_t.response_wifi_signal_label = STATE_AI_MODEL_ITEM;
		  // time_wifi_send_counter=0;
        	}
		  
    }
    else if(strstr((char *)wifi_t.rx_data_array,"state\":2")){
            if(discharge_f == 1){
            AI_timing_open_f = 0; //gctl_t.gModel=2;
			wifi_t.response_wifi_signal_label = STATE_TIMER_MODEL_ITEM;
			//time_wifi_send_counter=0;
            }
			
    }
	
    if(strstr((char *)wifi_t.rx_data_array,"temperature")){

	        if(discharge_f == 1){
			  wifi_t.response_wifi_signal_label = TEMPERATURE_ITEM;
			//  time_wifi_send_counter=0;
            
	        }
			
    }
   if(strstr((char *)wifi_t.rx_data_array,"find")){

		 if(discharge_f == 1){

			wifi_t.response_wifi_signal_label= FAN_ITEM;
		//	time_wifi_send_counter=0;
		}
	}
 
    }
    }
 }

/****************************************************************************
*****************************************************************************/
void Json_Parse_Command_Fun(void)
{
   #if 0
 
    static uint8_t wind_hundred, wind_decade,wind_unit,temp_decade,temp_unit;
	//static uint8_t buzzer_temp_on,phone_power_flag;
    

   switch(wifi_t.response_wifi_signal_label){
  
	case OPEN_ON_ITEM:
        if(wifi_connected_f==1){ //WT.EDIT 2025.03.27
             Beep(BEEP_ONCE);
			
         
            //gpro_t.phone_power_on_flag = 1; //ack_app_power_on;
	       
		   // SendWifiData_To_Cmd(0x20,0x01); //smart phone is power on
		
			delay_ms(100);//delay_ms(100);
			MqttData_Publish_SetOpen(1);  
			delay_ms(200);//delay_ms(200);//HAL_Delay(100);//delay_ms(100);//HAL_Delay(100);

	       // Publish_Data_ToTencent_Initial_Data();
		    delay_ms(200);//delay_ms(200);//HAL_Delay(200);

	       

			
			wifi_t.response_wifi_signal_label = 0xfe;
        }

	  break;

       case OPEN_OFF_ITEM:

	      
         if(wifi_connected_f==1){  //WT.EDIT 2025.03.27
             Beep(BEEP_ONCE);// Beep(BEEP_ONCE);
		 	 
		
	
            // SendWifiData_To_Cmd(0x20,0x0); //smart phone is power off
             delay_ms(100);//delay_ms(100);
			 MqttData_Publish_SetOpen(0); 
		     delay_ms(200);
			
	
         
            wifi_t.response_wifi_signal_label = 0xfe;
             }
       
	  break;

	  case PTC_ON_ITEM:
	  if(discharge_f == 1){

         Beep(BEEP_ONCE);// Beep(BEEP_ONCE);
       
          RELAY_ON(); //PTC_SetHigh();
		  PTC_heat_open_f = 1;//gpro_t.rx_ptc_flag = 1;//gctl_t.gDry=1;
		  ptc_prohibit_off_f =0; //WT.EDIT 2026.03-30

	
           SendWifiData_To_Cmd(0x02,0x01);
		   delay_ms(100);//HAL_Delay(5);
		   MqttData_Publish_SetPtc(0x01);
		   delay_ms(200);
         
		
         
         }

		
         wifi_t.response_wifi_signal_label=0xfe;
	  	
         
	   break;

	  case PTC_OFF_ITEM:
	  	if(discharge_f == 1){
          Beep(BEEP_ONCE);// Beep(BEEP_ONCE);
		  RELAY_OFF();  //PTC_SetLow();
		  ptc_prohibit_off_f=1;//WT.EDIT 2026.03-30
     	  PTC_heat_open_f =0; //gpro_t.rx_ptc_flag = 0;//gctl_t.gDry=0;
         //gctl_t.app_timer_power_on_flag = 0;
	
        
		
		 
	     SendWifiData_To_Cmd(0x02,0x0);
         delay_ms(100);//HAL_Delay(5);

		 MqttData_Publish_SetPtc(0);

		 delay_ms(200);
         }
	
	     wifi_t.response_wifi_signal_label = 0xfe;

	 
	  	break;

	  case ANION_OFF_ITEM: //"�?�?" //5
	  	if(discharge_f == 1){
			  Beep(BEEP_ONCE);// Beep(BEEP_ONCE);
			
            plasma_open_f = 0; //gctl_t.gPlasma=0;
			//gctl_t.gTimer_senddata_panel=8;
			SendWifiData_To_Cmd(0x03,0x0);
	  	   delay_ms(100);//HAL_Delay(5);
	  	    MqttData_Publish_SetPlasma(0);
			 delay_ms(200);
	  	}
      
	
	   wifi_t.response_wifi_signal_label = 0xfe;

	   break;
		
	  case ANION_ON_ITEM: //plasma 
	  	if(discharge_f == 1){
			  Beep(BEEP_ONCE);
         
             plasma_open_f = 1; //gctl_t.gPlasma=1;
            //  gctl_t.gTimer_senddata_panel=8;
			
			SendWifiData_To_Cmd(0x03,0x01);
	  	   delay_ms(100);//HAL_Delay(5);
	  	    MqttData_Publish_SetPlasma(1);
		     delay_ms(200);//delay_ms(100);//HAL_Delay(350);
	  	}
  

	   wifi_t.response_wifi_signal_label=0xfe;
	    break;

	  case SONIC_OFF_ITEM://ultransonic off
        if(discharge_f == 1){
			   Beep(BEEP_ONCE);

              
              Ultra_Sound_open_f = 0;//gctl_t.gUlransonic=0;
             //gctl_t.gTimer_senddata_panel=8; 
	
			SendWifiData_To_Cmd(0x04,0x0);
			delay_ms(100);//HAL_Delay(5);
			 MqttData_Publish_SetUltrasonic(0);
			delay_ms(200);//delay_ms(100);	//HAL_Delay(350);
			
        }
	
	   wifi_t.response_wifi_signal_label=0xfe;

	  	break;

	  case SONIC_ON_ITEM://ultransonic off
	    if(discharge_f == 1){
		      Beep(BEEP_ONCE);
           
              Ultra_Sound_open_f = 1;//gctl_t.gUlransonic=1;
              //gctl_t.gTimer_senddata_panel=8;
        
		
			SendWifiData_To_Cmd(0x04,0x01);
			delay_ms(100);//HAL_Delay(5);
			 MqttData_Publish_SetUltrasonic(1);
			delay_ms(200);//delay_ms(100);	//HAL_Delay(350);
        }
        

	   wifi_t.response_wifi_signal_label=0xfe;
	  	break;

	  case STATE_TIMER_MODEL_ITEM: //timer timing mode 0x0A
	  if(discharge_f == 1){

             Beep(BEEP_ONCE);
            AI_timing_open_f = 0;//gctl_t.gModel=2;
       
             
    	    SendWifiData_To_Cmd(0x27,0x02);
		   delay_ms(100);
		    MqttData_Publish_AitState(2);
    	     delay_ms(200);//delay_ms(100);//HAL_Delay(350);

           
        }
    

	   wifi_t.response_wifi_signal_label = 0xfe;
	
	  break;
		
	  case STATE_AI_MODEL_ITEM: // beijing timing 0x09
	  	 if(discharge_f == 1){
		       Beep(BEEP_ONCE);
              AI_timing_open_f =1;//gctl_t.gModel=1;
             
            
    		   SendWifiData_To_Cmd(0x27,0x01);
               delay_ms(100);
			    MqttData_Publish_AitState(1);
    		  delay_ms(200);//delay_ms(100);//HAL_Delay(350);
            
        }
     

	    wifi_t.response_wifi_signal_label = 0xfe;
		
	  	break;

	  case TEMPERATURE_ITEM:
	   if(discharge_f == 1){
		  Beep(BEEP_ONCE);

            temp_decade=wifi_t.rx_data_array[14]-0x30;
            temp_unit=wifi_t.rx_data_array[15]-0x30;
            setting_temperature = temp_decade*10 +  temp_unit;
            if( setting_temperature > 40) setting_temperature=40;
            if( setting_temperature <20 )  setting_temperature =20;
          
		    //gpro_t.set_temp_value_success = 1;//WT.EDIT 2026-03-30
			ptc_prohibit_off_f =0;

			
		
			//SendWifiData_To_Data(0x2A, setting_temperature); //smart phone set temperature value .
			delay_ms(100);//delay_ms(10);//HAL_Delay(10);
			
	

			 MqttData_Publis_SetTemp(setting_temperature);
             delay_ms(200);
       }
     

	  wifi_t.response_wifi_signal_label = 0xfe;

	  break;

	  case FAN_ITEM:
	    if(discharge_f == 1){
			

		     if(no_fan_load_f ==0){//if(gctl_t.fan_warning ==0){
                  Beep(BEEP_ONCE);
           		 wind_hundred =wifi_t.rx_data_array[7]-0x30;
	       		 wind_decade=wifi_t.rx_data_array[8]-0x30;
                 wind_unit = wifi_t.rx_data_array[9]-0x30;
            
                if(wind_hundred ==1 && wind_decade==0 && wind_unit==0)fan_speed_level=100;
                else
                     fan_speed_level = wind_hundred*10 + wind_decade;
			
         
		
			//delay_ms(200);//delay_ms(100);//HAL_Delay(350);
    	//	SendWifiData_To_PanelWindSpeed(fan_speed_level);
			delay_ms(100);//HAL_Delay(10);
			MqttData_Publis_SetFan(fan_speed_level);
			delay_ms(200);
          
		    }
			else{
			     Beep(BEEP_ONCE);
				fan_speed_level=0;

			    MqttData_Publis_SetFan(fan_speed_level);
				delay_ms(200);//delay_ms(100);//HAL_Delay(350);


			}
            
		}
     
	
	    wifi_t.response_wifi_signal_label = 0xfe;
	
	  	break;

	  case APP_TIMER_POWER_ON_REF :

	    if(wifi_connected_f==1){  //WT.EDIT 2025.03.27
		  // wifi_t.get_rx_beijing_time_enable=0; //enable beijing times
	  	 
		     if(strstr((char *)TCMQTTRCVPUB,"open\":1")){
		   
			  //gctl_t.app_timer_power_on_flag = 1;
              //gpro_t.power_off_run_step=1; // app power on 
              
		 
			   //gpro_t.gpower_on = power_on;
			   ///gpro_t.process_run_step=0;
			   SendWifiData_To_Cmd(0x21,0x01); //smart phone is open that App timer 
			   delay_ms(100);//HAL_Delay(10);
			    Beep(BEEP_ONCE);
               MqttData_Publish_SetOpen(1);  
			   delay_ms(200);//delay_ms(100);//HAL_Delay(350);
    
		    }
		    else if(strstr((char *)TCMQTTRCVPUB,"open\":0")){
		   
		   // gctl_t.app_timer_power_on_flag = 0;
           // gpro_t.gpower_on = power_off;
           /// gpro_t.power_off_run_step=1; //WT.EDIT 2025.01.04
           
            //gpro_t.send_ack_cmd = 1; //ack_app_power_off;

	
            SendWifiData_To_Cmd(0x20,0x0); //smart phone is power off
			delay_ms(100);//HAL_Delay(5);
			  Beep(BEEP_ONCE);
			  
            MqttData_Publish_SetOpen(0);  
			delay_ms(200);//delay_ms(100);
			
	        //phone_power_flag=2;
         
            wifi_t.response_wifi_signal_label = 0xfe;
             
       
		    }

	         wifi_t.response_wifi_signal_label=0xfe;
	       	}

	  

	  break;


   }


   if(wifi_t.response_wifi_signal_label==0xfe){
        
    
		
        memset(wifi_t.rx_data_array,0,20);
      
		wifi_t.response_wifi_signal_label=0xf0;
	}

  
   #endif 
 }



void Parse_Json_Statement(void)
{

   
    
    if(strstr((char *)TCMQTTRCVPUB,"sonic\":0")){
			
			  Ultra_Sound_open_f = 0;//gctl_t.gUlransonic=0;
				
	}
    else if(strstr((char *)TCMQTTRCVPUB,"sonic\":1")){
			
	   Ultra_Sound_open_f = 1;//gctl_t.gUlransonic=1;
				
   }
        
           
      if(strstr((char *)TCMQTTRCVPUB,"Anion\":0")){
			
				plasma_open_f =0;   //gctl_t.gPlasma=0;
				
				
			 
		}
		else if(strstr((char *)TCMQTTRCVPUB,"Anion\":1")){
			
				plasma_open_f =0;//gctl_t.gPlasma=1;
				
			
				
		}


       if(strstr((char *)TCMQTTRCVPUB,"ptc\":0")){
				
		   PTC_heat_open_f = 0; //gpro_t.rx_ptc_flag = 0;//gctl_t.gDry=0;
	 
           
				  
		}
		else if(strstr((char *)TCMQTTRCVPUB,"ptc\":1")){
				
			 PTC_heat_open_f = 1;	   // gpro_t.rx_ptc_flag = 1;//gctl_t.gDry=1;
		           
                 
				  
					
		}
		

    //  memset(TCMQTTRCVPUB,'\0',40);

  
   // }
   


}


