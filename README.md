# SEyTR · Ciclo de trabajo: compilar, grabar y monitorizar

Código de la demostración en vivo de **Sistemas Empotrados y de Tiempo Real**
(Máster Universitario en Robótica y Automática, UCLM). Se usa en la sesión
**U1-S3, «Entorno de desarrollo profesional»**.

El repositorio es público: no hace falta cuenta ni permisos para clonarlo. La
demostración de la sesión empieza precisamente clonándolo.

## Qué hay dentro

Dos proyectos ESP-IDF hermanos. **La carpeta que se abre en el editor es la de
uno de ellos, no la raíz**: en la raíz no hay proyecto.

| Carpeta | Qué es |
| --- | --- |
| `01_ciclo_completo/` | El programa que funciona. Arranca, avisa de que está listo y publica una medida por segundo. |
| `02_fallo_provocado/` | El mismo programa con un fallo preparado: al llegar a la tercera medida rompe. |

Los dos proyectos son **el mismo programa**. Sus ficheros `main/ciclo.c` se
diferencian en una sola línea:

```c
/* 01_ciclo_completo */   #define MEDIDAS_ANTES_DEL_FALLO 0
/* 02_fallo_provocado */  #define MEDIDAS_ANTES_DEL_FALLO 3
```

Cuando el contador llega a ese número, la fuente de lectura deja de ser válida y
el acceso siguiente provoca un acceso a memoria prohibido. Es el fallo que en la
sesión se lee en el volcado. Como entre medida y medida pasa un segundo, el
volcado aparece unos dos segundos después del arranque.

## Qué hace el programa

- Al arrancar imprime dos trazas seguidas, una de información y otra de aviso,
  para que se vean en el monitor dos niveles distintos con la misma etiqueta.
- Después publica una medida por segundo, con su número y su valor.
- No usa ningún periférico. Los periféricos son materia de la unidad 2.

Tres detalles del código que están puestos a propósito y conviene no «limpiar»:

- La fuente de lectura es un **puntero volátil**. Así el compilador no puede dar
  por supuesto a dónde apunta, y el acceso ocurre de verdad en tiempo de
  ejecución en lugar de desaparecer al optimizar.
- `leer_sensor()` y `publicar_medida()` se marcan **sin expansión en línea**.
  Así la pila de llamadas del volcado muestra los tres niveles —`leer_sensor`,
  `publicar_medida` y `app_main`— y se puede leer de dentro hacia fuera, que es
  justo lo que se explica en la sesión.
- Los textos que se imprimen van **sin tildes**, para que se lean igual en
  cualquier consola durante la clase. Los comentarios del código sí las llevan.

## Entorno

El chip es el **ESP32-S3**. El SDK, el editor, la extensión y el sistema
operativo son los que fija el documento «Entorno de desarrollo» publicado en el
Campus Virtual.

El destino de compilación queda fijado en el fichero `sdkconfig.defaults` de
cada proyecto. Si al abrir una copia recién clonada la extensión no muestra
`esp32s3` como destino, selecciónalo con su selector de destino —o ejecuta
`idf.py set-target esp32s3`— antes de la primera compilación.

El proyecto del fallo lleva además una opción propia en su `sdkconfig.defaults`:
el manejador de fallos **imprime el volcado y se detiene**, en lugar de
reiniciar. Sin ella, el chip se reinicia solo, vuelve a fallar y el volcado se
va de la pantalla antes de poder comentarlo.

Lo que se versiona es `sdkconfig.defaults`. El fichero `sdkconfig` y la carpeta
`build/` se generan y quedan fuera del control de versiones.

