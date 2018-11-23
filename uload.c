#include <vfs.h>
#include <print.h>
#include <panic.h>
#include <kmalloc.h>
#include <umemlay.h>
#include <uload.h>
#include <sched.h>
#include <schpcb.h>
#include <map.h>
#include <psm.h>
#include <mmu.h>
#include <memory.h>
#include <exf.h>

#define uload_map_psm_page(addr) map((addr), alloc_ppage() | PG_PSM | PG_P | PG_W | PG_U)
#define uload_unmap_psm_page(addr) free_ppage(unmap(addr))

int exec_user_program_fc(FILE *f)
{
	void *addr = (void *) USER_CODE_BEG;

	assert((ssize_t) f->f_size > 0);

	EXF_HEADER exfhdr;

	int res = read(f, (void*)&exfhdr, sizeof(exfhdr));
	assert(exfhdr.x_magic == EXF_MAGIC);
	assert(res == sizeof(exfhdr));

	user_space_destroy(); // destroy current proc

	for (size_t off = 0; off < exfhdr.x_size; off += PGSIZE)
		uload_map_psm_page((unsigned long)addr + off);

	mmu_set_pd(mmu_get_pd());

	res = seek(f, exfhdr.x_off, SEEK_SET);
	assert(!res);
	res = read(f, addr, exfhdr.x_size);
	if (res > 0)
		res = 0;

	close(f);
	kfree(f);

	unsigned long stkbtm = USER_STACK_END;
	for (int i = 0; i < exfhdr.x_stkpgs + 1; i++) // todo: move this to page_fault
		uload_map_psm_page(stkbtm -= PGSIZE);

	mmu_set_pd(mmu_get_pd());

	printf("shcmd[0]:%p\n", *(int*)0x10005a80);

	return res;
}


void user_space_destroy(void)
{
	unmap_free_psm_pages(USER_CODE_BEG,   USER_CODE_END);
	unmap_free_psm_pages(USER_STACK_BEG, USER_STACK_END);
	unmap_free_psm_pages(USER_MMAP_BEG,   USER_MMAP_END);
}
