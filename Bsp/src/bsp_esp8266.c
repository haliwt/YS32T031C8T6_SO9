#include "bsp.h"


 uint8_t dc_connect_net_step	;
 uint8_t link_counter_times;
 /**
 *
 *@brief 
 *@param  
 *
 **/
 static void auto_connect_wifi_handler(void);


/**********************************************************************
	*
	*Functin Name: void wifi_auto_detected_link_state(void)
	*Function :
	*Input Ref:  NO
	*Return Ref: NO
	*
**********************************************************************/
void wifi_auto_detected_link_state(void)
{
	if(wifi_connected_success_f ==0){
		
       auto_connect_wifi_handler();//InitWifiModule();
	
	 
    }
	
    if(wifi_connected_success_f==1 && link_counter_times ==0 ){
              
           link_counter_times ++ ;
		   link_counter_times =10;
           wifi_linking_tencent_f =0;//wifi_t.linking_tencent_cloud_doing = 0;
           time_link_net_counter =0;
        
          if(discharge_f){
		     MqttData_Publish_PowerOff_Ref();
               delay_ms(200);//HAL_Delay(200);

          }
          
          Subscriber_Data_FromCloud_Handler();
          delay_ms(200);//HAL_Delay(200);
         

          SendWifiData_To_Cmd(0x1F,0x01); //link wifi order 1 --link wifi net is success.
          delay_ms(100);
		  time_link_net_counter =0;
   }
   
   if(time_link_net_counter > 4 && link_counter_times ==1){
	  time_link_net_counter =0;

      link_counter_times =5;
      if(wifi_connected_success_f==0){
         SendData_Set_Command(0x1F,0);//SendWifiData_To_Data(0x1F,0x0); //WT.EDIT 2025.04.02 0x1F: wifi link net is succes 
         delay_ms(100);

	  }
	  else{
	     SendData_Set_Command(0x1F,1);//SendWifiData_To_Data(0x1F,0x0); //WT.EDIT 2025.04.02 0x1F: wifi link net is succes 
		 delay_ms(100);


	  }

   }
   
   
}
/****************************************************************************************************
**
*Function Name:static void initBtleModule(void)
*Function: power on auto link net fun
*Input Ref: 
*Return Ref:NO
*
****************************************************************************************************/
static void auto_connect_wifi_handler(void)
{

  switch(dc_connect_net_step){

	case 0:

	    
	    wifi_rx_numbers = 0;
        time_link_net_counter=0;
        wifi_linking_tencent_f =1;//linking_tencent_cloud_doing = 1;
	    wifi_cofig_success_f=0;
		
	   send_usart2_data("AT+RST\r\n", strlen("AT+RST\r\n"));
	   delay_ms(1000);//HAL_Delay(1000);
	   time_link_net_counter=0;

	    dc_connect_net_step=1;

	break;

	case 1:
	 if(time_link_net_counter > 2 ){
	   time_link_net_counter=0; 
	    dc_connect_net_step=2;
	   
	  }
	break;

	case 2:

     if(time_link_net_counter >3){
	   
		 wifi_linking_tencent_f = 1;//net_t.linking_tencent_cloud_doing  = 1;
	     wifi_rx_numbers = 0; 
		 //wifi_t.soft_ap_config_flag =0;
		 dc_connect_net_step = 3;
     }

	break;

	case 3:
  
        time_link_net_counter=0;//gpro_t.gTimer_power_on_first_link_tencent=0;
      
	 // HAL_UART_Transmit(&huart2, "AT+TCMQTTCONN=1,5000,240,0,1\r\n", strlen("AT+TCMQTTCONN=1,5000,240,0,1\r\n"), 0xffff);//??
	       send_usart2_data("AT+TCMQTTCONN=1,5000,240,0,1\r\n", strlen("AT+TCMQTTCONN=1,5000,240,0,1\r\n"));
		   delay_ms(1000);//HAL_Delay(1000);
	       time_link_net_counter=0;
	        
	 dc_connect_net_step=4;
	  
     break;

	 case 4:
   
	   if(wifi_connected_success_f==1 && time_link_net_counter > 1){
				//wifi_t.linking_tencent_cloud_doing =0;

				time_link_net_counter=0;
				wifi_linking_tencent_f =0;
	            dc_connect_net_step =5;
	            SendWifiData_To_Cmd(0x1F,0x01); //link wifi order 1 --link wifi net is success.
	            delay_ms(100);
		}
	    else if(wifi_connected_success_f==0 && time_link_net_counter > 1){
			time_link_net_counter=0;
	       dc_connect_net_step=5;
	        SendWifiData_To_Cmd(0x1F,0x00);
		    delay_ms(100);
	    }

	break;

	default:

	break;

	}
}

