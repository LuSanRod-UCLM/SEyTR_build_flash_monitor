/*
 * SEyTR · U1-S3 · Entorno de desarrollo profesional
 * Demostración del ciclo de trabajo: configurar, compilar, grabar, monitorizar.
 *
 * El programa arranca, avisa de que está listo y publica una medida por
 * segundo. Sirve para ver en el monitor el nivel, la etiqueta y la marca de
 * tiempo de cada traza de registro.
 *
 * Este fichero es idéntico en los dos proyectos del repositorio salvo por el
 * valor de MEDIDAS_ANTES_DEL_FALLO. No usa ningún periférico: los periféricos
 * son materia de la unidad U2.
 *
 * Los textos que se imprimen van sin tildes a propósito, para que se lean
 * igual en cualquier consola durante la clase. Los comentarios sí las llevan.
 */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

/* ÚNICA DIFERENCIA entre los dos proyectos del repositorio:
 * en 01_ciclo_completo vale 0, que significa «no falla nunca»;
 * en 02_fallo_provocado vale 3, y entonces la medida número 3 rompe el programa. */
#define MEDIDAS_ANTES_DEL_FALLO 0

static const char *TAG = "ciclo";

/* La fuente de lectura se declara como puntero volátil a propósito: así el
 * compilador no puede dar por supuesto a dónde apunta, y el acceso de
 * leer_sensor() se hace de verdad en tiempo de ejecución. Es lo que permite
 * que el proyecto gemelo provoque el fallo cambiando solo una línea. */
static int lectura;
static int *volatile fuente_de_lectura = &lectura;

/* Las dos funciones se marcan sin expansión en línea para que la pila de
 * llamadas del volcado muestre los tres niveles y se pueda leer de dentro
 * hacia fuera: leer_sensor, publicar_medida y app_main. */
static int __attribute__((noinline)) leer_sensor(void)
{
    return *fuente_de_lectura;
}

static void __attribute__((noinline)) publicar_medida(int numero)
{
    int valor = leer_sensor();
    ESP_LOGI(TAG, "medida %d: valor = %d", numero, valor);
}

void app_main(void)
{
    ESP_LOGI(TAG, "Arranque completado. Empieza el ciclo de medidas.");
    ESP_LOGW(TAG, "Aviso de ejemplo: asi se ve una traza de otro nivel.");

    for (int numero = 1; ; numero++) {
        if (numero == MEDIDAS_ANTES_DEL_FALLO) {
            ESP_LOGW(TAG, "La fuente de lectura deja de ser valida.");
            fuente_de_lectura = NULL;
        }

        lectura = numero * 10;
        publicar_medida(numero);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
