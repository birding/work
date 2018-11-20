

[root@localhost u-boot]# cp common/main.c /home/github/work/

static int abortboot_normal(int bootdelay)

	if(abortbootchar != NULL){
		printf("Hit '#' key to stop autoboot: %2d ", bootdelay);
	}else{
		printf("Hit any key to stop autoboot: %2d ", bootdelay);
	}


