/* Includes */
#include "stm32f4xx.h"


int display[] = {0x378, 0x60, 0x730, 0x570, 0x468, 0x558, 0x758, 0x70, 0x778, 0x578}; // display 7 seg
int numDisplay = 0; // o numero de display que usamos 1 -> 4
int segundos = 0,horasUni = 0, horasDec = 0, minutosUni = 0, minutosDec = 0;
int bit = 0;

void TIM1_UP_TIM10_IRQHandler(void){
	numDisplay++;
	if(numDisplay > 3) numDisplay = 0;



	GPIOC->ODR |= 0X0F;
	switch(numDisplay){
		case 0:
			GPIOC->ODR &=~0X01;
			GPIOB->ODR = display[minutosUni];
			break;

		case 1:
			GPIOC->ODR &=~0X02;
			GPIOB->ODR = display[minutosDec];
			break;

		case 2:
			GPIOC->ODR &=~0X04;
			GPIOB->ODR = display[horasUni];
			break;

		case 3:
			GPIOC->ODR &=~0X08;
			GPIOB->ODR = display[horasDec];
			break;
	}


	if(segundos > 59){
					segundos = 0;
					minutosUni++;
				}


			  if(bit != 1){
				  if(minutosUni > 9){
					  minutosUni = 0;
				  	  minutosDec++;
				  }
				   if(minutosDec > 5){
					   minutosDec = 0;
					   horasUni++;
				   }
			  }


			  if(horasUni > 9){
				  horasUni = 0;
				  horasDec++;
			  }

			  if(horasDec > 1 && horasUni > 3){ //zerando todo o relogio
				  horasDec = 0;
				  horasUni = 0;
				  minutosUni=0;
				  minutosDec =0;
			  }



    TIM10->SR &=~0x01;
}


void TIM1_TRG_COM_TIM11_IRQHandler(void){
	segundos++;
    TIM11->SR &=~0x01;
}



int main(void)
{
	RCC->AHB1ENR =0x0087;

	//configurar o timer 10
	RCC->APB2ENR |=1<<17;//habilita o clock do timer 10
	TIM10->PSC=99;
	TIM10->ARR=111;
	TIM10->CR1|=0x01;
	TIM10->DIER|=0x01;    //habilita interrupçao do timer 10

	NVIC_SetPriority(TIM1_UP_TIM10_IRQn, 4);
	NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);


	RCC->APB2ENR |=1<<18;//habilita o clock do timer 11
	TIM11->PSC=15999;
	TIM11->ARR=999;		//timer 1 minuto
	TIM11->CR1|=0x01;    //Ligando o timer11
	TIM11->DIER|=0x01;    //habilita interrupçao do timer 10

	NVIC_SetPriority(TIM1_TRG_COM_TIM11_IRQn, 5);//Priority 3
	NVIC_EnableIRQ(TIM1_TRG_COM_TIM11_IRQn);//

	GPIOB->MODER=0X151540; // 3, 4, 5, 6, 8, 9, 10 PARA O DISPLAY
	GPIOC->MODER=0X55;// PARA CONTROLAR A MULTIPLEXAÇÃO

	/*LIGANDO O TIMER TIM9*/
		RCC->APB2ENR|=0X10000;
		TIM9->PSC=1999;
		TIM9->ARR=1999;
		TIM9->CR1|=0X01;

	int chave1 = 0, chave2 = 0;


  while (1)
  {
	  bit = 0;
		//DEBOUNCE
		if((GPIOC->IDR &0X1000) == 0X1000 && chave1 == 0){//pc12

			TIM9->CNT = 0;
			TIM9->SR &= ~(0x01); //limpando  flag do timer11
			chave1 = 1;
			minutosUni++;
			if(minutosUni > 9 && minutosDec > 4){
				minutosDec = 0;
				minutosUni = 0;
				bit = 1;
			}


			while((TIM9->SR & 0X01) == 0x0){};
		}

		if((GPIOC->IDR &=0X1000) == 0X0) chave1 = 0;


		//DEBOUNCE2
		if((GPIOC->IDR &0X400) == 0X400 && chave2 == 0){//pc10

			TIM9->CNT = 0;
			TIM9->SR &= ~(0x01); //limpando  flag do timer11
			chave2 = 1;
			horasUni++;
			if(horasUni > 9 && horasDec > 4){
					horasUni = 0;
					horasDec = 0;
					bit = 1;
				}

			while((TIM9->SR & 0X01) == 0x0){}
		}

		if((GPIOC->IDR &=0X400) == 0X0) chave2 = 0;


  }
}

