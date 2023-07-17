#pragma once
class noncopyable
{
protected:
    __inline noncopyable() {}
    __inline ~noncopyable() {}

private:
    __inline noncopyable(const noncopyable&) {}
    __inline noncopyable& operator = (const noncopyable&) {}
};
