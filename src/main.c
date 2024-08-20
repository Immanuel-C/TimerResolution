// All data, proof and source was based off of https://github.com/valleyofdoom/TimerResolution/tree/main.
#include "timer_helpers.h"
#include <processthreadsapi.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <synchapi.h>
#include <winbase.h>
#include <winnt.h>
#include <winuser.h>
#include <profileapi.h>

void display_help() {
    printf(
        "-r (resulution) -> sets the global timer resolution to the provided resolution in micro seconds.\n"
        "-q -> query the global timer resolution and check how long Sleep(1) actually lasts for.\n"
        "-h -> bring up this menu.\n"
    );
}

int main(int argc, char** argv) {
    set_global_timer_resolution_requests(0x1);
    
    for (int i = 0; i < argc; i++) {
        if (strcmp("-r", argv[i]) == '\0') {
            if (argc <= (i + 1)) {
                MessageBoxA(NULL, "No resolution provided.", "Error!", MB_OK);
                return 1;
            }

            char* end;
            uint32_t resolution = strtoul(argv[i + 1], &end, 10);

            printf("Desired Resolution: %u\n", resolution);

            if (end == argv[i + 1]) {
                MessageBoxA(NULL, "Invalid resolution.", "Error!", MB_OK);
                return 1;
            }

            set_global_timer_reslution(resolution);
    
            Sleep(INFINITE);
    
        } 

        if (strcmp("-q", argv[i]) == '\0') {    
            LARGE_INTEGER start, end, freq;
            QueryPerformanceFrequency(&freq);

            if (!SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS)) {
                MessageBoxA(NULL, "Failed to set application priority to realtime. Are you running an administrative  mode?", "Errror!", MB_OK);
                return 1;
            }

            for (;;) {
                QueryPerformanceCounter(&start);
                Sleep(1);
                QueryPerformanceCounter(&end);

                double delta_between_queries = (double)(end.QuadPart - start.QuadPart) / freq.QuadPart;
                double delta_between_queries_ms = delta_between_queries * 1000;
                double delta_from_sleep = delta_between_queries_ms - 1;

                timer_resolution_values values = query_global_timer_reslution();                
                
                printf("Resolution: %.4f, Sleep(1) slept for %.4fms (delta: %.4fms)\n", 
                       (double)(values.currResolution) / 10000.0, delta_between_queries_ms, delta_from_sleep);

                // Sleep for 1000 seconds to make sure QueryPerformanceCounter works properly.
                Sleep(1000);
            }

        }

        if (strcmp("-h", argv[i]) == '\0')
            display_help();
    }

    // No input
    display_help();
    
    return 0;
} 
