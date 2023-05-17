#pragma once
#ifndef ELEMENTS_HOLDER_H
#define ELEMENTS_HOLDER_H

template <typename T>
class ElementsHolder
{
    virtual T Next() noexcept = 0;
    virtual bool HasNext() const noexcept = 0;
};

#endif // ELEMENTS_HOLDER_H