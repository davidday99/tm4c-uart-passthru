#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "driverlib/sysctl.h"
#include "driverlib/hw_memmap.h"
#include "driverlib/uart.h"
#include "driverlib/gpio.h"

#define PART_TM4C123GH6PM
#include "driverlib/pin_map.h"

int uart_init(void) {
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);

    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinConfigure(GPIO_PC4_U1RX);
    GPIOPinConfigure(GPIO_PC5_U1TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5);

    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200, 
            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
    UARTConfigSetExpClk(UART1_BASE, SysCtlClockGet(), 115200, 
            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
    return 1;
}

int uart_getData(uint32_t uart, char *in, size_t count) {
    while (count--) {
        *in++ = UARTCharGet(uart);
    }
    return 1;
}

int uart_putData(uint32_t uart, const char *out, size_t count) {
    while (count--) {
        UARTCharPut(uart, *out++);
    }
    return 1;
}

int uart_dataWaiting(uint32_t uart) {
    return UARTCharsAvail(uart);
}

int uart_flushRxFifo(uint32_t uart) {
    while (UARTCharsAvail(uart)) {
        UARTCharGet(uart);
    }
    return 1;
}

char rx_buf[256];
uint8_t rx_buf_count = 0;

char tx_buf[256];
uint8_t tx_buf_count = 0;

int main(void){
    int count;
    uart_init();
    while (1) {
        count = uart_dataWaiting(UART0_BASE);
        uart_getData(UART0_BASE, rx_buf, count);
        for (int i = 0; i < count; i++) {
            uart_putData(UART1_BASE, rx_buf, count);
        }
        count = uart_dataWaiting(UART1_BASE);
        uart_getData(UART1_BASE, rx_buf, count);
        for (int i = 0; i < count; i++) {
            uart_putData(UART0_BASE, rx_buf, count);
        }
    }
}

