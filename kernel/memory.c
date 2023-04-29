#include "memory.h"
#include "stdint.h"
#include "baselib.h"
#include "stddef.h"

#define MAX_ADDR *(uint32_t *)((void *)&max_addr + PAGE_OFFSET) // max_addr остался в загрузочной части
#define PHISYCAL_EOC ((uint32_t)&end_of_code - PAGE_OFFSET)

struct s_page {
	uint32_t	flags;
	uint32_t	count;
	uint32_t	index;
	void		*virtual;
};

static unsigned char	*bitmask = &end_of_code;
static uint32_t			bm_size;
static uint32_t			*tables;
static void				*heap_start;
static void				*heap_end; // следующий за последним валидным адресом

unsigned char* get_bitmask()
{
	return (bitmask);
}

uint32_t get_bitmask_size()
{
	return (bm_size);
}

uint32_t align(uint32_t addr)
{
	return (((addr + (PAGE_SIZE - 1)) / PAGE_SIZE) * PAGE_SIZE);
}

static void intital_map()
{
	uint32_t *pde = (uint32_t *)0xFFFFF000;
	uint32_t frames = 0;

	for (int i = 768; i < 896; i++, tables += 1024) { // 0x0 - 0x20000000 : 0xC000000 - 0xE0000000
		for (int j = 0; j < 1024; j++, frames += PAGE_SIZE) {
			tables[j] = frames | 3;
		}
		pde[i] = ((uint32_t)tables - PAGE_OFFSET) | 3; // физические адреса там хранятся
	}
	refresh_map();
}

void memory_init()
{
	/*
		На начало у нас смаплена 1 а может или 2 страницы для загрузки,
		нам надо смапить до конца кучи 0xE0000000
		всю физическую память
		тогда доступ к физической будет
		физический адрес = виртуальлный - PAGE_OFFSET
	*/

	uint32_t frames_in_use;

	// размер доступной физической памяти из загрузчика
	printf("Size of ph. memory (page aligned) = %x\n", align(MAX_ADDR));
	// скольйо байт нужно для описания размера доступной физической памяти
	bm_size = align(MAX_ADDR) / PAGE_SIZE / 8;
	printf("Size of bit bitmask array %u bytes\n", bm_size);
	// сколько физической памяти занято код ядра + битовая маска
	frames_in_use = PHISYCAL_EOC / PAGE_SIZE + align(bm_size) / PAGE_SIZE;
	// начало таблиц для трансляии 512M (128 таблиц)
	tables = (uint32_t *)(frames_in_use * PAGE_SIZE + PAGE_OFFSET);
	// тут начнется куча
	heap_start = (void *)((frames_in_use + 128) * PAGE_SIZE + PAGE_OFFSET);
	heap_end = (void *)0xE0000000; // тут куча закончится (sbrk будет это менять ?)
	intital_map();
	// помечаем занятые фреймы
	bzero(bitmask, bm_size);
	for (uint32_t i = 0; i < frames_in_use + 128 ; i++) {
		bitmask[i / 8] = bitmask[i / 8] | (1 << (i % 8));
	}
}
