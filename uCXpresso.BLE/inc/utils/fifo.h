#ifndef __FIFO_H__
#define __FIFO_H__

#include "uCXpresso.h"

#ifdef __cplusplus
extern "C" {
#endif

/**@brief A FIFO instance structure. Keeps track of which bytes to read and write next.
 *        Also it keeps the information about which memory is allocated for the buffer
 *        and its size. This needs to be initialized by app_fifo_init() before use.
 */
typedef struct
{
    uint8_t *          p_buf;
    uint16_t           buf_size_mask;
    volatile uint32_t  read_pos;
    volatile uint32_t  write_pos;
} fifo_t;

/**@brief Function for initializing the FIFO.
 *
 * @param[out] p_fifo   FIFO object.
 * @param[in]  p_buf    FIFO buffer for storing data. The buffer size has to be a power of two.
 * @param[in]  buf_size Size of the FIFO buffer provided, has to be a power of 2.
 *
 * @retval     true		If initialization was successful.
 * @retval     false   	If a NULL pointer is provided as buffer or size of buffer provided is not a power of two.
 */
bool fifo_init(fifo_t * p_fifo, uint8_t * p_buf, uint16_t buf_size);

bool fifo_put(fifo_t * p_fifo, uint8_t byte);

bool fifo_get(fifo_t * p_fifo, uint8_t * p_byte);

bool fifo_flush(fifo_t * p_fifo);

int fifo_avail(fifo_t * p_fifo);

int fifo_free(fifo_t * p_fifo);

int fifo_get_block(fifo_t *p_fifo, uint8_t *buf, int size);

int fifo_put_block(fifo_t *p_fifo, uint8_t *buf, int size);

#ifdef __cplusplus
}
#endif
#endif // APP_FIFO_H__

/** @} */
