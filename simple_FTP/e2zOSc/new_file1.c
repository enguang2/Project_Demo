/**
 * nonstop_networking
 * CS 241 - Fall 2021
 */
#include "common.h"
#include "format.h"
#include <stdio.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <netdb.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>
ssize_t read_from_socket(int socket, char *buffer, size_t count) {
    // Your Code Here
    size_t total = 0;

    while (total < count) {
      ssize_t ret = read(socket, buffer + total, count - total);
      if (ret == 0) {
        break;
      }
      if (ret == -1 && errno == EINTR){
        continue;
      }
      if (ret == -1) {
        perror("read_from_socket");
        return -1;
      }
      total += ret;
    }
    return total;
}

ssize_t write_to_socket(int socket, const char *buffer, size_t count) {
    size_t total = 0;

    while (total < count) {
      ssize_t ret = write(socket, buffer + total, count - total);
      if (ret == 0) {
          break;
      }
      if (ret == -1 && errno == EINTR){
          continue;
      }
      if (ret == -1) {
        perror("write_to_socket");
        return -1;
      }
      total += ret;
    }
    return total;
}

size_t read_head(int socket, char *buffer, size_t count){
	size_t total = 0;

	    while (total < count) {
		ssize_t ret = read(socket, buffer + total, 1);
		if (buffer[strlen(buffer) - 1] == '\n' || ret == 0) {
			break;
		}
        if (ret == -1 && errno == EINTR) {
            continue;
        } else if (ret == -1) {
            perror("read_head");
        }
		
		total += ret;
	}
	return total;
}

int error_dectect(size_t size1, size_t size2) {
  if (size1 != size2 && size1 == 0) {
    print_connection_closed();
    return 1;
  } 
  if (size1 < size2) {
    print_too_little_data();
    return 1;
  }
  if (size2 < size1) {
    print_received_too_much_data();
    return 1;
  }
  return 0;
}