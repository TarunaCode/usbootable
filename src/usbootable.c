/*  
 *   usbootable For Creating bootable usb media
 *   Copyright (C) 2020-2021 Maulana B.
 *   
 *   This Program is free software you can redistribute it and/or modify 
 *   it under terms of the GNU General Public License as published by
 *   Free Software Foundation , either version 3 of the License, or
 *   (at your option) any later version
 *  
 *  
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTIBILTY or FITNESS FOR A PARTICULAR PURPOSE.   See the
 *   GNU General Public License for more details
 *  
 *   You should have received a copy of the GNU General Public License
 *   along with this program. If not, see <https//www.gnu.org/licenses/>
 *
 *   Maulana B. <https://github.com/floppvs>
 *
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>

#include <conio.h>
#include <fcntl.h>
#include <dirent.h>

#include <errno.h>
#include <err.h>

#include <cdk/cdk_test.h>

#include <sys/mount.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <blkid/blkid.h>
#include "huebo_exec.h"



void makee_x(void){
	system("clear");	
	CDKSCREEN *scr = 0;
	CDKENTRY *ent = 0;
	CDKFSELECT *fsl = 0;

	FILE *fl;
	FILE *fx;

	DIR *path;

	int i;
	int total_partition;

	int fr;
	int mkdir_return_value;

	int retr;

	int mount_return_value;

	int yesno_int;

	const char *tombol[5];
	const char *pesanx[4];
	const char *judul = "<C> Choose file with .iso formatted on it";
	const char *label = "File: ";
	const char *dir = ".";

	const char *mode;

	const char *partition;
	const char *uuid;
	const char *lbel;
	const char *type;

	const char *source;
	const char *target;
	const char *filesystem;

	const char *cmd,*cmd2,*cmd3,*cmd4/**/;

	//char *const* cmdargslist[5];
	//struct partition pt;

	char *isofile= 0;

	//const char *cmd;
	//char *const cmdargslist[5];

	char inputKey[10];

	char result[1035];
	char resutx[1035];


	char nestx[1024];

	char yesno[1024];

	tombol[0]="</5>OK<!5>";
	tombol[1]="</5>Cancel<!5>";

	mode="r";

	target = "/mnt";

	scr = initCDKScreen(NULL);
	
	initCDKColor(); /* initialize CDKColor (Background) */

	fsl = newCDKFselect(scr,CENTER,CENTER,20,65,judul,label,A_NORMAL,'_',A_REVERSE,"</5>","</48>","</N>","</N>",TRUE,FALSE); /* Create new object : CDKFSelect */

	setCDKFselect(fsl,dir,A_NORMAL,' ',A_REVERSE,"</5>","</48>","</N>","</N>",ObjOf(fsl)->box); /* Set New Object : CDKFSelect */

	isofile = copyChar(activateCDKFselect(fsl,0)); /* Store Filename, And Path to isofile variable */

	destroyCDKScreen(scr); /* Destroy CDKScreen */

	endCDK(); /* ENd CDK */

	/* Select Disk */
	
	printf("Select Disk : ex : /dev/sdXX\n");
	printf("\n");
	fl = popen("/usr/bin/sudo fdisk -l | egrep -v '(sd[b-d][1-9])' | grep 'GiB' | awk -F ' ' '{print  " " $2 $3 " " $4}' | sed -e 's/.$//' | grep 'sd[b-d]'",mode);
	if(fl == NULL){
		printf("failed to execute command : Error %d : %s",errno,strerror(errno));
		exit(EXIT_FAILURE);
	}else{
		while(fgets(result,sizeof(result),fl) != NULL){
			printf("%s",result);
		}
		
	}

	char xs[256];
	printf("\n");
	printf("Disk : ");
	scanf("%s",xs);

	system("clear");

	printf("--------------------------------DISK INFORMATION-------------------------------");
	printf("\n");
	printf("Selected Disk : %s\n" ,xs);
	/* Print Partition For user-selected disk. */
	partition=xs;
	
	blkid_probe part = blkid_new_probe_from_filename(partition);

	if(!part){
		perror("Failed To Open Device");
		exit(EXIT_FAILURE);
	}
	blkid_partlist partition_list;

	partition_list = blkid_probe_get_partitions(part);

	total_partition = blkid_partlist_numof_partitions(partition_list);

	if( total_partition == 0 ){
		perror("Failed to Get Partition info");
	}
	for(i=0;i<total_partition;i++){
		sprintf(nestx,"%s%d",partition,(i+1));

		part = blkid_new_probe_from_filename(nestx);

		blkid_do_probe(part);
		
		blkid_probe_lookup_value(part,"UUID",&uuid,NULL);
		blkid_probe_lookup_value(part,"LABEL",&lbel,NULL);
		blkid_probe_lookup_value(part,"TYPE",&type,NULL);

		
		printf("Partition : %s\n",nestx);
		/*printf("Label: %s\n",lbel);
		printf("Type : %s\n",type);
		printf("UUID : %s\n",uuid);*/
		printf("-------------------------------------------------------------------------------\n");
	}
	blkid_free_probe(part);
	sleep(6);
	system("clear");
	printf("==================================================\n");
	printf("%s WILL BE FORMATTED WITH NTFS FILESYSTEM\n",nestx);
	printf("AND ALL THE DATA CONTAINS IN %s WILL BE ERASED\n",nestx);
	printf("AND CAN CAUSE YOUR DISK UNRECOVERABLE\n");
	printf("PRESS CTRL-C TO CANCEL PROCESS \n");
	printf("===================================================\n");
	printf("\n");
	printf("Program will continue in 5 second");
	for(i=0;i < 5;i++){
		sleep(1);
	}
	system("clear");
	/* Format the partition Using mkntfs */
	cmd1 = "mkntfs";
	char *const* cmdargslista[] = { "mkntfs","-v -F -L 'Bootable'" ,nestx,NULL};
	exec_fork(cmd1,cmdargslista);
	path = opendir("/mnt/usbootable");
	if(path){
		closedir(path);
	}
	else if( ENOENT == errno ){
		mkdir_return_value = mkdir("/mnt/usbootable",0777);
		if(!mkdir_return_value){
			perror("Mkdir"); /* Use perror() for printing the status of mkdir , but this is not to always display error */
		}else{
			perror("Mkdir"); /* display error since this block is an else block */
			exit(EXIT_FAILURE);
		}
	}else{
		perror("Mkdir"); /* Display mkdir() function error */
		exit(EXIT_FAILURE);
	}
	path = opendir("/mnt/usbootable_iso");
	if(path){
		closedir(path);
	}
	else if(ENOENT == errno){
		mkdir_return_value = mkdir("/mnt/usbootable_iso",0777);
		if(!mkdir_return_value){
			perror("Mkdir"); /* Just Same */
		}else{
			perror("Mkdir"); /* Just Same */
			exit(EXIT_FAILURE);
		}
	}else{
		perror("Mkdir"); /* Just Same */
		exit(EXIT_FAILURE);
	}


	/* Mount partition */
	printf("Mounting Partition : %s to /mnt/usbootable ",nestx);
	sleep(1);

	source = nestx;
	filesystem = type;

	mount_return_value = mount(source,target,filesystem,MS_SYNCHRONOUS,NULL);
	if(mount_return_value == -1){
		perror("Mount");
		exit(EXIT_FAILURE);
	}
	printf("Mounted Succesfully %s on %s with %s fs",source,target,filesystem);
	sleep(2);
	system("clear");
	printf("Mounting %s to /mnt/usbootable_iso",isofile);
	cmd2 = "mount";
	char *const* cmdargslistb[]  = { "mount" , isofile ,"/mnt/usbootable_iso",NULL};
	exec_fork(cmd2,cmdargslistb);
	printf("Mounted Succeesfully %s , to /mnt/usbootable_iso",isofile);
	sleep(5);

	system("clear");

	cmd3 = "cp";
	char *const* cmdargslistc[] = { "cp" , "-rvf", "/mnt/usbootable_iso/*","/mnt/usbootable"};
	printf("Copying file.......");
	exec_fork(cmd3,cmdargslistc);

	system("clear");

	printf("Unmounying %s & %s ",isofile,nestx);
	cmd4 = "umount";
	char *const* cmdargslistd[] = { "umount",isofile };
	exec_fork(cmd4,cmdargslistd);
	umount(nestx);

	scr = initCDKScreen(NULL);

	pesanx[0] = ".";
	pesanx[1] = "Succesfully making bootable media ";
	pesanx[2] = ".";
	pesanx[3] = "Now You can test to boot it ";

	popupLabel(scr,(CDK_CSTRING2)pesanx,4);

	destroyCDKScreen(scr);

	endCDK();
	ExitProgram(EXIT_SUCCESS);
}
int main(int argc,char **argv){
	
	/* Variable(s) declaration for main function */

	const char *exp = "-V";

	const char *opt = argv[1];

	int opsi;
	int op;

	/* End Of Variable(s) Filling And Declare */

	if(argc != 2){
		system("clear");
	}else{
	
		while((op = getopt(argc,argv,"V")) != -1){
			switch(op){
				default :
					printf("Invalid Argument %s ",opt);
					exit(EXIT_FAILURE);
				case 'V' :
					printf("usbootable  Copyright (C) 2021  Maulana B.\n");
					printf("This program comes with ABSOLUTELY NO WARRANTY.\n");
					printf("This is free software, and you are welcome to redistribute it\n");
					printf("under certain conditions\n");
					printf("\n");
					printf(" Maulana B. <https://github.com/floppvs>\n");
					exit(EXIT_SUCCESS);
			}
		}
	}
	printf("-----------------------------------\n");
	printf("| USB Bootable Maker For GNU/Linux |\n");
	printf("|         Version : 1.0-a          |\n");
	printf("------------------------------------\n");
	printf(" 1. Start		2. Exit\n");
	printf("> ");
	scanf("%d",&opsi);
	switch(opsi){
		case 1:
			makee_x();
			break;
		default:
			clear();
			return 1;
			exit(EXIT_FAILURE);
	}

}

