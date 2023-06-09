/* Includes */
#include "stm32f4xx.h"


int main(void){


/*INICIALIZANDO UM REGISTRADOR*/
RCC->AHB1ENR=0X00000087;


/*LIGANDO O TIMER TIM11*/
RCC->APB2ENR|=0X40000;
//RCC->APB2ENR|=1<<18;

/*TIMER DE 10ms DE ATRASO*/

//DEFININDO O PRESCALER
TIM11->PSC=9;

//DEFININDO O AUTORELOAD REGISTER
TIM11->ARR=15999;

// LIGANDO O TIMER 11
TIM11->CR1|=0X01;

// REGISTRADOR DA PORTA C PARA SAIDAS OUT //
GPIOC->MODER=0X01; //PC0

/* Infinite loop:*/

	while (1){
		int soltou = 0;

		if((GPIOC->IDR&=0X02) && soltou == 0){//BOT�O APERTADO
			soltou = 1;
			TIM11->CNT = 0;

			if(TIM11->SR &= 0X01){// SE HOUVE ESTOURO NO TIMER {}
				GPIOC->ODR ^= 0X01; // SAIDA PARA NIVEL L�GICO 1

				TIM11->SR &= ~(0x01); //limpando  flag do timer11
				}

			}

		if((GPIOC->IDR&=0x02)==0){// SE O BOT�O N�O TIVER MAIS APERTADO
				  soltou=0;
			}

	}
}
