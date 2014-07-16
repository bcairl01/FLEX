#ifndef     FLEX_H
#define     FLEX_H
#include    "stdint.h"
#include    "cbuffer.h"


/// @ingroup FLEXTypedefs
/// @{

    typedef void(*FLEXTXService)   (uint8_t);


    typedef struct _FLEXRXStat
    {
        uint16_t    head_itr;
        uint16_t    data_itr;
        uint16_t    tail_itr;
        uint8_t     checksum;
        uint8_t     checkbuf[2048UL];
    } FLEXRXStat_t;


    #define FLEXRXSTAT_RESET(stat_p)                                                            \
        stat_p->head_itr    =                                                                   \
        stat_p->tail_itr    =                                                                   \
        stat_p->data_itr    =                                                                   \
        stat_p->checksum    = 0;


    #define FLEXRXSTAT_COPY(stat_p,userp)                                                       \
        while(stat_p->data_itr--)                                                               \
            userp[stat_p->data_itr] = stat_p->checkbuf[stat_p->data_itr]


/// @}





/// @ingroup FLEXBase
/// @{


    void    FLEXBASE_SendTree( uint8_t userp[], uint16_t size, FLEXTXService _TX );
    

    uint8_t FLEXBASE_UpdateTree( uint8_t userp[], uint16_t size, cbuffer* buf_p, FLEXRXStat_t* _RXStat );


/// @}






/// @ingroup FLEXAPI
/// @{


    /// @ brief defines the start of a FLEXTree structure
    #define FLEXTREE_BEGIN(tag)                                                                 \
        static FLEXRXStat_t     FLEXDef_RecvSupports_##tag          = {0,0,0,0};                \
        static cbuffer          FLEXDef_Buffer_##tag;                                           \
        static union  __FLEXDEF_Tree_##tag                                                      \
        {                                                                                       \
            struct __FLEXDEF_Struct_##tag                                                       \
            {




    /// @brief Defines the end of a FLEXTree structure
    #define FLEXTREE_END(tag)                                                                   \
            } FLEXDEF_Struct_##tag;                                                             \
            uint8_t FLEXDEF_cbuf[sizeof(struct __FLEXDEF_Struct_##tag)];                        \
        } FLEXDEF_Tree_##tag;                                                                   \




    /// @brief Initializes FLEXTree support structures. To be called AFTER a FLEXTree is defined
    #define FLEXTREE_SETUP(tag,size)                                                            \
        buffer_init(&FLEXDef_Buffer_##tag,size);                                                \
        buffer_alloc(&FLEXDef_Buffer_##tag,size);




    /// @brief Deletes FLEXTree support structures. To be called AFTER a FLEXTree is defined
    #define FLEXTREE_CLEAR(tag)                                                                 \
        buffer_free(&FLEXDef_Buffer_##tag)




    /// @brief Sends the FLEXTree using the provided tx-service
    #define FLEXTREE_SEND(tag,tx_handle)                                                        \
        FLEXBASE_SendTree(                                                                      \
            FLEXDEF_Tree_##tag.FLEXDEF_cbuf,                                                    \
            sizeof(FLEXDEF_Tree_##tag.FLEXDEF_Struct_##tag),                                    \
            tx_handle                                                                           \
        )




    /// @brief Updates the FLEXTree using data from its associated rx-buffer
    #define FLEXTREE_UPDATE(tag)                                                                \
        FLEXBASE_UpdateTree(                                                                    \
            FLEXDEF_Tree_##tag.FLEXDEF_cbuf,                                                    \
            sizeof(FLEXDEF_Tree_##tag.FLEXDEF_Struct_##tag),                                    \
        &   FLEXDef_Buffer_##tag,                                                               \
        &   FLEXDef_RecvSupports_##tag                                                          \
        )



    /// @brief Adds a variable to the FLEXTree
    #define FLEXTREE_VAR(_ty,varname)                                                           \
            _ty varname;




    /// @brief Get a variable from the FLEXTree
    #define FLEXTREE_GETVAR(tag,varname)                                                        \
        (FLEXDEF_Tree_##tag.FLEXDEF_Struct_##tag).varname




    /// @brief Gets the size of the FLEXTree in bytes
    #define FLEXTREE_GETSIZE(tag)                                                               \
        sizeof(FLEXDEF_Tree_##tag.FLEXDEF_Struct_##tag)




    /// @brief Sets byte to recieve buffer
    #define FLEXTREE_RXBYTE(tag,byte)                                                           \
        buffer_put(&FLEXDef_Buffer_##tag,byte)




    /// @brief Gets the status of the FLEXTree. Returns '1' is all is well
    #define FLEXTREE_OK(tag)                                                                    \
        BUFFER_OK(&FLEXDef_Buffer_##tag)


/// @}

#endif