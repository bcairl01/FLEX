#ifndef     FLEX_H
#define     FLEX_H
#include    "stdint.h"


/// @ingroup flexTypedefs
/// @{

    typedef void(*flexTXService)    (uint8_t);
    typedef uint8_t(*flexRXService) (void);

    /// @brief  Generic Ring-Buffer Structure
    /// @var    rd_idx          next slot in buffer to read from
    /// @var    wr_idx          next slot in buffer to write to
    /// @var    size            number of unread bytes in buffer
    /// @var    data            buffer stack space
    /// @var    overflow        buffer-overflow flag
    typedef struct {
        uint16_t                rd_idx;
        uint16_t                wr_idx;
        uint16_t                size;
        uint8_t                 data[2048UL];
        uint8_t                 flags;
    } flexBuffer_t;

/// @}




/// @ingroup flexAPI
/// @{

/// @ brief defines the start of a flex-tree structure
#define FLEXTREE_BEGIN(tag)                                             \
    static union  __flexdef_Tree_##tag                                  \
    {                                                                   \
        struct __flexdef_Struct_##tag                                   \
        {




/// @brief Defines the end of a flexTree structure
#define FLEXTREE_END(tag)                                               \
        } flexdef_Struct_##tag;                                         \
        uint8_t flexdef_cbuf[sizeof(struct __flexdef_Struct_##tag)];    \
    } flexdef_Tree_##tag;                                               \




/// @brief Adds a variable to the flexTree
#define FLEXTREE_VAR(_ty,varname)                                       \
        _ty varname;




/// @brief Get a variable from the flexTree
#define FLEXTREE_GETVAR(tag,varname)                                    \
    (flexdef_Tree_##tag.flexdef_Struct_##tag).varname




/// @brief Gets the size of the flexTree in bytes
#define FLEXTREE_GETSIZE(tag)                                           \
    sizeof(flexdef_Tree_##tag.__flexdef_Struct_##tag)



/// @brief Gets the current output buffer corresponding to the target flexTree
#define FLEXTREE_GETBUG(tag)                                            \
    flexdef_Tree_##tag.flexdef_cbuf

/// @}

#endif