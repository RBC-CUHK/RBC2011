#include "arm_math.h"

int numDigi(int num){	//max for 7 digi
	//check for -ve
	if (num<0){
		num=-num;
	}
	if (num >= 1000000000){
		return 10;
	}else if(num>=100000000){
		return 9;
	}else if(num>=10000000){
		return 8;
	}else if(num>=1000000){
		return 7;
	}else if(num>=100000){
		return 6;
	}else if (num>=10000){
		return 5;
	}else if (num >=1000){
		return 4;
	}else if (num>=100){
		return 3;
	}else if (num>=10){
		return 2;
	}else if (num>=0){
		return 1;
	}
	if (num>10000000){
		return -1;
	}
	return -2;

}

int pow2(int n){
	switch (n){
	case 0:
		return 1;
	case 1:
		return 2;
	case 2:
		return 4;
	case 3:
		return 8;
	case 4:
		return 16;
	case 5:
		return 32;
	case 6:
		return 64;
	case 7:
		return 128;
	case 8:
		return 256;
	case 9:
		return 512;
	case 10:
		return 1024;
	case 11:
		return 2048;
	case 12:
		return 4096;
	case 13:
		return 8192;
	case 14:
		return 16384;
	case 15:
		return 32768;
	case 16:
		return 65536;
	}
	return -1;
}

int pow10(int n){
	switch (n){
	case 0:
		return 1;
	case 1:
		return 10;
	case 2:
		return 100;
	case 3:
		return 1000;
	case 4:
		return 10000;
	case 5:
		return 100000;
	case 6:
		return 1000000;
	case 7:
		return 10000000;
	case 8:
		return 100000000;
	case 9:
		return 1000000000;
	}
	return -1;
}
