/// @file   FLEX.h
/// @brief  A light-weight black-boarding communication protocol for embedded platforms
///
///         Requires:
///         ===========
///         - cbuffer.h     
///
///     
///         Usage Notes:
///         ============
///         FLEX is ideal for use in situations where you would like to transfer a 'workspace' or collection of variables
///         from one controller to another. Variables whose values are update, i.e., are sent / received by the FLEX protocol
///         are defined within a structure called a FLEX-tree workspace.
///         \n
///         Variables are declared within the FLEX-tree workspace like so:
///         \n
///         FLEXTREE_BEGIN(TREE_TAG);
///             FLEXTREE_VAR(float      ,a_float);
///             FLEXTREE_VAR(float      ,more_floats[10]);
///             FLEXTREE_VAR(double     ,a_double);
///             FLEXTREE_VAR(char       ,a_char);
///             FLEXTREE_VAR(int        ,a_int);
///             FLEXTREE_VAR(long       ,a_long);
///         FLEXTREE_END(TREE_TAG);
///         \n
///         'TREE_TAG' is a unique name given to a FLEX-tree workspace. This tag should be unique for all FLEX workspaces
///         defined on a single system.
///         \n
///         When a variable is defined within a FLEX-tree, it is automatically updated from received data whenever
///         FLEXTREE_UPDATE(TREE_TAG) is called, where tag is the name. Variables within the FLEX-tree can be accessed using the 
///         macro FLEXTREE_GETVAR(TREE_TAG,VAR_NAME) where 'VAR_NAME' is the name of a variable registered within the FLEX-tree
///         given the tag 'TREE_TAG'. You can use FLEXTREE_GETVAR(TREE_TAG,VAR_NAME) you can manipulate the return value of 
///         FLEXTREE_GETVAR(TREE_TAG,VAR_NAME) as if it was a regular scoped variable. For example, the following operations
///         can be performed:
///             + FLEXTREE_GETVAR(TREE_TAG,a_float) = 0.0f;  
///             + FLEXTREE_GETVAR(TREE_TAG,more_floats)[10] = 9.232f;  
///         \n
///         If a controller is transmitting data to another controller, then the receiving controller must have an identical
///         (name and order) FLEX-tree defined in its source. This way, variables that are updated on the controller with authority 
///         over said variables can be referenced in a remote workspace by the same name, as if the controllers were sharing a
///         unified same memory.
///         \n
///         Currently, FLEX uses a simple 8-bit checksum to check packet viability.
///         \n
///         FLEX is merely a packeting protocol. All hardware handling (interrupt callbacks, etc.) must be defined elsewhere.
///         For convenience FLEXTREE_RXBYTE(TREE_TAG) can be called from within a 
///
/// @defgroup FLEXTypedefs
/// @defgroup FLEXBase
/// @defgroup FLEX_UserAPI
#ifndef     FLEX_H
#define     FLEX_H
#include    "stdint.h"
#include    "cbuffer.h"



/// @ingroup FLEXTypedefs
/// @{


    /// @brief  Function pointer type for single-byte transmit callback
    ///         [INTERNAL - DO NOT USE ; Use FLEX_UserAPI only!]
    typedef void(*FLEXTXService)   (uint8_t);


    /// @brief  FLEX-Tree status structure
    ///         [INTERNAL - DO NOT USE ; Use FLEX_UserAPI only!]
    typedef struct _FLEXRXStat
    {
        uint16_t    head_itr;
        uint16_t    data_itr;
        uint16_t    tail_itr;
        uint8_t     checksum;
        uint8_t     checkbuf[2048UL];
    } FLEXRXStat_t;


    /// @brief  Sets zeros to all iterators within a variable of type FLEXRXStat_t
    ///         [INTERNAL - DO NOT USE ; Use FLEX_UserAPI only!]
    #define FLEXRXSTAT_RESET(stat_p)                                                            \
        stat_p->head_itr    =                                                                   \
        stat_p->tail_itr    =                                                                   \
        stat_p->data_itr    =                                                                   \
        stat_p->checksum    = 0;


    /// @brief  Copies user data into into the FLEXRXStat_t for checksum processing
    ///         [INTERNAL - DO NOT USE ; Use FLEX_UserAPI only!]
    #define FLEXRXSTAT_COPY(stat_p,userp)                                                       \
        while(stat_p->data_itr--)                                                               \
            userp[stat_p->data_itr] = stat_p->checkbuf[stat_p->data_itr]


/// @}





/// @ingroup FLEXBase
/// @{

    /// @brief  Transmits a FLEXTree 
    ///         [INTERNAL - DO NOT USE ; Use FLEX_UserAPI only!]
    void    FLEXBASE_SendTree( uint8_t userp[], uint16_t size, FLEXTXService _TX );
    
    /// @brief  Checks recieved data and updates the FLEXTree accordingly.
    ///         [INTERNAL - DO NOT USE ; Use FLEX_UserAPI only!]
    uint8_t FLEXBASE_UpdateTree( uint8_t userp[], uint16_t size, cbuffer* buf_p, FLEXRXStat_t* _RXStat );

/// @}






/// @ingroup FLEX_UserAPI
/// @{


    /// @ brief defines the start of a FLEXTree structure
    #define FLEXTREE_BEGIN(tag)                                                                 \
        static FLEXRXStat_t     FLEXDEF_RecvSupports_##tag          = {0,0,0,0};                \
        static cbuffer          FLEXDEF_Buffer_##tag;                                           \
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
        buffer_init(&FLEXDEF_Buffer_##tag,size);                                                \
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
        &   FLEXDEF_Buffer_##tag,                                                               \
       EF   FLEXDef_RecvSupports_##tag                                                          \
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




    /// @brief Sets byte to relieve buffer
    #define FLEXTREE_RXBYTE(tag,byte)                                                           \        
        buffer_put(&FLEXDef_Buffer_##tag,byte)




    /// @brief Gets the status of the FLEXTree. Returns '1' is all is well
    #define FLEXTREE_OK(tag)                                                                    \        
        BUFFER_OK(&FLEXDef_Buffer_##tag)


/// @}

#endif