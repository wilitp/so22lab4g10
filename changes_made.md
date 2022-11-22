# Cambios y modificaciones del proyecto

### fat_fuse.c
Aca agregamos dos llamadas `rmdir()` y `unlink()`, es decir `rmdir` y `rm`

### fat_fuse_ops.h
Aca agregamos un par de comentarios y prototipos de `rmdir()` y `unlink()` 

### fat_fuse_ops.c
Aca arreglamos el tema de la basura, agregamos la implementacion de `rmdir()` y `unlink()`, 2 mensajes de los logs (en read y write), tambien hicimos que `ls` no lea los archivos de big brother modificando la funcion `fat_fuse_readdir()`

### fat_file.c
Agregamos la funcion que inicializa un cluster `fat_file_init_dir_cluster()` y guille hizo una BANDA de fixs

### fat_file.h
Se agregaron prototipos de funciones y en el `struct fat_file_s`, una lista de las entryes libres
