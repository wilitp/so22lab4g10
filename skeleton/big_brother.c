#include "big_brother.h"
#include "fat_table.h"
#include "fat_util.h"
#include "fat_volume.h"

#include <sys/mman.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int bb_is_log_file_dentry(fat_dir_entry dir_entry) {
    return strncmp(LOG_FILE_BASENAME, (char *)(dir_entry->base_name), 3) == 0 &&
           strncmp(LOG_FILE_EXTENSION, (char *)(dir_entry->extension), 3) == 0;
}

int bb_is_log_filepath(char *filepath) {
    return strncmp(BB_LOG_FILE, filepath, 8) == 0;
}

int bb_is_log_dirpath(char *filepath) {
    return strncmp(BB_DIRNAME, filepath, 15) == 0;
}

/* Searches for a cluster that could correspond to the bb directory and returns
 * its index. If the cluster is not found, returns 0.
 */
u32 search_bb_orphan_dir_cluster() {
    /* First two clusters are reserved */
    u32 bb_dir_start_cluster = 2;
    fat_volume vol = get_fat_volume();
    fat_table table = vol->table;

    bool any_free_until_now = false;
    u32 cluster_group_count = 1;
    for (;;bb_dir_start_cluster++) {

        u32 cluster_table_value = le32_to_cpu(((const le32 *)table->fat_map)[bb_dir_start_cluster]);
        if(cluster_table_value == FAT_CLUSTER_BAD_SECTOR) {
            u32 bytes_per_cluster = fat_table_bytes_per_cluster(table);
            off_t offset = fat_table_cluster_offset(table, bb_dir_start_cluster);
            u8 *buf = alloca(bytes_per_cluster);
            full_pread(table->fd, buf, bytes_per_cluster, offset);

            if(bb_is_log_file_dentry((fat_dir_entry) buf)) {
                break;
            }
        } else {
            any_free_until_now = any_free_until_now || cluster_table_value == FAT_CLUSTER_FREE;
        }

        if(bb_dir_start_cluster > 10000 * cluster_group_count) {
            if(any_free_until_now) {
                // Si no encontramos el cluster de logueo hasta ahora Y si habian lugares libres, 
                // asumimos que no lo vamos a encontrar en el resto del disco(porq en esos lugares libres lo hubieramos creado)
                return 0;
            } else {
                // Caso contrario volvemos a buscar de la misma forma en los siguientes 10k clusters
                ++cluster_group_count;
            }
        }

    }
    if (!fat_table_is_valid_cluster_number(table,
                                           bb_dir_start_cluster)) {
        fat_error("There was a problem fetching for bb cluster");
        bb_dir_start_cluster = FAT_CLUSTER_END_OF_CHAIN_MAX;
    }
    return bb_dir_start_cluster;
}

/* Creates the /bb directory as an orphan and adds it to the file tree as
 * child of root dir.
 */
static int bb_create_new_orphan_dir(u32 start_cluster) {

    // RECORDAR: en ningun momento agregar una dentry para este directorio en cluster de '/'. 
    // De hacer eso el dir dejaria de ser huerfano.
    errno = 0;
    fat_volume vol = NULL;
    fat_tree_node root_node = NULL;

    vol = get_fat_volume();

    // Create a new file from scratch, instead of using a direntry like
    // normally done. 
    fat_file loaded_bb_dir = fat_file_init_orphan_dir(BB_DIRNAME, vol->table, start_cluster);

    // Add directory to file tree. It's entries will be like any other dir.
    // Agregar el directorio huerfano en /bb.
    // Si ya existe ese directorio bueno xd.
    root_node = fat_tree_node_search(vol->file_tree, "/");
    vol->file_tree = fat_tree_insert(vol->file_tree, root_node, loaded_bb_dir);


    return -errno;
}

int bb_init_log_dir() {
    errno = 0;

    fat_volume vol = get_fat_volume();
    

    // Busca el BB cluster (directorio huerfano)
    u32 start_cluster = search_bb_orphan_dir_cluster();

    // Si no encontramos uno libre y lo inicializamos ahi
    if(start_cluster == 0) {
        start_cluster = fat_table_get_next_free_cluster(vol->table);
        // Marcar el cluster como malo
        fat_table_set_next_cluster(vol->table, start_cluster, FAT_CLUSTER_BAD_SECTOR);
    } 
    bb_create_new_orphan_dir(start_cluster);

    return -errno;
}
