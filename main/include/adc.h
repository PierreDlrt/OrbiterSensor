#ifndef ADC_H
#define ADC_H

/* Includes */

/* Defines */

/* Public function declarations */
void init_adc(void);
void read_adc(int *voltage_A, int *voltage_B);
void adc_stop(void);

#endif // ADC_H
