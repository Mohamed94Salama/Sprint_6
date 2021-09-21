/*****************************************************************************
* Task: AVR_DRIVERS
* File Name: Register.h
* Description: File for AVR Register definitions
* Author: Amr Mohamed
* Date: 8/7/2021
******************************************************************************/

#ifndef __REGISTER__
#define __REGISTER__

#define F_CPU														(8000000ul)

/* DIO_Registers */
#define DDRA_R  (*(volatile unsigned char*)0x3A)
#define DDRB_R  (*(volatile unsigned char*)0x37)
#define DDRC_R  (*(volatile unsigned char*)0x34)
#define DDRD_R  (*(volatile unsigned char*)0x31)

#define PINA_R  (*(volatile unsigned char*)0x39)
#define PINB_R  (*(volatile unsigned char*)0x36)
#define PINC_R  (*(volatile unsigned char*)0x33)
#define PIND_R  (*(volatile unsigned char*)0x30)

#define PORTA_R (*(volatile unsigned char*)0x3B)
#define PORTB_R (*(volatile unsigned char*)0x38)
#define PORTC_R (*(volatile unsigned char*)0x35)
#define PORTD_R (*(volatile unsigned char*)0x32)
/************************************************************************************************/
/* ADC_Registers */
#define ADMUX_R   (*(volatile unsigned char*)0x27)
#define ADLAR_B   5
#define REFS0_B   6
#define REFS1_B   7

#define ADCSRA_R  (*(volatile unsigned char*)0x26)
#define ADPS0_B   0
#define ADPS1_B   1
#define ADPS2_B   2
#define ADIE_B    3
#define ADIF_B    4
#define ADATE_B   5
#define ADSC_B    6
#define ADEN_B    7

#define ADCL_R    (*(volatile unsigned char*)0x24)
#define ADCH_R    (*(volatile unsigned char*)0x25)
#define ADC_R     (*(volatile unsigned short*)0x24)      //to get the the full 10 bit ADC value in one command


/************************************************************************************************/

/*******************************************************************************
 *                           INTERRUPT Registers                               *
********************************************************************************/
#define GICR_R    (*(volatile unsigned char*) 0x5B)
#define SREG_R    (*(volatile unsigned char*) 0x5F)
#define SREG_I_B    7u
#define INT0_B    6u
#define INT1_B    7u
#define INT2_B    5u
#define MCUCR_R   (*(volatile unsigned char*) 0x55)
#define MCUCSR_R   (*(volatile unsigned char*)0x54)
#define ISC2_B 6
#define GIFR_R    (*(volatile unsigned char*)0x5A)
#define INTF1_B 7u
#define INTF0_B 6u
#define INTF2_B 5u
/************************************************************************************************/
/* Timer 0 */
#define TCNT0_R   (*(volatile unsigned char*)0x52)
#define TCCR0_R   (*(volatile unsigned char*)0x53)
/* TCCR0 */
#define FOC0_B    7
#define WGM00_B   6
#define COM01_B   5
#define COM00_B   4
#define WGM01_B   3
#define CS02_B    2
#define CS01_B    1
#define CS00_B    0

#define TWCR_R    (*(volatile unsigned char*)0x56)
#define SPMCR_R   (*(volatile unsigned char*)0x57)
#define TIFR_R    (*(volatile unsigned char*)0x58)
/* TIFR */
#define TOV0_B    0
#define OCF0_B    1
#define TOV1_B    2
#define OCF1B_B   3
#define OCF1A_B   4
#define ICF1_B    5
#define TOV2_B    6
#define OCF2_B    7

#define TIMSK_R   (*(volatile unsigned char*)0x59)
/* TIMSK */
#define OCIE2_B   7
#define TOIE2_B   6
#define TICIE1_B  5
#define OCIE1A_B  4
#define OCIE1B_B  3
#define TOIE1_B   2
#define OCIE0_B   1
#define TOIE0_B   0

#define OCR0_R    (*(volatile unsigned char*)0x5C)
/****************************************************************************************************/

/*TIMER 1*/


#define ICR1_R			(*(volatile unsigned short*)0x46)
#define ICR1L_R		(*(volatile unsigned char*)0x46)
#define ICR1H_R		(*(volatile unsigned char*)0x47)
#define OCR1B_R		(*(volatile unsigned short*)0x48)
#define OCR1BL_R		(*(volatile unsigned char*)0x48)
#define OCR1BH_R		(*(volatile unsigned char*)0x49)
#define OCR1A_R		(*(volatile unsigned short*)0x4A)
#define OCR1AL_R		(*(volatile unsigned char*)0x4A)
#define OCR1AH_R		(*(volatile unsigned char*)0x4B)
#define TCNT1_R		(*(volatile unsigned short*)0x4C)
#define TCNT1L_R		(*(volatile unsigned char*)0x4C)
#define TCNT1H_R		(*(volatile unsigned char*)0x4D)
#define TCCR1B_R     (*(volatile unsigned char*)0x4E)
#define TCCR1A_R     (*(volatile unsigned char*)0x4F)
#define SFIOR_R		(*(volatile unsigned char*)0x50)
#define OSCCAL_R		(*(volatile unsigned char*)0x51)
/*************************************************************************************************/
/* Timer 2 */

#define OCR2_R       (*(volatile unsigned char*)0x43)
#define TCNT2_R      (*(volatile unsigned char*)0x44)
#define TCCR2_R      (*(volatile unsigned char*)0x45)




/* TCCR2 */
#define FOC2_B    7
#define WGM20_B   6
#define COM21_B   5
#define COM20_B   4
#define WGM21_B   3
#define CS22_B    2
#define CS21_B    1
#define CS20_B    0

/* ASSR */
/* bits 7-4 reserved */
#define AS2_B     3
#define TCN2UB_B  2
#define OCR2UB_B  1
#define TCR2UB_B  0

/* TCCR1A */
#define COM1A1_B  7
#define COM1A0_B  6
#define COM1B1_B  5
#define COM1B0_B  4
#define FOC1A_B   3
#define FOC1B_B   2
#define WGM11_B   1
#define WGM10_B   0

/* TCCR1B */
#define ICNC1_B   7
#define ICES1_B   6
/* bit 5 reserved */
#define WGM13_B   4
#define WGM12_B   3
#define CS12_B    2
#define CS11_B    1
#define CS10_B    0


/*************************************************************************************************/

//USART REGISTERS

/* USART Baud Rate Register Low */
#define UBRRL_R    (*(volatile unsigned char*)0x29)

/* USART Control and Status Register B */
#define UCSRB_R   (*(volatile unsigned char*)0x2A)

#define    RXCIE_B        7
#define    TXCIE_B        6
#define    UDRIE_B        5
#define    RXEN_B         4
#define    TXEN_B         3
#define    UCSZ_B         2
#define    UCSZ2_B        2
#define    RXB8_B         1
#define    TXB8_B         0
/* USART Control and Status Register A */
#define UCSRA_R    (*(volatile unsigned char*)0x2B)

#define    RXC_B          7
#define    TXC_B          6
#define    UDRE_B         5
#define    FE_B           4
#define    DOR_B          3
#define    UPE_B          2
#define    U2X_B          1
#define    MPCM_B         0
/* USART I/O Data Register */
#define UDR_R       (*(volatile unsigned char*)0x2C)

/* USART Baud Rate Register High */
#define UBRRH_R    (*(volatile unsigned char*)0x40)

/* USART Control and Status Register C */
#define UCSRC_R    (*(volatile unsigned char*)0x40)

/* USART Register C */
#define    URSEL_B        7
#define    UMSEL_B        6
#define    UPM1_B         5
#define    UPM0_B         4
#define    USBS_B         3
#define    UCSZ1_B        2
#define    UCSZ0_B        1
#define    UCPOL_B        0


/********************************************************************************************************/

/*SPI REGISTERS*/


/* SPI I/O Data Register */
#define SPDR_R    (*(volatile unsigned char*)0x2F)

/* SPI Status Register */
#define SPSR_R    (*(volatile unsigned char*)0x2E)
#define SPIF_B        7
#define WCOL_B        6
/*Pins 1 ~ 5 are reserved */
#define SPI2X_B       0


/* SPI Control Register */
#define SPCR_R    (*(volatile unsigned char*)0x2D)
#define SPIE_B        7
#define SPE_B         6
#define DORD_B        5
#define MSTR_B        4
#define CPOL_B        3
#define CPHA_B        2
#define SPR1_B        1
#define SPR0_B        0


/*********************************************************************************************************/

/*  I2C / TWI Registers   */

#define TWBR_R    (*(volatile unsigned char*)0x20)


#define TWSR_R    (*(volatile unsigned char*)0x21)
/* TWSR */
#define TWS7_B    7
#define TWS6_B    6
#define TWS5_B    5
#define TWS4_B    4
#define TWS3_B    3
/* bit 2 reserved */
#define TWPS1_B   1
#define TWPS0_B   0

#define TWAR_R    (*(volatile unsigned char*)0x22)
/* TWAR */
#define TWA6_B    7
#define TWA5_B    6
#define TWA4_B    5
#define TWA3_B    4
#define TWA2_B    3
#define TWA1_B    2
#define TWA0_B    1
#define TWGCE_B   0

#define TWDR_R    (*(volatile unsigned char*)0x23)

#define TWCR_R    (*(volatile unsigned char*)0x56)
/* TWCR */
#define TWINT_B   7
#define TWEA_B    6
#define TWSTA_B   5
#define TWSTO_B   4
#define TWWC_B    3
#define TWEN_B    2
/* bit 1 reserved */
#define TWIE_B    0

/*********************************************************************************************************/

/* Interrupt vectors */
/* External Interrupt Request 0 */
#define INT0_vect             __vector_1
/* External Interrupt Request 1 */
#define INT1_vect		         __vector_2
/* External Interrupt Request 2 */
#define INT2_vect		         __vector_3
/* Timer/Counter2 Compare Match */
#define TIMER2_COMP_vect		__vector_4
/* Timer/Counter2 Overflow */
#define TIMER2_OVF_vect			__vector_5
/* Timer/Counter1 Capture Event */
#define TIMER1_ICU_vect		   __vector_6
/* Timer/Counter1 Compare Match A */
#define TIMER1_OCA_vect		   __vector_7
/* Timer/Counter1 Compare Match B */
#define TIMER1_OCB_vect		   __vector_8
/* Timer/Counter1 Overflow */
#define TIMER1_OVF_vect			__vector_9
/* Timer/Counter0 Compare Match */
#define TIMER0_OC_vect		   __vector_10
/* Timer/Counter0 Overflow */
#define TIMER0_OVF_vect			__vector_11
/* Serial Transfer Complete */
#define SPI_STC_vect			   __vector_12
/* USART, Rx Complete */
#define UART_RX_vect			   __vector_13
/* USART Data Register Empty */
#define UART_UDRE_vect			__vector_14
/* USART, Tx Complete */
#define UART_TX_vect			   __vector_15
/* ADC Conversion Complete */
#define ADC_vect			      __vector_16
/* EEPROM Ready */
#define EE_RDY_vect			   __vector_17
/* Analog Comparator */
#define ANA_COMP_vect			__vector_18
/* 2-wire Serial Interface */
#define TWI_vect			      __vector_19
/* Store Program Memory Ready */
#define SPM_RDY_vect			   __vector_20

#  define BAD_vect            __vector_default

/*interrupt functions*/

# define sei()  __asm__ __volatile__ ("sei" ::)
# define cli()  __asm__ __volatile__ ("cli" ::)
# define reti()  __asm__ __volatile__ ("reti" ::)
# define ret()  __asm__ __volatile__ ("ret" ::)

#  define ISR_NOBLOCK    __attribute__((interrupt))
#  define ISR_NAKED      __attribute__((naked))



#  define ISR(vector,...)            \
void vector (void) __attribute__ ((signal))__VA_ARGS__ ; \
void vector (void)


#endif /* __REGISTER__ */
