#pragma once
template <class ValueType>
class SmartAllocator : public noncopyable
{
public:
    ~SmartAllocator()
    {
        for (const auto& item : alloca_list_)
        {
            if (item.first)
            {
                allocator_.deallocate(item.first, item.second);
            }
        }
    }

    ValueType* Allocetor(size_t count)
    {
        auto ptr = allocator_.allocate(count);
        if (ptr)
        {
            alloca_list_.insert({ ptr, count });
        }
        return ptr;
    }

    void Deallocate(const ValueType* ptr, size_t count)
    {
        allocator_.deallocate(ptr, count);
        alloca_list_.erase(ptr);
    }

private:
    std::allocator<ValueType> allocator_;
    std::unordered_map<ValueType*, size_t> alloca_list_;
};