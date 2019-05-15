/**
 * test/test.h
 * test heap
 */

#include "test_multiproc.h"
#include "../src/program/program.h"
#include "../src/process/process.h"
#include "../src/logging/logging.h"
#include "../src/schedule/schedule.h"
#include "../src/status/status.h"
#include "../src/core/core.h"
#include <cassert>
#include <iostream>

using logging::info;
using logging::log_endl;
using std::cout;
using std::endl;

class merge_sort : public program {
private:

	handle<int> arr;
	handle<int> tmp;
	int n;

	void merge_sort_fun(handle<int> arr, handle<int> L, handle<int> R)
	{
		int Lp = L, Rp = R;
		cout << L <<  " " << R << endl;
		if (L >= R) {
			return;
		} else {
			handle<int> mid = alloc_stack<int>();
			mid = (L+R)>>1;
			merge_sort_fun(arr, (int)L, (int)mid);
			merge_sort_fun(arr, (int)mid+1, (int)R);
			handle<int> Lp = (int)L, Rp = (int)mid+1;
			handle<int> top = 0;
			while (Lp <= mid && Rp <= R) {
				if (arr[Lp] <= arr[Rp]) {
					tmp[top] = (int)(arr[Lp]);
					Lp = Lp + 1;
					top = top + 1;
				} else {
					tmp[top] = (int)(arr[Rp]);
					Rp = Rp + 1;
					top = top + 1;
				}
			}
			while (Lp <= mid) {
				tmp[top] = (int)(arr[Lp]);
				Lp = Lp + 1;
				top = top + 1;
			}
			while (Rp <= R) {
				tmp[top] = (int)(arr[Rp]);
				Rp = Rp + 1;
				top = top + 1;
			}
			for (handle<int> i = 0; i < top; i = i + 1) {
				arr[L + i] = (int)(tmp[i]);
			}
		}		
	}
	
public:

	merge_sort()
	{
		build();
	}

	virtual void static_init()
	{
		set_name("qwq");
		n = 10;
		arr = alloc_static<int>(n);
		tmp = alloc_bss<int>(n);
		size_t a = 31, b = 19260817, x = 0;
		for (size_t i = 0; i < n; i++) {
			info << (int)x << log_endl;
			arr[i].modify_in_compile(x);
			x = a * x + b;
		}
	}

	virtual void main()
	{
		cout << "merge_sort start." << endl;
		merge_sort_fun(arr, 0, n-1);
		for (handle<int> i = 0; i < n; i = i + 1) {
			info << (int)arr[i] << log_endl;
		}
		cout << "merge_sort ok." << endl;
	}
};

void test_heap()
{
	program *prog = new merge_sort;
	size_t pid = create_process();
	int ok = exec_program(pid, prog);
	schedule(0);
	while (1) {
		std::this_thread::yield();
	}
}
