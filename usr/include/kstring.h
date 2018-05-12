/* copy (ptr * 16) -aligned data (faster than memcpy ) */
void* kmemcpy_ptr(void *dest, const void *src, size_t len);
