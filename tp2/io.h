/**
 * @file   io.h
 * @date   02/11/2014
 */


#ifndef IO_H
#define IO_H

#define CLUSTER_SIZE 2048
#define NUMBER_OF_CLUSERS 1024

// Creates a new zeroed file (not formated!)
int create_new_fs(const char *fs_image_name);
// Loads an existing image file
int load_fs(const char *fs_image_name);
// Unloads the image currently in use
int unload_fs();
// Reads a cluster from the loaded image
int read_cluster(unsigned int cluster_id, char *buffer);
// Writes a cluster to the loaded image
int write_cluster(unsigned int cluster_id, const char *buffer);

#endif // IO_H
