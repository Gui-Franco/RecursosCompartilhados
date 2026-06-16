#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/adc.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>

#define STACK_SIZE 1024
#define PRIORITY 5 //IGUAL INICIALMENTE

volatile int saldo_vitrine = 0;

K_SEM_DEFINE(vitrine_sem, 1, 1);

void Padeiro(void *arg1, void *arg2, void *arg3)
{
    while(1) {
        //Padeiro produz um pão a cada 1 segundo
        k_sleep(K_SECONDS(1));

        k_sem_take(&vitrine_sem, K_FOREVER); // Trava o semáforo para acessar a vitrine

        saldo_vitrine++; // Adiciona um pão à vitrine
        printk("Padeiro produziu um pão. Saldo atual na vitrine: % d\n", saldo_vitrine);

        k_sem_give(&vitrine_sem); // Libera o semáforo para permitir que o cliente compre um pão   
    }
}

void Cliente(void *arg1, void *arg2, void *arg3)
{
    while(1) {
        //Cliente compra um pão a cada 1.5 segundo
        k_sleep(K_MSEC(1500)); 

        k_sem_take(&vitrine_sem, K_FOREVER); // Trava o semáforo para acessar a vitrine

        saldo_vitrine--; // Cliente compra um pão da vitrine
        printk("Cliente comprou um pão. Saldo atual na vitrine: % d\n", saldo_vitrine);
    
        k_sem_give(&vitrine_sem); // Libera o semáforo para permitir que o padeiro produza mais pães
    }
}

K_THREAD_DEFINE(padeiro_tid, STACK_SIZE, Padeiro, NULL, NULL, NULL,
                PRIORITY, 0, 0);
K_THREAD_DEFINE(cliente_tid, STACK_SIZE, Cliente, NULL, NULL, NULL,
                PRIORITY, 0, 0);


void main (void){
    
    printk("Padaria Aberta (Com sincronização via Semaforo Binario).\n");

    while(1) {
        k_sleep(K_MSEC(1000));
    }

}



