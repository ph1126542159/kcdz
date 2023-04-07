#ifndef BufferAllocator___H
#define BufferAllocator___H
#include <ios>
#include <cstddef>
namespace PH {
template<typename ch>
class BufferAllocator {
public:
    typedef ch char_type;

    static char_type* allocate(std::streamsize nSize) {
        return new char_type[static_cast<std::size_t>(nSize)];
    }

    static void deallocate(char_type* ptr, std::streamsize) noexcept {
        delete[] ptr;
    }
};
}//end namespace PH
#endif // !BufferAllocator___H
