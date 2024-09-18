// asan支持，手动加一下这个：  /fsanitize=address /Zi 


#include <windows.h>
#include "pybind11/pybind11.h"

extern bool CustomIsDebuggerPresent()
{
	return !!::IsDebuggerPresent();
}
extern void CustomSleep(float Seconds)
{
	::Sleep((int)(Seconds * 1000));
}

extern void WaitForDebugger()
{
	pybind11::print("attach to debug...");
	while (!CustomIsDebuggerPresent())
	{
		CustomSleep(0.01f);
	}
	__debugbreak();
}