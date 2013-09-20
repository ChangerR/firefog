#ifndef __FIRE_BLK
#define __FIRE_BLK

#define NR_BLK_DEV 7

#define NR_REQUEST 32

typedef struct request_s
{
	int dev;
	int cmd;
	int errors;
	unsigned long sector;
	unsigned long nr_sectors;
	char* buffer;
	task_t* waiting;
	buffer_header_t* bh;
	struct request_s* next;
}request_t

#define IN_ORDER(s1,s2) \
	((s1)->cmd <(s2)->cmd || (s1)->cmd==(s2)->cmd&& \
	((s1)->dev <(s2)->dev || ((s1)->dev == (s2)->dev&&\
	(s1)->sector <(s2)->sector)))
	
typedef struct blk_dev_s {
	void (*request_fn)(void);
	request_t* current_request;
}blk_dev_t;

extern blk_dev_t blk_dev[NR_BLK_DEV];
extern request_t request[NR_REQUEST];
extern task_t* wait_for_request;

#ifdef MAJOR_NR


#if(MAJOR_NR==1)
#define DEVICE_NAME "ramdisk"
#define DEVICE_REQUEST do_rd_request
#define DEVICE_NR(device) ((device)&7)
#define DEVICE_ON(device)
#define DEVICE_OFF(device)

#elif(MAJOR_NR==2)
#define DEVICE_NAME "floppy"
#define DEVICE_INTR do_floppy
#define DEVICE_REQUEST do_fd_request
#define DEVICE_NR(device) ((device)&3)
#define DEVICE_ON(device)	floppy_on(DEVICE_NR(device))
#define DEVICE_OFF(device)	floppy_off(DEVICE_NR(device))

#elif(MAJOR_NR==3)
#define DEVICE_NAME "harddisk"
#define DEVICE_INTR do_hd
#define DEVICE_REQUEST do_hd_request
#define DEVICE_NR(device) ((MINOR(device)/5)
#define DEVICE_ON(device)
#define DEVICE_OFF(device)

#elif

#endif


#define CURRENT (blk_dev[MAJOR_NR].current_request)
#define CURRENT_DEV DEVICE_NR(CURRENT->dev)




#endif	
#endif