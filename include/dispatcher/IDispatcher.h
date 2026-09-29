#pragma once

template <typename T> class IDispatcher {

  public:
    ~IDispatcher() = default;
    virtual bool dispatch(T& command) = 0;

};
