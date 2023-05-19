#include "Application.h"
#include "Interface.h"
#undef APIENTRY
#include <Windows.h>

char* g_buffer = nullptr;
void* alloc(size_t size)
{
    if (!g_buffer)
    {
        g_buffer = (char*)VirtualAlloc((void*)0x1'000'000'000, 1024 * 1024 * 1024, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
        if (!g_buffer)
        {
            char* errorMsg;
            FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&errorMsg, 0, NULL);
            LocalFree(errorMsg);
        }
    }
    void* p = g_buffer;
    g_buffer += size;
    return p;
}

std::vector<void*>* g_memoryLeaks;
bool g_realloc = false;
size_t g_memoryUsage = 0;
void* operator new(size_t size)
{
    g_memoryUsage += size;
    void* p = alloc(size);
    if (!g_realloc)
    {
        g_realloc = true;
        if (!g_memoryLeaks)
        {
            g_memoryLeaks = new std::vector<void*>;
        }
        g_memoryLeaks->push_back(p);
        g_realloc = false;
    }
    return p;
}

void operator delete(void* p)
{
    std::vector<void*>::iterator old = std::find(g_memoryLeaks->begin(), g_memoryLeaks->end(), p);
    if (old != g_memoryLeaks->end())
    {
        g_memoryLeaks->erase(old);
    }
}

int main()
{
    {
        g_memoryLeaks->clear();
        Application app;
        Window& window = Application::GetWindow(); //Used as text replacement

        Interface myInterface(window.GetGLFWWindow(), window.GetGLSLVersion().c_str());

        while (!window.ShouldClose())
        {
            myInterface.BeginFrame();
            app.Run();
            myInterface.Draw();
            app.SwapFrames();
        }
    }
    std::cout << "end" << std::endl;
}