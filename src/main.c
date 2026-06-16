#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/adc.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>

#define STACK_SIZE 1024
#define PRIORITY 5 //IGUAL INICIALMENTE

volatile int saldo_vitrine = 0;

k_mutex_define(vitrine_mutex);

void Padeiro(void *arg1, void *arg2, void *arg3)
{
    while(1) {
        //Padeiro produz um pão a cada 1 segundo
        k_sleep(K_SECONDS(1));

        k_mutex_lock(&vitrine_mutex, K_FOREVER); // Trava o mutex para acessar a vitrine

        saldo_vitrine++; // Adiciona um pão à vitrine
        printk("Padeiro produziu um pão. Saldo atual na vitrine: % d\n", saldo_vitrine);

        k_mutex_unlock(&vitrine_mutex); // Destrava o mutex para permitir que o cliente compre    
    }
}

void Cliente(void *arg1, void *arg2, void *arg3)
{
    while(1) {
        //Cliente compra um pão a cada 1.5 segundo
        k_sleep(K_MSEC(1500)); 

        k_mutex_lock(&vitrine_mutex, K_FOREVER); // Trava o mutex para acessar a vitrine

        saldo_vitrine--; // Cliente compra um pão da vitrine
        printk("Cliente comprou um pão. Saldo atual na vitrine: % d\n", saldo_vitrine);
    
        k_mutex_unlock(&vitrine_mutex); // Destrava o mutex para permitir que o padeiro produza mais pães
    }
}

K_THREAD_DEFINE(padeiro_tid, STACK_SIZE, Padeiro, NULL, NULL, NULL,
                PRIORITY, 0, 0);
K_THREAD_DEFINE(cliente_tid, STACK_SIZE, Cliente, NULL, NULL, NULL,
                PRIORITY, 0, 0);


void main (void){
    
    printk("Padaria Aberta (Com sincronização via MUTEX).\n");

    while(1) {
        k_sleep(K_MSEC(1000));
    }

}



