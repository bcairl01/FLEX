#ifndef     FLEX_H
#define     FLEX_H
#include    "stdint.h"


/// @ingroup flexTypedefs
/// @{

typedef void(*flexTXService)   (uint8_t);
typedef uint8_t(*flexRXService)(void);

#define FLEXBUFFER_CAP                      2048UL

/// @brief  Generic Ring-Buffer Structure
/// @var    rd_idx          next slot in buffer to read from
/// @var    wr_idx          next slot in buffer to write to
/// @var    size            number of unread bytes in buffer
/// @var    data            buffer stack space
/// @var    overflow        buffer-overflow flag
typedef struct {
    uint16_t                rd_idx;
    uint16_t                wr_idx;
    uint16_t                pr_idx;
    uint16_t                size;
    uint8_t                 flags;
    uint8_t                 data[FLEXBUFFER_CAP];
} flexBuffer_t;
#define FLEXBUFFER_INITIALIZER              {0,0,0,0}
#define FLEXBUFFER_RDPP(buf_p)              if(buf_p->size > 0UL           ) { --buf_p->size; ++buf_p->rd_idx; buf_p->rd_idx%=FLEXBUFFER_CAP; }
#define FLEXBUFFER_WRPP(buf_p)              if(buf_p->size < FLEXBUFFER_CAP) { ++buf_p->size; ++buf_p->wr_idx; buf_p->wr_idx%=FLEXBUFFER_CAP; }
#define FLEXBUFFER_SIZE(buf_p)              buf_p->size
#define FLEXBUFFER_RD_AT(buf_p)             buf_p->data[buf_p->rd_idx]
#define FLEXBUFFER_WR_AT(buf_p)             buf_p->data[buf_p->wr_idx]
/// @}



/// @ingroup flexRXStat
/// @{
typedef struct _flexRXStat
{
    uint16_t    head_itr;
    uint16_t    data_itr;
    uint16_t    tail_itr;
    uint8_t     checksum;
    uint8_t     temp_buf[2048];
} flexRXStat_t;

#define FLEXRXSTAT_RESET(stat_p)                                                            \
    stat_p->head_itr    =                                                                   \
    stat_p->tail_itr    =                                                                   \
    stat_p->data_itr    =                                                                   \
    stat_p->checksum    = 0;

#define FLEXRXSTAT_COPY(stat_p,userp)                                                       \
    while(stat_p->data_itr--)                                                               \
        userp[stat_p->data_itr] = stat_p->temp_buf[stat_p->data_itr]
/// @}



/// @ingroup flexBase
/// @{
void    FLEXBASE_SendTree( uint8_t userp[], uint16_t size, flexTXService _TX );
uint8_t FLEXBASE_RecvTree( uint8_t userp[], uint16_t size, flexRXService _RX, flexRXStat_t* _RXStat );


/// @}






/// @ingroup flexAPI
/// @{


/// @ brief defines the start of a flexTree structure
#define FLEXTREE_BEGIN(tag)                                                                 \
    static flexRXStat_t     flexDef_RecvSupports_##tag          = {0,0,0,0};                \
    static union  __flexdef_Tree_##tag                                                      \
    {                                                                                       \
        struct __flexdef_Struct_##tag                                                       \
        {




/// @brief Defines the end of a flexTree structure
#define FLEXTREE_END(tag)                                                                   \
        } flexdef_Struct_##tag;                                                             \
        uint8_t flexdef_cbuf[sizeof(struct __flexdef_Struct_##tag)];                        \
    } flexdef_Tree_##tag;                                                                   \




/// @brief Sends the flexTree using the provided tx-service
#define FLEXTREE_SEND(tag,tx_fn)                                                            \
    FLEXBASE_SendTree(                                                                      \
        flexdef_Tree_##tag.flexdef_cbuf,                                                    \
        sizeof(flexdef_Tree_##tag.flexdef_Struct_##tag),                                    \
        tx_fn                                                                               \
    )



/// @brief Sends the flexTree using the provided rx-service
#define FLEXTREE_RECV(tag,rx_fn)                                                            \
    FLEXBASE_RecvTree(                                                                      \
        flexdef_Tree_##tag.flexdef_cbuf,                                                    \
        sizeof(flexdef_Tree_##tag.flexdef_Struct_##tag),                                    \
        rx_fn,                                                                              \
    &   flexDef_RecvSupports_##tag                                                          \
    )




/// @brief Adds a variable to the flexTree
#define FLEXTREE_VAR(_ty,varname)                                                           \
        _ty varname;




/// @brief Get a variable from the flexTree
#define FLEXTREE_GETVAR(tag,varname)                                                        \
    (flexdef_Tree_##tag.flexdef_Struct_##tag).varname




/// @brief Gets the size of the flexTree in bytes
#define FLEXTREE_GETSIZE(tag)                                                               \
    sizeof(flexdef_Tree_##tag.flexdef_Struct_##tag)



/// @brief Gets the current output buffer corresponding to the target flexTree
#define FLEXTREE_GETBUF(tag)                                                                \
    flexdef_Tree_##tag.flexdef_cbuf



/// @brief Set a variable from a flexTree value
#define FLEXTREE_SETFROMVAR(tag,varname,this)                                               \
    this = (flexdef_Tree_##tag.flexdef_Struct_##tag).varname



/// @}

#endif