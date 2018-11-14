#include <fs.h>
#include <mbr.h>
#include <kmalloc.h>
#include <string.h>
#include <memory.h>
#include <panic.h>
#include <print.h>
#include <derive.h>
#include <struct.h>
#include <wchar.h>

typedef unsigned int clus_t; // use fat of 32bit in ramfs, (although 12bit physically)

// references: https://blog.csdn.net/dingbihu/article/details/51475351
#define FAT_RDONLY (1<<0)
#define FAT_HIDDEN (1<<1)
#define FAT_SYSTEM (1<<2)
#define FAT_VOLLAB (1<<3)
#define FAT_SUBDIR (1<<4)
#define FAT_ARCHIV (1<<5)
#define FAT_NAME_MAX 8
#define FAT_EXT_MAX  3

#define FAT_LLAST (1<<6)
#define FAT_LSEQMASK 0x1f
#define FAT_LNAME1_MAX 5
#define FAT_LNAME2_MAX 6
#define FAT_LNAME3_MAX 2
#define FAT_LNAME_MAX 13
#define FAT_LMAGIC 0x0f

STRUCT(FAT_DIRENT)
{
	union {
		struct {
			char e_name[FAT_NAME_MAX];
			char e_ext[FAT_EXT_MAX];
			unsigned char e_attr;
			unsigned char e_reserved;
			unsigned char e_ctime_10ms;
			unsigned short e_ctime;
			unsigned short e_cdate;
			unsigned short e_adate;
			unsigned short e_clus_hi;
			unsigned short e_mtime;
			unsigned short e_mdate;
			unsigned short e_clus_lo;
			unsigned int e_size;
		} __attribute__((packed));
		struct {
			unsigned char l_attr;
			wchar_t l_name1[FAT_LNAME1_MAX];
			unsigned char l_magic;
			unsigned char l_reserved;
			unsigned char l_chksum;
			wchar_t l_name2[FAT_LNAME2_MAX];
			unsigned short l_clus;
			wchar_t l_name3[FAT_LNAME3_MAX];
		} __attribute__((packed));
	};
};

STRUCT(RAMFS_SUPER_PRIV)
{
	char *s_data;
	clus_t *s_fat;
	unsigned int s_clusiz;
};

STRUCT(RAMFS_FILE)
{
	FILE f_file;
	off_t f_pos;
	off_t f_size;
	unsigned int f_cloff;
	clus_t f_clus;
};

static
int ramfs_read(FILE *file, char *buf, size_t size)
{
	SUPER *sb = file->f_inode->i_sb;
	RAMFS_FILE *f = derive_of(RAMFS_FILE, f_file, file);
	RAMFS_SUPER_PRIV *s_priv = sb->s_priv;

	int i = 0;
	printf("f_clus=%d\n", f->f_clus);
	while (1) {
		off_t off = f->f_clus * s_priv->s_clusiz + f->f_cloff;
		while (i++ < size && f->f_cloff < s_priv->s_clusiz)
		{
			if (f->f_pos >= f->f_size)
				return i;
			f->f_pos++, f->f_cloff++;
			printf("data='%c'\n", s_priv->s_data[off]);
			*buf++ = s_priv->s_data[off++];
		}
		if (i >= size)
			return i;
		f->f_clus = s_priv->s_fat[f->f_clus];
		f->f_cloff = 0;
	}
}

static
void ramfs_close(FILE *file)
{
	kfree(file);
}

FILE_OPS ramfs_fops = {
	.read = ramfs_read,
	.close = ramfs_close,
};

static
FILE *ramfs_open(INODE *inode)
{
	RAMFS_FILE *f = kmalloc_for(RAMFS_FILE);
	f->f_file.f_inode = inode;
	f->f_file.f_ops = &ramfs_fops;
	f->f_clus = inode->i_priv_i;
	f->f_size = inode->i_size;
	f->f_cloff = 0;
	f->f_pos = 0;

	return &f->f_file;
}

static
DIR *ramfs_opendir(__attribute__((unused)) INODE *inode)
{
	return NULL;
}

INODE_OPS ramfs_iops = {
	.open = ramfs_open,
	.opendir = ramfs_opendir,
};

static
void ramfs_free_super(SUPER *sb)
{
	kfree(((RAMFS_SUPER_PRIV*)sb->s_priv)->s_fat);
	kfree(sb->s_priv);
	for (LIST *le = sb->s_root->d_ents; le;) {
		DIRENT *de = list_entry(DIRENT, e_list, le);
		le = le->next;
		kfree(de->e_inode);
		kfree(de);
	}
	kfree(sb->s_root);
	kfree(sb);
}

INODE *ramfs_alloc_inode(SUPER *sb)
{
	INODE *inode = kmalloc_for(INODE);
	inode->i_sb = sb;
	inode->i_op = &ramfs_iops;
	return inode;
}

void ramfs_free_inode(INODE *inode)
{
	kfree(inode);
}

static
SUPER_OPS ramfs_super_ops = {
	.free_super = ramfs_free_super,
	.alloc_inode = ramfs_alloc_inode,
	.free_inode = ramfs_free_inode,
};

STRUCT(RAMFS_LOADING)
{
	SUPER *sb;

	char *data;
	MBR_HEADER *mbr;

	char *fat12;
	unsigned int fat_ents;

	FAT_DIRENT *rootdir;
	unsigned int rootdir_ents;
};

static
int find_rev_not_space(const char *s, int len)
{
	int i;
	for (i = len - 1; i >= 0; --i)
		if (s[i] != ' ' && s[i])
			return i;
	return -1;
}

static
int get_fat_dirent_name(char *buf, FAT_DIRENT const *fe)
{
	int i, j;
        i = find_rev_not_space(fe->e_name, FAT_NAME_MAX) + 1;
	if (i)
		memcpy(buf, fe->e_name, i);
        j = find_rev_not_space(fe->e_ext, FAT_EXT_MAX) + 1;
	if (j) {
		buf[i++] = '.';
		memcpy(buf + i, fe->e_ext, j);
	}
	buf[i + j] = 0;
	return i + j;
}

static
int w_find_rev_not_eil(const wchar_t *s, int len)
{
	int i;
	for (i = len - 1; i >= 0; --i)
		if (s[i] != 0xffff && s[i])
			return i;
	return -1;
}

static
char *append_fat_long_name(char *p, FAT_DIRENT const *fe)
{
	const wchar_t *lns[] = { fe->l_name1, fe->l_name2, fe->l_name3 };
	size_t lls[] = { FAT_LNAME1_MAX, FAT_LNAME2_MAX, FAT_LNAME3_MAX };
	for (int t = 0; t < 3; t++) {
		const wchar_t *ns = lns[t];
		int i = w_find_rev_not_eil(ns, lls[t]) + 1;
		/*if (i != lls[t])
			break;*/
		while (i--)
			*p++ = *ns++;
	}
	return p;
}

static
unsigned char get_short_name_chksum(const char *s)
{
	int i = 11;
	unsigned char r = 0;
	while (i--)
		r = (r<<7) + (r>>1) + *s++;
	return r;
}

static
void copy_fate_to_inode(FAT_DIRENT *fe, INODE *inode)
{
	inode->i_attr |= INODE_RD;
	if (!(fe->e_attr & FAT_RDONLY))
		inode->i_attr |= INODE_WR;
	if (!(fe->e_attr & FAT_SUBDIR))
		inode->i_attr |= INODE_DIR;
	inode->i_size = fe->e_size;

	inode->i_priv_i = fe->e_clus_hi;
       	inode->i_priv_i <<= 16;
	inode->i_priv_i += fe->e_clus_lo;
}

static
void ramfs_decode_rootents(RAMFS_LOADING *es)
{
	//printf("%x\n", sizeof(FAT_DIRENT));
	//printf("loading rootents...(%d maxium)\n", es->rootdir_ents);
	int total = 0;
	unsigned char chksum = 0;
	DIRENT *de = kmalloc_for(DIRENT);
	es->sb->s_root = kmalloc_for(DIR);
	es->sb->s_root->d_ents = 0;
	for (int i = es->rootdir_ents; i >= 0; i--)
	{
		FAT_DIRENT *fe = &es->rootdir[i];
		if (/*fe->e_attr == FAT_LMAGIC && */(fe->e_attr & FAT_VOLLAB)) {
			//printf("volume label: %11s\n", fe->e_name);
			continue;
		}
		if (get_fat_dirent_name(de->e_name, fe)) {
			total++;
			chksum = get_short_name_chksum(fe->e_name);
			char *p = de->e_name;
			FAT_DIRENT *nfe = fe;
			while (i > 0 && es->rootdir[i - 1].e_attr == FAT_LMAGIC) {
				nfe = &es->rootdir[--i];
				if (chksum != nfe->l_chksum) {
					get_fat_dirent_name(de->e_name, fe);
					break;
				}
				p = append_fat_long_name(p, nfe);
				*p = 0;
			}
			//printf("loaded entry: %s\n", de->e_name);
			list_link_s2(&de->e_list, es->sb->s_root->d_ents);
			de->e_list.prev = 0;
			es->sb->s_root->d_ents = &de->e_list;
			de->d_sb = es->sb;
			INODE *inode = ramfs_alloc_inode(es->sb);
			copy_fate_to_inode(fe, inode);
			de->e_inode = inode;
			de = kmalloc_for(DIRENT);
			p = de->e_name;
			fe = nfe;
			continue;
		}
	}
	de->e_list.prev->next = 0;
	kfree(de);
	//printf("rootents loaded (%d total)\n", total);
}

static
void ramfs_decode_mbr(RAMFS_LOADING *es)
{
	RAMFS_SUPER_PRIV  *s_priv = es->sb->s_priv;
#define FAQ(x, y) ((x)    = (typeof(x)) (y)) // LETHER_MAN!!
	FAQ(es->mbr       , es->data);
	unsigned int bps  = es->mbr->bpb_bytes_per_sec;
	s_priv->s_clusiz  = es->mbr->bpb_sec_per_clus * bps;
	FAQ(es->fat12     , es->data + bps * es->mbr->bpb_rsvd_secs);
	FAQ(es->rootdir   , es->data + bps * (0
			  + es->mbr->bpb_rsvd_secs
			  + es->mbr->bpb_fat_size16 * es->mbr->bpb_num_fats
			  //+ es->mbr->bpb_hidd_secs
			  ));
	FAQ(s_priv->s_data, (char *) es->rootdir
			  + es->mbr->bpb_root_ents * sizeof(FAT_DIRENT)
			  - 2 * s_priv->s_clusiz
			  //+ es->mbr->bpb_hidd_secs * s_priv->s_bps
			  );
	es->fat_ents      = (es->mbr->bpb_fat_size16 * 3) >> 1;
	es->rootdir_ents  = es->mbr->bpb_root_ents;

	printf("ramfs: super block info in mbr:\n");
	printf("  VOL NAME  %.11s\n", es->mbr->bs_vol_label);
	printf("  VOL ID    0x%8X\n", es->mbr->bs_vol_number);
	printf("  OEM NAME  %.8s\n", es->mbr->bs_oem_name);
}

static
void ramfs_uncompress_fat(RAMFS_LOADING *es)
{
	int i;
	clus_t *p = kmalloc(es->fat_ents * sizeof(clus_t));
	((RAMFS_SUPER_PRIV*)es->sb->s_priv)->s_fat = p;
	const char *p12 = es->fat12;
	for (i = 0; i < es->fat_ents; i++) {
		if (i % 2) {
			*p++ = (*(short const*)p12++)&0xfff;
		} else {
			*p++ = (*(short const*)p12++)>>4;p12++;
		}
	}
	es->fat12 = 0;
}

static
SUPER *ramfs_load_super(void *ramdisk)
{
	SUPER *sb = kmalloc_for(SUPER);
	RAMFS_LOADING *es = kmalloc_for(RAMFS_LOADING);

	es->sb = sb;
	es->sb->s_priv = kmalloc_for(RAMFS_SUPER_PRIV);
	es->data = (char *) ramdisk;

	ramfs_decode_mbr(es);
	ramfs_decode_rootents(es);
	ramfs_uncompress_fat(es);

	es->sb->s_ops = &ramfs_super_ops;

	kfree(es);

	return sb;
}

static
FSDRIVE ramfs_drive = {
	.name = "ramfs",
	.load_super = ramfs_load_super,
};

void init_ramfs(void)
{
	add_fsdrive(&ramfs_drive);
}
