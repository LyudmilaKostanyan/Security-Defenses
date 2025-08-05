#include <iostream>
#include <cstring>

#if defined(_WIN32)
    #include <windows.h>
#else
    #include <sys/mman.h>
    #include <unistd.h>
#endif

// x86_64: mov eax, 42; ret
unsigned char code[] = {
    0xB8, 0x2A, 0x00, 0x00, 0x00, // mov eax, 42
    0xC3                          // ret
};

int main() {
    void* exec_mem = nullptr;
    size_t code_size = sizeof(code);

#if defined(_WIN32)
    exec_mem = VirtualAlloc(nullptr, code_size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (!exec_mem) {
        std::cerr << "VirtualAlloc failed" << std::endl;
        return 1;
    }
#else
    size_t page_size = sysconf(_SC_PAGESIZE);
    exec_mem = mmap(nullptr, page_size, PROT_READ | PROT_WRITE | PROT_EXEC,
                    MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (exec_mem == MAP_FAILED) {
        perror("mmap");
        return 1;
    }
#endif

    std::memcpy(exec_mem, code, code_size);

    using func_t = int(*)();
    func_t func = reinterpret_cast<func_t>(exec_mem);

    std::cout << "Function pointer address: " << reinterpret_cast<void*>(func) << std::endl;

    int result = func();
    std::cout << "Executed result: " << result << std::endl;

#if defined(_WIN32)
    VirtualFree(exec_mem, 0, MEM_RELEASE);
#else
    munmap(exec_mem, page_size);
#endif

    return 0;
}
