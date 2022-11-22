# Informe

[Resumen de todos los cambios hechos](./changes_made.md)  

### Cuando se ejecuta el main con la opción -d, ¿qué se está mostrando en la pantalla?

se esta mostrando la ejecuccion del comando en modo debug, son mensajes de desarrollo de todas las funciones de fuse.

#### como?

en fat_fuce.c detecta si estamos corriendolo en modo debug
de serlo así, agrega -d a los argumentos de fuse_argv

```c
if (debug) {
        fuse_argv[fuse_argc] = "-d"; // Debug mode
        fuse_argc++;
    }
```

### ¿Hay alguna manera de saber el nombre del archivo guardado en el cluster 157?

Podemos recorrer las dir entries a partir del root, de manera recursiva en directorios, hasta encontrar una entry cuyo start_cluster(o uno de los clusters en la cadena) sea el 157.
De esta dir_entry sacamos el nombre `dir_entry->basename`

### ¿Dónde se guardan las entradas de directorio? ¿Cuántos archivos puede tener adentro un directorio en FAT32?

Las entradas de directorio se guardan dentro del contenido de un archivo llamado directorio, la cual es una tabla de entradas.

Un directorio de FAT32 puede tener 512/32=16 por cluster.
En esta implementacion de FAT32 solo 16.

### Cuando se ejecuta el comando como ls -l, el sistema operativo, ¿llama a algún programa de usuario? ¿A alguna llamada al sistema? ¿Cómo se conecta esto con FUSE? ¿Qué funciones de su código se ejecutan finalmente?  

ls -l es un programa de usuario que va a llamar a readdir que finalmente llamara a nuestro codigo en fat_operations_readdir y luego tambien llamara a stat para conseguir informacion de los archivos. 

Por ende, si llama a un programa de usuario, tambien a llamadas al sistema, FUSE dentro del kernel ejecuta nuestro codigo, y nuestro codigo es un proceso en background en espacio de usuario.

### ¿Por qué tienen que escribir las entradas de directorio manualmente pero no tienen que guardar la tabla FAT cada vez que la modifican?

Porque la tabla FAT esta mapeada en RAM y solo nosotros la leemos.
Nosotros leemos la FAT desde la RAM pero de igual manera la copiamos manualmente al disco en cada modificacion.

Dentro de map_fat donde mapea la primera tabla FAT, llama a mmap tal que

```c
ptr = mmap(NULL, fat_aligned_size_bytes, prot, MAP_PRIVATE, fd,
               fat_aligned_offset);
```

Donde podemos ver que al pasar el argumento `MAP_PRIVATE` estamos señalizando que la escritura de las modificaciones en disco sera manualmente.

Lo cual lo hacemos en fat_table_set_next_cluster

```c
pwrite(table->fd, &next_cluster_le32, sizeof(le32), entry_offset);
```

### Para los sistemas de archivos FAT32, la tabla FAT, ¿siempre tiene el mismo tamaño? En caso de que sí, ¿qué tamaño tiene?

En nuestra implementación de FAT32 depnede de la cantidad de clusters, que depende de el espacio del volumen, para una cantidad mayor de clusters la tabla sera mayor.

El tamaño terminara siendo de el volumen/tamaño del cluster.

