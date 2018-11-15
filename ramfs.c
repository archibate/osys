#include <vfs.h>
#include <mbr.h>
#include <kmalloc.h>
#include <errno.h>
#include <string.h>
#include <memory.h>
#include <panic.h>
#include <print.h>
#include <derive.h>
#include <struct.h>
#include <wchar.h>
#include <div.h>

// references: https://blog.csdn.net/dingbihu/article/details/51475351
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

static
int ramfs_read(FILE *f, char *buf, size_t size)
{
	SUPER *sb = f->f_inode->i_sb;


	int i = 0;
	while (1) {
		if (f->f_fat.fe_clus >= sb->s_fat.se_clusmax) {
			return -E_BAD_BLK;
		}
		off_t off = f->f_fat.fe_clus * sb->s_fat.se_clusiz + f->f_fat.fe_cloff;
		//printf("ramfs_read: clus=%x\n", f->f_fat.fe_clus);
		while (i < size && f->f_fat.fe_cloff < sb->s_fat.se_clusiz)
		{
			if (f->f_pos >= f->f_size)
				return i;
			i++, f->f_pos++, f->f_fat.fe_cloff++;
			*buf++ = sb->s_fat.se_data[off++];
		}
		if (i >= size)
			return i;
		f->f_fat.fe_clus = sb->s_fat.se_fat[f->f_fat.fe_clus];
		f->f_fat.fe_cloff = 0;
	}
}

static
void _ramfs_rewind(FILE *f)
{
	f->f_pos = 0;
	f->f_fat.fe_clus = f->f_inode->i_fat.ie_clus;
	f->f_fat.fe_cloff = 0;
}

static
int ramfs_seek(FILE *f, long offset, int whence)
{
	SUPER *sb = f->f_inode->i_sb;
	int res = SEEK_CUR;

	off_t maxoff = f->f_size - f->f_pos;

	switch (whence) {
	case SEEK_SET:
		_ramfs_rewind(f);
		break;
	case SEEK_END:
		offset = maxoff - offset;
		break;
	};

	if (offset > maxoff) {
		offset = maxoff;
		res = SEEK_END;
	} else if (offset < 0) {
		offset += f->f_pos;
		_ramfs_rewind(f);
		if (offset < 0) {
			offset = 0;
			res = SEEK_SET;
		}
	}

	int rest = sb->s_fat.se_clusiz - f->f_fat.fe_cloff;
	if (offset < rest) {
		f->f_fat.fe_cloff += offset;
		f->f_pos += offset;
	} else {
		div_t di = div(offset - rest, sb->s_fat.se_clusiz);
		while (di.quot-- > 0) {
			f->f_fat.fe_clus = sb->s_fat.se_fat[f->f_fat.fe_clus];
			if (f->f_fat.fe_clus >= sb->s_fat.se_clusmax)
				return -E_BAD_BLK;
			f->f_pos += sb->s_fat.se_clusiz;
		}
		f->f_fat.fe_cloff = di.rem;
		f->f_pos += di.rem;
	}

	return res;
}

/*static
int ramfs_tell(FILE *f, int what)
{
	return what == TELL_SIZ ? f->f_size : f->f_pos;
}*/

static
int ramfs_close(__attribute__((unused)) FILE *f)
{
	return 0;
}

static
int ramfs_closedir(__attribute__((unused)) DIR *d)
{
	return 0;
}

static
FILE_OPS ramfs_fops;
static
int ramfs_open(FILE *f, INODE *inode, unsigned int oattr)
{
	f->f_oattr = oattr;
	f->f_inode = inode;
	f->f_ops = &ramfs_fops;
	f->f_size = inode->i_size;

	_ramfs_rewind(f);
	return 0;
}

static
void ramfs_decode_fatents(SUPER *sb, DIR *dir, const FAT_DIRENT *fes, unsigned int ents);

static
DIR_OPS ramfs_dops;
static
int ramfs_opendir(DIR *dir, INODE *inode, unsigned int oattr)
{
	dir->d_oattr = oattr;
	dir->d_ops = &ramfs_dops;
	dir->d_inode = inode;

	char *buf = inode->i_sb->s_fat.se_data;
	buf += inode->i_fat.ie_clus * inode->i_sb->s_fat.se_clusiz;
	unsigned int ents = inode->i_sb->s_fat.se_clusiz / sizeof(FAT_DIRENT);
	ramfs_decode_fatents(inode->i_sb, dir, (FAT_DIRENT *) buf, ents);

	return 0;
}

static
FILE_OPS ramfs_fops = {
	.open = ramfs_open,
	.read = ramfs_read,
	.seek = ramfs_seek,
	//.tell = ramfs_tell,
	.close = ramfs_close,
};

static
DIR_OPS ramfs_dops = {
	.opendir = ramfs_opendir,
	.closedir = ramfs_closedir,
};

////

static
INODE_OPS ramfs_iops = {
};

static
INODE *ramfs_alloc_inode(SUPER *sb)
{
	INODE *inode = kmalloc_for(INODE);
	inode->i_sb = sb;
	inode->i_ops = &ramfs_iops;
	return inode;
}

static
void ramfs_free_inode(INODE *inode)
{
	kfree(inode);
}

static
void ramfs_free_super(SUPER *sb)
{
	for (LIST *le = sb->s_root->d_ents; le;) {
		DIRENT *de = list_entry(DIRENT, e_list, le);
		le = le->next;
		free_inode(de->e_inode);
		kfree(de);
	}
	kfree(sb->s_root);
	kfree(sb);
}

static
SUPER_OPS ramfs_super_ops = {
	.free_super = ramfs_free_super,
	.alloc_inode = ramfs_alloc_inode,
	.free_inode = ramfs_free_inode,
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
void copy_fate_to_inode(const FAT_DIRENT *fe, INODE *inode)
{
	inode->i_fat.ie_attr = fe->e_attr;

	inode->i_attr |= INODE_RD;
	if (!(fe->e_attr & FAT_RDONLY))
		inode->i_attr |= INODE_WR;

	if (fe->e_attr & FAT_ARCHIV)
		inode->i_type = INODE_FIL;
	if (fe->e_attr & FAT_SUBDIR)
		inode->i_type = INODE_DIR;

	inode->i_size = fe->e_size;

	inode->i_fat.ie_clus = fe->e_clus_hi;
       	inode->i_fat.ie_clus <<= 16;
	inode->i_fat.ie_clus += fe->e_clus_lo;
}

static
void ramfs_decode_fatents(SUPER *sb, DIR *dir, const FAT_DIRENT *fes, unsigned int ents)
{
	//printf("loading directory entries...(%d maxium) from %p\n", ents);

	int total = 0;
	unsigned char chksum = 0;

	DIRENT *de = kmalloc_for(DIRENT);

	dir->d_ents = 0;
	for (int i = ents - 1; i >= 0; i--)
	{
		const FAT_DIRENT *fe = &fes[i];
		if (fe->e_attr != FAT_LMAGIC && (fe->e_attr & FAT_VOLLAB)) {
			//printf("volume label: %11s\n", fe->e_name);
			continue;
		}

		if (get_fat_dirent_name(de->e_name, fe)) {
			total++;

			chksum = get_short_name_chksum(fe->e_name);
			char *p = de->e_name;
			const FAT_DIRENT *nfe = fe;
			while (i > 0 && fes[i - 1].e_attr == FAT_LMAGIC) {
				nfe = &fes[--i];
				if (chksum != nfe->l_chksum) {
					get_fat_dirent_name(de->e_name, fe);
					break;
				}
				p = append_fat_long_name(p, nfe);
				*p = 0;
			}
			//printf("loaded entry: %s\n", de->e_name);

			list_add_head_n(&dir->d_ents, &de->e_list);

			de->e_inode = ramfs_alloc_inode(sb);
			copy_fate_to_inode(fe, de->e_inode);

			if (de->e_inode->i_type == INODE_DIR)
				de->e_inode->i_dops = &ramfs_dops;
			else
				de->e_inode->i_fops = &ramfs_fops;

			de = kmalloc_for(DIRENT);
			p = de->e_name;
			fe = nfe;

			continue;
		}
	}
	de->e_list.prev->next = 0;

	kfree(de);

	//printf("%d entries loaded\n", total);
}

STRUCT(RAMFS_LOADING)
{
	SUPER *sb;

	unsigned char *data;
	MBR_HEADER *mbr;

	unsigned char *fat12;
	unsigned int fat_ents;

	FAT_DIRENT *rootdir;
	unsigned int rootdir_ents;
};

static
void ramfs_decode_mbr(RAMFS_LOADING *es)
{
	FAT_SUPER_EX *s_fat = &es->sb->s_fat;
#define FAQ(x, y) ((x)      = (typeof(x)) (y)) // LETHER_MAN!!
	FAQ(es->mbr         , es->data);
	unsigned int bps    = es->mbr->bpb_bytes_per_sec;
	s_fat->se_clusiz    = es->mbr->bpb_sec_per_clus * bps;
	FAQ(es->fat12       , es->data + bps * es->mbr->bpb_rsvd_secs);
	FAQ(es->rootdir     , es->data + bps *
			    ( es->mbr->bpb_rsvd_secs
			    + es->mbr->bpb_fat_size16 * es->mbr->bpb_num_fats
			    //+ es->mbr->bpb_hidd_secs
			    ));
	FAQ(s_fat->se_data  , (char *) es->rootdir
			    + es->mbr->bpb_root_ents * sizeof(FAT_DIRENT)
			    - 2 * s_fat->se_clusiz
			    //+ bps * es->mbr->bpb_hidd_secs
			    );
	s_fat->se_clusmax   = 0xff7;
	es->fat_ents        = (bps * es->mbr->bpb_fat_size16 * 2) / 3;
	es->rootdir_ents    = es->mbr->bpb_root_ents;

	printf("ramfs: super block info in mbr:\n");
	printf("  VOL NAME  %.11s\n", es->mbr->bs_vol_label);
	printf("  VOL ID    0x%8X\n", es->mbr->bs_vol_number);
	printf("  OEM NAME  %.8s\n", es->mbr->bs_oem_name);
	printf("  FAT ENTS  %d\n", es->fat_ents);
	printf("  ROOT MAX  %d\n", es->rootdir_ents);
	printf("  CLU SIZE  %d\n", s_fat->se_clusiz);
}

// reference: https://github.com/archibate/OSASK/blob/master/src/haribote/file.c
static
void uncompress_fat12(clus_t *fat, const unsigned char *img, unsigned int ents)
{
	for (int i = 0, j = 0; i < ents; i += 2, j += 3) {
		fat[i+0] = 0xfff & (( img[j+0]     )|( img[j+1] <<8 ));
		fat[i+1] = 0xfff & (( img[j+1] >>4 )|( img[j+2] <<4 ));
	}
}

static
SUPER *ramfs_load_super(void *ramdisk)
{
	SUPER *sb = kmalloc_for(SUPER);
	RAMFS_LOADING *es = kmalloc_for(RAMFS_LOADING);

	es->sb = sb;
	es->data = (unsigned char *) ramdisk;

	es->sb->s_inode = 0;
	es->sb->s_root = kmalloc_for(DIR);

	ramfs_decode_mbr(es);
	ramfs_decode_fatents(es->sb, es->sb->s_root, es->rootdir, es->rootdir_ents);

	es->sb->s_fat.se_fat = kmalloc((es->fat_ents + 1) * sizeof(clus_t));
	uncompress_fat12(es->sb->s_fat.se_fat, es->fat12, es->fat_ents);

	es->sb->s_ops = &ramfs_super_ops;

	kfree(es);

	return sb;
}

static
FSDRIVE ramfs_drive = {
	.name = "ramfs",
	.load_super = ramfs_load_super,
};

static
SUPER *rd_sb;

void init_ramfs(void)
{
	add_fsdrive(&ramfs_drive);

	extern char ramdisk[];
	rd_sb = load_super("ramfs", ramdisk);
	vfs_setroot(rd_sb->s_root);
}
