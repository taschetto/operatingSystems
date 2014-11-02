/**
 * @file   io.h
 * @date   02/11/2014
 */


#ifndef IO_H
#define IO_H

#define CLUSTER_SIZE 2048

int read_cluster(int id, char *buffer);
int write_cluster(int id, const char *buffer);

#endif // IO_H
