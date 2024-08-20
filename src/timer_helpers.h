#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdint.h>
/**
* @brief The return values of NtQueryTimerResolution().
*/
typedef struct {
	uint32_t 
		/// The maximun timer resolution.
		maxResolution, 
		/// The minimun timer resolution.
		minResolution, 
		/// The current timer resolution.
		currResolution;
} timer_resolution_values;
/**
 * @brief Set a registry key to make on applications timer reslution requests the global timer resolution.
 * @param val must be 0x0 or 0x1. 0x0 means turn off the global timer resolution and 0x1 means turn on the global timer reslution.
 */
void set_global_timer_resolution_requests(DWORD val);
/**
 * @brief Query the global timer reslution to test if the application is working correctly. see timer_resolution_values struct.
 */
timer_resolution_values query_global_timer_reslution();
/**
 * @brief Set the global timer resolution
 * @param The value that the timer resolution will be set to. This is in µs.
 */
void set_global_timer_reslution(uint32_t resolution);
/**
 * @brief Adjusting the timer resolution from the maximum to be a little less precise can actually help with the timer precision.
 * @param start the minimum resolution that should be tested. 
 * @param end the maximum resolution that should be tested.
 * @param increment how much should you increment the timer resolution between tests.
 * @param samples how many times a specific resolution should be tested. More means more accurate data.
 * @return the best global timer resolution for the best timer precision.
 */
uint32_t micro_adjust_timer_resolution(uint32_t start, uint32_t end, uint32_t increment, uint32_t samples);
