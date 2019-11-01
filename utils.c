#include "utils.h"

long long int mstime(struct timeval time){
	long long int ret = ((long long int)time.tv_sec)*1000;
	ret += time.tv_usec/1000;
	return ret;
}
