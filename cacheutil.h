#ifndef __CACHEUTIL_H__
#define __CACHEUTIL_H__ 

typedef struct op_result
{
    optype: OpType,
    result: Lookup, // Hit, clean miss, dirty miss.
    addr: Addr,     // Unpacked address (index, tag).
    line_num: usize,
    valid_bit: u8,
    dirty_bit: u8,
    prev_tag: Option<u32>,     // Only present if line was valid.
    prev_mtime: Option<usize>, // Only present if E > 1.
}op_result_t;

#endif