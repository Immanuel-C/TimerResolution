#include "timer_helpers.h"
#include <stdio.h>
#include <tlhelp32.h>
#include <winternl.h>

NTSYSAPI NTSTATUS NTAPI NtQueryTimerResolution(PULONG MinimumResolution, PULONG MaximumResolution, PULONG CurrentResolution);
NTSYSAPI NTSTATUS NTAPI NtSetTimerResolution(ULONG DesiredResolution, BOOLEAN SetResolution, PULONG CurrentResolution);

void set_global_timer_resolution_requests(DWORD val) {
    HKEY key;
    LRESULT result = 0;

    result = RegCreateKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\Session Manager\\kernel", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &key, NULL);
    if (result != ERROR_SUCCESS) {
        char info[512] = { 0 };
        sprintf(info, "RegCreateKeyEx failed. Are you running this app with admin privlages?\nError Code: %lld", result);
        MessageBoxA(NULL, info, "Error!", MB_OK);
        return;
    }

    result = RegSetValueExA(key, "GlobalTimerResolutionRequests", 0, REG_DWORD, (LPBYTE)&val, sizeof(DWORD));
    if (result != ERROR_SUCCESS) {
        MessageBoxA(NULL, "RegSetValueExA failed", "Error!", MB_OK);
        return;
    }

    RegCloseKey(key);
}

timer_resolution_values query_global_timer_reslution() {
    ULONG minResolution, maxResolution, currResolution;

    if (NtQueryTimerResolution(&minResolution, &maxResolution, &currResolution)) {
        MessageBoxA(NULL, "NtQueryTimerResolution failed!", "Error!", MB_OK | MB_ICONERROR);
        timer_resolution_values failed = { 0, 0, 0 };
        return failed;
    }

    timer_resolution_values values = {
        .minResolution  = (uint32_t)minResolution,
        .maxResolution  = (uint32_t)maxResolution,
        .currResolution = (uint32_t)currResolution
    };

    return values;
}

void set_global_timer_reslution(uint32_t resolution_micro_seconds) {
    if (resolution_micro_seconds < 0) {
        MessageBoxA(NULL, "set_global_timer_reslution(uint32_t resolution) requires that parameter reslution is not negative!", "Error!", MB_OK | MB_ICONERROR);
        return;
    }

    // Tell windows always honour timer resolution changes this process makes
    PROCESS_POWER_THROTTLING_STATE powerThrottling = { 0 };
    powerThrottling.Version = PROCESS_POWER_THROTTLING_CURRENT_VERSION;
    powerThrottling.ControlMask = PROCESS_POWER_THROTTLING_IGNORE_TIMER_RESOLUTION;
    powerThrottling.StateMask = 0;

    SetProcessInformation(GetCurrentProcess(), ProcessPowerThrottling, &powerThrottling, sizeof(PROCESS_POWER_THROTTLING_STATE));

    ULONG current_resolution = 0;

    if (NtSetTimerResolution(resolution_micro_seconds, TRUE, &current_resolution)) {
        MessageBoxA(NULL, "NtSetTimerResolution failed!", "Error!", MB_OK | MB_ICONERROR);
        return;
    }

    timer_resolution_values values = query_global_timer_reslution();
    
    printf("Timer resolution set to: %.4f\n", ((double)values.currResolution / 10000.0));
}

uint32_t micro_adjust_timer_resolution(uint32_t start, uint32_t end, uint32_t increment, uint32_t samples) {

}
