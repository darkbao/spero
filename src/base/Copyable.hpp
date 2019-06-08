#ifndef SPERO_BASE_COPYABLE_H
#define SPERO_BASE_COPYABLE_H

namespace spero
{

/*  A tag class emphasises the objects are Copyable.
    The empty base class optimization applies.
    Any derived class of Copyable should be a value type.  */
class Copyable
{
protected:
    Copyable() = default;
    ~Copyable() = default;
};

}  // namespace spero

#endif  // SPERO_BASE_COPYABLE_H
