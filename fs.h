#pragma once


#include <struct.h>
#include <list.h>
#include <fstyps.h>

#define MAX_FNAME 30


EXTSTRUCT(FILE);
EXTSTRUCT(FILE_OPS);
EXTSTRUCT(DIR);
EXTSTRUCT(INODE_OPS);
EXTSTRUCT(INODE);
EXTSTRUCT(DIRENT);
EXTSTRUCT(SUPER_OPS);
EXTSTRUCT(SUPER);
EXTSTRUCT(FSDRIVE);


struct DIR_OPS
{
};


struct DIR
{
	LIST_HEAD d_ents;
};


struct FILE // 表示一个文件
{
	FILE_OPS *f_ops; // 应该会在open还是alloc的时候指定吧

	INODE *f_inode;
};


struct FILE_OPS // 文件操作,读写之类的
{
	int (*read)(FILE *file, char *buf, size_t size); // 读取文件
	int (*write)(FILE *file, const char *buf, size_t size); // 写入文件
	int (*fsync)(FILE *file); // 同步文件改动
	void (*close)(FILE *file); // 关闭文件
};


struct INODE_OPS // inode(文件节点)操作,好像就创建删除之类的————和FILE_OPS不一样
{
	FILE *(*open)(INODE *inode); // 打开文件
	DIR *(*opendir)(INODE *inode); // 打开目录
	/*int (*create)(INODE *inode, DENTRY *dir); // 创建文件, ln
	int (*remove)(INODE *inode, DENTRY *dir); // 删除文件, rm
	int (*mkdir)(INODE *inode, DENTRY *dir); // 创建目录, mkdir
	int (*rmdir)(INODE *inode, DENTRY *dir); // 删除目录, rmdir
	int (*rename)(INODE *inode, DENTRY *dir); // 重命名, mv*/
};


#define INODE_RD  (1<<0)
#define INODE_WR  (1<<1)
#define INODE_DIR (1<<2)

struct INODE // 表示一个文件或者目录, 算是什么东西的最小单位吧
{
	LIST_NODE i_list;

	unsigned long i_priv_i; // fs驱动的私有变量, 可用于存储起始clus号之类的

	SUPER *i_sb; // 所属的super指针

	INODE_OPS *i_op;

	unsigned int i_attr; // inode属性

	off_t i_size; // 文件大小, 如果是个文件的话

	LIST_HEAD i_dirents; // 如果该inode是个目录的话, 这一项不为空
};


struct DIRENT // 表示目录中的一个项目, 比如/home/bate, 则bate是home的一个dirent
{
	LIST_NODE e_list;

	char e_name[MAX_FNAME + 1];

	SUPER *d_sb;

	INODE *e_inode; // 这个目录项对应的inode
};


struct SUPER_OPS
{
	INODE *(*alloc_inode)(SUPER *sb); // 内存中分配一个inode
	void (*free_inode)(INODE *inode); // 关闭一个分配的inode
	void (*free_super)(SUPER *sb); // 释放一个已加载的super块
	int (*sync_fs)(struct SUPER *sb); // 同步改动到文件系统
};


struct SUPER // 表示一个具有文件系统的块, 比如: /dev/fd0具有fat12的文件系统
{
	struct SUPER_OPS *s_ops; // 通常会在FILESYS的read_super方法里设置

	LIST_NODE s_list;

	DIR *s_root;

	void *s_priv;
};


struct FSDRIVE // 表示一种文件系统类型，比如：ext2, fat12, procfs
{
	FSDRIVE *next;

	const char *name;
	SUPER *(*load_super)(void *arg); // 加载一个具有该种文件系统的super块
};


void add_fsdrive(FSDRIVE *fs);
FSDRIVE *get_fsdrive(const char *name);
DIRENT *dir_find_file(DIR *dir, const char *name);
SUPER *load_super(const char *fsname, void *arg);
void free_super(SUPER *super);
DIRENT *dir_find_file(DIR *dir, const char *name);
FILE *open_inode(INODE *inode);
FILE *open_in_dir(DIR *dir, const char *name);
int read(FILE *file, char *buf, size_t size);
void close(FILE *file);
