#pragma once


#include <struct.h>
#include <list.h>
#include <fstyps.h>
#include <fsdefs.h>


EXTSTRUCT(FILE);
EXTSTRUCT(FILE_OPS);
EXTSTRUCT(INODE_OPS);
EXTSTRUCT(INODE);
EXTSTRUCT(DIRENT);
EXTSTRUCT(U_DIRENT);
EXTSTRUCT(SUPER_OPS);
EXTSTRUCT(SUPER);
EXTSTRUCT(FSDRIVE);

typedef struct FILE DIR;
typedef struct FILE_OPS DIR_OPS;


struct FILE // 表示一个文件,或一个目录
{union{
	struct {
		LIST_NODE f_list;

		FILE_OPS *f_ops; // 应该会在文件打开的时候指定
		INODE *f_inode;
		unsigned int f_oattr; // 文件open时的oattr哇

		off_t f_pos;
		off_t f_size;

		union {
			void *fe_priv_data;

			struct FIFO *fe_fifo;
			struct EFIFO *fe_efifo;

			struct {
				clus_t fe_clus;
				unsigned int fe_cloff;
			};
		};
	};

	struct {
		LIST_NODE d_list;

		DIR_OPS *d_ops;
		INODE *d_inode;
		unsigned int d_oattr;

		union {
			void *d_priv_data;

			LIST_HEAD de_pos;
		};
	};
};};


struct FILE_OPS // 文件操作,读写之类的
{
	union {
		int (*open)(FILE *file, INODE *inode, unsigned int oattr); // 打开文件
		int (*opendir)(DIR *dir, INODE *inode, unsigned int oattr); // 打开目录
	};
	union {
		int (*close)(FILE *file); // 关闭文件(但是不释放FILE指针内存的那种)
		int (*closedir)(DIR *dir);
	};

	int (*read)(FILE *file, char *buf, size_t size); // 读取文件
	int (*write)(FILE *file, const char *buf, size_t size); // 写入文件
	unsigned int (*getch)(FILE *file); // 读入一个字节
	int (*putch)(FILE *file, unsigned char ch); // 输出一个字节
	char *(*getline)(FILE *file); // 读取一行, 返回一个buf, 需要kfree
	// TODO: 考虑给getline加个`void *(*alloc)(size_t)`参数?
	int (*mmap) // 把文件的内容直接映射到虚拟内存空间中
		( FILE *file // 哪个文件?
		// 额,现在我们打算通过seek来给定偏移量了:
		//, off_t off // 从文件中的什么地方开始映射数据? (有的fops要求对齐到页或簇)
		, void *p // 要映射到的虚拟内存地址 (API要求对齐到页)
		, size_t size // 映射多长一段呢? (也要对齐到页哦)
		, unsigned int mattr // 映射要求页的属性? 只读? (pte的参数嘛)
		);
	size_t (*glinesize)(FILE *file); // 预估一行的最大容量(API要求的呗)
	long (*seek)(FILE *file, long offset, int whence); // 定位到特定文件位置
	//int (*tell)(FILE *file, int what); // 告知文件特定的偏移量信息
	int (*fsync)(FILE *file); // 同步文件改动

	// 以下是一些对目录的操作,删除啊遍历啊之类的
	DIRENT *(*dirfind)(DIR *dir, const char *name); // 在目录中寻找一个相应名字的项
	DIRENT *(*readdir)(DIR *dir); // 读取一个目录项
	int (*rewinddir)(DIR *dir); // 重定位到目录开始的那一项
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
	union {
		off_t i_size; // 文件大小
		LIST_HEAD ie_dents;
	};

	union {
		void *ie_priv_data;
		struct FIFO *ie_fifo;
		struct EFIFO *ie_efifo;
		struct PIPE *ie_pipe;

		struct {
			unsigned int ie_clus;
			unsigned int ie_attr;
		};
	};
};


#include <udirent.h>

struct DIRENT // 表示目录中的一个项目, 比如/home/bate, 则bate是home的一个dirent
{
	LIST_NODE e_list;

	union {
		U_DIRENT e_ude;
		struct {
			char e_name[MAX_FNAME + 1];
			INODE *e_inode; // 这个目录项对应的inode
		};
	};
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
};

struct SUPER // 表示一个具有文件系统的块, 比如: /dev/fd0具有fat12的文件系统
{
	LIST_NODE s_list;

	struct SUPER_OPS *s_ops; // 通常会在FILESYS的read_super方法里设置

	INODE *s_inode;

	DIR *s_root;

	union {
		void *se_priv;
		struct {
			char *se_ramdat;
			clus_t *se_fat;
			clus_t se_clusmax;
			unsigned int se_clusiz;
		};
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
int open_in(FILE *file, DIR *indir, const char *name, unsigned int oattr);
DIRENT *simple_readdir(DIR *dir);
DIRENT *readdir(DIR *dir);
int rewinddir(DIR *dir);
DIRENT *dirfind(DIR *dir, const char *name);
DIRENT *simple_dirfind(DIR *dir, const char *name);
int simple_open(FILE *f, INODE *inode, unsigned int oattr);
int simple_opendir(DIR *dir, INODE *inode, unsigned int oattr);
int simple_rewinddir(DIR *dir);
int simple_close(FILE *f);
int simple_closedir(DIR *d);
int read(FILE *file, char *buf, size_t size);
int write(FILE *file, const char *buf, size_t size);
int mmap(FILE *f, void *p, size_t size, unsigned int mattr);
unsigned int getch(FILE *file);
int putch(FILE *file, unsigned char ch);
long seek(FILE *file, long offset, int whence);
size_t glinesize(FILE *file);
char *getline(FILE *file);
int fsync(FILE *file);
int close(FILE *file);
DIRENT *__dir_new_entry
	( INODE *dirnode
	, const char *name
	, unsigned int iattr
	);
#define dir_new_entry(dirnode, name, iattr) (__dir_new_entry(dirnode, name, iattr)->e_inode)

static inline
int inode_opendir(DIR *dir, INODE *inode, unsigned int oattr)
{
	return inode_open(dir, inode, oattr | OPEN_DIR);
}

static inline
int opendir_in(DIR *dir, DIR *indir, const char *name, unsigned int oattr)
{
	return open_in(dir, indir, name, oattr | OPEN_DIR);
}
