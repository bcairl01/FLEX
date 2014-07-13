#ifndef     FLEX_H
#define     FLEX_H
#include    "stdint.h"



typedef void(*flexTXService)    (uint8_t);
typedef uint8_t(*flexRXService) (void);


/// @ brief defines the start of a flex-tree structure
#define FLEXTREE_BEGIN(tag)                                             \
static union __flexdef_Tree_##tag                                       \
{                                                                       \
    struct __flexdef_Struct_##tag                                       \
    {



/// @brief Defines the end of a flexTree structure
#define FLEXTREE_END(tag)                                               \
    } flexdef_Struct_##tag;                                             \
    uint8_t flexdef_cbuf[sizeof(struct __flexdef_Struct_##tag)];        \
} flexdef_Tree_##tag;                                                   \
                                                                        \
static void flexTransmit_##tag( flexTXService _TX )                     \
{                                                                       \
    uint16_t size = sizeof(flexdef_Tree_##tag.flexdef_Struct_##tag);    \
    uint8_t* cbuf_ptr = (flexdef_Tree_##tag).flexdef_cbuf;              \
    while(size--)                                                       \
        _TX(*(cbuf_ptr++));                                             \
}



/// @brief Adds a variable to the flexTree
#define FLEXTREE_VAR(_ty,varname)                                       \
        _ty varname;



/// @brief Get a variable from the flexTree
#define FLEXTREE_GET(tag,varname)                                       \
    (flexdef_Tree_##tag.flexdef_Struct_##tag).varname
    

    
/// @brief Gets the size of the flexTree in bytes
#define FLEXTREE_SIZE(tag)                                              \
    sizeof(flexdef_Tree_##tag.__flexdef_Struct_##tag)


#endif