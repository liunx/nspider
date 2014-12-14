/*
 * =====================================================================================
 *
 *       Filename:  popen.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2014年12月13日 16时00分21秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/select.h>

int main()
{
	FILE *fp;
	int fd;
	int flags = 1;
	char buf[64];
	//ssize_t ret;
	fd_set rfds;
	printf("popen...\n");
	fp = popen("sleep 10;echo hello", "r");
	fd = fileno(fp);
	FD_ZERO(&rfds);
	FD_SET(fd, &rfds);
	if (0) {
		flags = fcntl(fd, F_GETFL, 0);
		fcntl(fd, F_SETFL, flags | O_NONBLOCK);
	}
	printf("select...\n");
	select(fd + 1, &rfds, NULL, NULL, NULL);
	printf("fread...\n");
	fread(buf, sizeof(char), 64, fp);
	printf("pclose...\n");
	//fp = fdopen(fd, "r");
	pclose(fp);
	printf("return...\n");
	sleep(100);
	return 0;
}

