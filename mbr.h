#pragma once

#include <struct.h>

STRUCT(MBR_HEADER)
{
	char bs_jmp_code[3];			// 启动扇区的 jmp entry; nop
	char bs_oem_name[8];			// 启动区的名称（8字节）
	unsigned short bpb_bytes_per_sec;	// 每个扇区（sector）的大小
	unsigned char bpb_sec_per_clus;		// 簇（cluster）的大小
	unsigned short bpb_rsvd_secs;		// FAT的起始位置
	unsigned char bpb_num_fats;		// FAT的个数
	unsigned short bpb_root_ents;		// 根目录的大小
	unsigned short bpb_tot_sec16;		// 该磁盘的大小
	unsigned char bpb_media;		// 磁盘的种类
	unsigned short bpb_fat_size16;		// FAT的长度
	unsigned short bpb_sec_per_track;	// 1个磁道（track）有几个扇区
	unsigned short bpb_num_heads;		// 磁头数
	unsigned int bpb_hidd_secs;		// 不使用分区数
	unsigned int bpb_tot_sec32;		// 重写一次磁盘大小
	unsigned char bs_drv_num;		// 磁盘在 BIOS 中的代号
	unsigned char bs_reserved1;		// 保留（必须为0）
	unsigned char bs_boot_sig;		// 启动签名（意义不明）
	unsigned int bs_vol_number;		// 磁盘的卷号码
	char bs_vol_label[11];			// 磁盘的卷标（11字节）
	char bs_fsys_type[8];			// 磁盘格式名称（8字节）
} __attribute__((packed));
