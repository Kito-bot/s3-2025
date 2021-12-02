#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "image_split.h"

void split_image(void){
	

	char *binary = "mkdir";
  	char *a1 = "pic";
 
  	execl(binary, binary, a1,NULL);

	char *binaryPath = "cp";
  	char *arg1 = "../.pic/*.png";
  	char *arg2 = "./pic/";
 
  	execl(binaryPath, binaryPath, arg1, arg2, NULL);

}