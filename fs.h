#pragma once


#include <struct.h>
#include <list.h>
#include <fstyps.h>

#define MAX_FNAME 30


// inode.i_type:
#define INODE_FIL 0
#define INODE_DIR 1
#define INODE_CHR 2
#define INODE_BLK 3
// inode.i_attr:
#define INODE_RD  (1<<(8+0))
#define INODE_WR  (1<<(8+1))
#define INODE_EX  (1<<(8+2))
// inode.i_fat.ie_attr
#define FAT_RDONLY (1<<0)
#define FAT_HIDDEN (1<<1)
#define FAT_SYSTEM (1<<2)
#define FAT_VOLLAB (1<<3)
#define FAT_SUBDIR (1<<4)
#define FAT_ARCHIV (1<<5)
// open.oattr:
#define OPEN_RD  (1<<0)
#define OPEN_WR  (1<<1)
// seek.whence:
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2


EXTSTRUCT(FILE);
EXTSTRUCT(DIR);
EXTSTRUCT(FILE_OPS);
EXTSTRUCT(DIR_OPS);
EXTSTRUCT(INODE_OPS);
EXTSTRUCT(INODE);
EXTSTRUCT(DIRENT);
EXTSTRUCT(SUPER_OPS);
EXTSTRUCT(SUPER);
EXTSTRUCT(FSDRIVE);


STRUCT(FAT_FILE_EX)
{
	unsigned int fe_cloff;
	clus_t fe_clus;
	clus_t fe_beg_clus;
};

struct FILE // 表示一个文件
{
	LIST_NODE f_list;

	FILE_OPS *f_ops; // 应该会在文件打开的时候指定
	INODE *f_inode;
	unsigned int f_oattr; // 文件open时的oattr

	off_t f_pos;
	off_t f_size;

	union {
		FAT_FILE_EX f_fat;
	};
};


STRUCT(FAT_DIR_EX)
{
};

struct DIR
{
	LIST_NODE d_list;

	DIR_OPS *d_ops;
	INODE *d_inode;
	unsigned int d_oattr;

	LIST_HEAD d_ents; // TODO: put this in FAT_DIR_EX!
	union {
		FAT_DIR_EX d_fat;
	};
};


struct FILE_OPS // 文件操作,读写之类的
{
	int (*open)(FILE *file, INODE *inode, unsigned int oattr); // 打开文件
	int (*read)(FILE *file, char *buf, size_t size); // 读取文件
	int (*write)(FILE *file, const char *buf, size_t size); // 写入文件
	int (*seek)(FILE *file, long offset, int whence); // 定位到特定文件位置
	//int (*tell)(FILE *file, int what); // 告知文件特定的偏移量信息
	int (*fsync)(FILE *file); // 同步文件改动
	int (*close)(FILE *file); // 关闭文件(但是不释放FILE*的内存的那种)
};

struct DIR_OPS // 对目录的操作,删除啊遍历啊之类的
{
	int (*opendir)(DIR *dir, INODE *inode, unsigned int oattr); // 打开目录
	int (*closedir)(DIR *dir);
};


struct INODE_OPS // inode(文件节点)操作,好像就创建删除之类的————和FILE_OPS不一样
{
	/*int (*create)(INODE *inode, DIRENT *dir); // 创建文件, ln
	int (*remove)(INODE *inode, DIRENT *dir); // 删除文件, rm
	int (*mkdir)(INODE *inode, DIRENT *dir); // 创建目录, mkdir
	int (*rmdir)(INODE *inode, DIRENT *dir); // 删除目录, rmdir
	int (*rename)(INODE *inode, DIRENT *dir,
		      INODE *inode2, DIRENT *dir2); // 重命名, mv*/
};


STRUCT(FAT_INODE_EX)
{
	unsigned int ie_clus;
	unsigned int ie_attr;
};

STRUCT(DEVDIR_INODE_EX)
{
	LIST_HEAD ie_dents;
};

struct INODE // 表示一个文件或者目录, 算是什么东西的最小单位吧
{
	LIST_NODE i_list;

	INODE_OPS *i_ops;
	union {
		FILE_OPS *i_fops;
		DIR_OPS *i_dops;
	}; // 具体是哪个由i_attr的INODE_DIR字段确定

	SUPER *i_sb; // 所属的super指针

	union {
		unsigned char i_type; // inode类型
		unsigned int i_attr; // inode属性
	};
	off_t i_size; // 文件大小

	union {
		FAT_INODE_EX i_fat; // fat系统的私有变量, 存储起始clus号之类的
		DEVDIR_INODE_EX i_devdir; // /dev节点的私有变量
	};
};


struct DIRENT // 表示目录中的一个项目, 比如/home/bate, 则bate是home的一个dirent
{
	LIST_NODE e_list;

	char e_name[MAX_FNAME + 1];

	INODE *e_inode; // 这个目录项对应的inode
};


struct SUPER_OPS
{
	INODE *(*alloc_inode)(SUPER *sb); // 内存中分配一个inode
	void (*free_inode)(INODE *inode); // 关闭一个分配的inode
	void (*free_super)(SUPER *sb); // 释放一个已加载的super块
	int (*sync_fs)(struct SUPER *sb); // 同步改动到文件系统
};


STRUCT(FAT_SUPER_EX)
{
	char *se_data;
	clus_t *se_fat;
	clus_t se_clusmax;
	unsigned int se_clusiz;
};

struct SUPER // 表示一个具有文件系统的块, 比如: /dev/fd0具有fat12的文件系统
{
	LIST_NODE s_list;

	struct SUPER_OPS *s_ops; // 通常会在FILESYS的read_super方法里设置

	INODE *s_inode;

	DIR *s_root;

	struct {
		FAT_SUPER_EX s_fat;
	};
};


struct FSDRIVE // 表示一种文件系统类型，比如：ext2, fat12, procfs
{
	FSDRIVE *next;

	const char *name;
	SUPER *(*load_super)(void *arg); // 加载一个具有该种文件系统的super块
};


void add_fsdrive(FSDRIVE *fs);
FSDRIVE *get_fsdrive(const char *name);
SUPER *load_super(const char *fsname, void *arg);
void free_super(SUPER *super);
INODE *alloc_inode(SUPER *sb);
void free_inode(INODE *inode);
DIRENT *dir_find_entry(LIST_HEAD dents, const char *name);
DIRENT *dir_locate_entry(DIR *_dir, const char *_name);
int inode_open(FILE *file, INODE *inode, unsigned int oattr);
int inode_opendir(DIR *dir, INODE *inode, unsigned int oattr);
int open_in(FILE *file, DIR *indir, const char *name, unsigned int oattr);
int opendir_in(DIR *dir, DIR *indir, const char *name, unsigned int oattr);
void closedir(DIR *file);
int read(FILE *file, char *buf, size_t size);
int seek(FILE *file, long offset, int whence);
void close(FILE *file);
