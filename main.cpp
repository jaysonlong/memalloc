/**
 * Author: Github/JaysonLong
 * Desciption: a memory alloc test utils
 */

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<getopt.h>
#include<string.h>

// alloc memory arguments
struct my_arg {
  int size; // memory size
  bool init; // if init by zero immediately after allocated
  int keep_time; // time to sleep after allocated
};

// parse command line arguments
my_arg parse_arg(int argc, char *argv[]);


// alloc memory according to the command line arguments
int main(int argc, char *argv[]) {
    my_arg arg = parse_arg(argc, argv);

    if (arg.size == 0) {
      printf("please specify a memory size greater than 0");
      exit(0);
    }

    printf("start to allocate memory of %d byte, kept for %d millisecond\n", arg.size, arg.keep_time);
    char *arr = (char *) malloc(sizeof(char) * arg.size);;
    if (arg.init) {
      printf("initing by zero\n");
      for (int i=0; i<arg.size; i++) {
        arr[i] = '\0';
      }
    }

    if (arg.keep_time > 0) {
      usleep(arg.keep_time * 1000);
    }
    printf("finish\n");
}

my_arg parse_arg(int argc, char *argv[]) {
  if (argc < 2) {
    printf("please specify the memory size to be allocated");
    exit(0);
  }

  my_arg arg = { 0, 0, false };
  char * size = argv[1];
  int len = strlen(size);
  switch(size[len-1]) {
    case 'b':
    case 'B': size[len-1] = '\0'; arg.size = atoi(size); break;
    case 'k':
    case 'K': size[len-1] = '\0'; arg.size = atoi(size) * 1024; break;
    case 'm':
    case 'M': size[len-1] = '\0'; arg.size = atoi(size) * 1024 * 1024; break;
    case 'g':
    case 'G': size[len-1] = '\0'; arg.size = atoi(size) * 1024 * 1024 * 1024; break;
    default: arg.size = atoi(size); break;
  }

  int opt;
  const char *optstring = "ik:";
  static struct option long_options[] = {  
      {"init", no_argument, NULL, 'i'},
      {"keep", required_argument, NULL, 'k'}
  };
  while((opt = getopt_long(argc, argv, optstring, long_options, NULL)) != EOF) {
    if (opt == 'k') {
      arg.keep_time = atoi(optarg);
    } else if (opt == 'i') {
      arg.init = true;
    }
  }

  return arg;
}