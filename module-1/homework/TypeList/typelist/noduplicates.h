#pragma once

#include "erase.h"
#include "typelist.h"

template<typename TList>
struct NoDuplicates;

template<>
struct NoDuplicates<NullType> {
    typedef NullType NewTypeList;
};

template<typename Head, typename Tail>
struct NoDuplicates<TypeList<Head, Tail>> {
    typedef TypeList<Head, typename Erase<typename NoDuplicates<Tail>::NewTypeList, Head>::NewTypeList> NewTypeList;
};
