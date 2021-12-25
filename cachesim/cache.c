#include "common.h"
#include <inttypes.h>

void mem_read(uintptr_t block_num, uint8_t *buf);
void mem_write(uintptr_t block_num, const uint8_t *buf);

static uint64_t cycle_cnt = 0;

void cycle_increase(int n) { cycle_cnt += n; }

// TODO: implement the following functions
struct cache_line {
  int dirty;
  uint64_t mark;
  uint8_t block[BLOCK_SIZE];
};

struct cache_line* cache;
uint64_t cache_line_size, asso_size;
uint32_t asso_width, asso_num_width;
#define BLOCK_NUM(addr) addr >> BLOCK_WIDTH
#define BLOCK_BACK(mark, cache_num) (mark << asso_num_width) | (cache_num >> asso_width)
#define MARK(addr) addr >> (asso_num_width + BLOCK_WIDTH)
#define ASSO_NUM(addr) (addr & ((mask_with_len(asso_num_width) << BLOCK_WIDTH))) >> BLOCK_WIDTH
#define ASSO_BEGIN(addr) ASSO_NUM(addr) << asso_width
#define BLOCK_OFFSET(addr) (uint64_t)(addr & mask_with_len(BLOCK_WIDTH))

uint32_t cache_read(uintptr_t addr) {
  uint64_t mark = MARK(addr);
  uint64_t asso_begin = ASSO_BEGIN(addr);
  for (uint64_t i = asso_begin; i < asso_begin + asso_size; i++) {
    if (cache[i].dirty != -1 && cache[i].mark == mark) {
      return *(uint32_t *)(cache[i].block + BLOCK_OFFSET(addr));
    }
  }
  for (uint64_t i = asso_begin; i < asso_begin + asso_size; i++) {
    if (cache[i].dirty == -1) {
      cache[i].dirty = 0;
      cache[i].mark = mark;
      mem_read(BLOCK_NUM(addr), cache[i].block);
      return *(uint32_t *)(cache[i].block + BLOCK_OFFSET(addr));
    }
  }
  uint64_t cache_num = asso_begin + rand() % asso_size;
  if (cache[cache_num].dirty) mem_write(BLOCK_BACK(cache[cache_num].mark, cache_num), cache[cache_num].block);
  cache[cache_num].mark = mark;
  cache[cache_num].dirty = 0;
  mem_read(BLOCK_NUM(addr), cache[cache_num].block);
  return *(uint32_t *)(cache[cache_num].block + BLOCK_OFFSET(addr));
}

void cache_write(uintptr_t addr, uint32_t data, uint32_t wmask) {
  uint64_t mark = MARK(addr);
  uint64_t asso_begin = ASSO_BEGIN(addr);
  for (uint64_t i = asso_begin; i < asso_begin + asso_size; i++) {
    if (cache[i].dirty != -1 && cache[i].mark == mark) {
      *(uint32_t *)(cache[i].block + BLOCK_OFFSET(addr)) = (data & wmask) | (~wmask & *(uint32_t *)(cache[i].block + BLOCK_OFFSET(addr)));
      cache[i].dirty = 1;
      return;
    }
  }
  for (uint64_t i = asso_begin; i < asso_begin + asso_size; i++) {
    if (cache[i].dirty == -1) {
      cache[i].mark = mark;
      mem_read(BLOCK_NUM(addr), cache[i].block);
      *(uint32_t *)(cache[i].block + BLOCK_OFFSET(addr)) = (data & wmask) | (~wmask & *(uint32_t *)(cache[i].block + BLOCK_OFFSET(addr)));
      cache[i].dirty = 1;
      return;
    }
  }
  uint64_t cache_num = asso_begin + rand() % asso_size;
  if (cache[cache_num].dirty) mem_write(BLOCK_BACK(cache[cache_num].mark, cache_num), cache[cache_num].block);
  cache[cache_num].mark = mark;
  cache[cache_num].dirty = 1;
  mem_read(BLOCK_NUM(addr), cache[cache_num].block);
  *(uint32_t *)(cache[cache_num].block + BLOCK_OFFSET(addr)) = (data & wmask) | (~wmask & *(uint32_t *)(cache[cache_num].block + BLOCK_OFFSET(addr)));
}

void init_cache(int total_size_width, int associativity_width) {
  cache_line_size = 1 << (total_size_width - BLOCK_WIDTH);
  asso_width = associativity_width;
  asso_size = 1 << asso_width;
  asso_num_width = total_size_width - BLOCK_SIZE - associativity_width;

  cache = (struct cache_line *)malloc(cache_line_size * sizeof(struct cache_line));
  for (uint64_t i = 0; i < cache_line_size; i++) {
    cache[i].dirty = -1;
  }
}

void display_statistic(void) {
}
