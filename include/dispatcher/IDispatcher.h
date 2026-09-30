#pragma once

template <typename T> class IDispatcher {

  public:
    virtual ~IDispatcher() = default;
    virtual bool dispatch(T& command) = 0;

};
