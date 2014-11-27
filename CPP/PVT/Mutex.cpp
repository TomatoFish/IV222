#include <iostream>
#include <atomic>
#include <vector>
#include <thread>
#include <mutex>

std::mutex mut;

class spinlock
{
	std::atomic_flag mutex;
public:
	spinlock(){}
	void lock()
	{
		while (mutex.test_and_set(std::memory_order_acquire));
	}
	void unlock()
	{
		mutex.clear();
	}
};


class guardlock
{
	std::mutex & m;
public:
	explicit guardlock(std::mutex & mut) : m(mut){ m.lock(); }
	~guardlock(){ m.unlock(); }
};

void dosmth()
{
	for (int i = 0; i < 1000; i++)
	{
		guardlock lock(mut);
		std::cout << std::this_thread::get_id() << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(2));
	}
}

int main()
{
	std::vector<std::thread> v;
	for (int n = 0; n < 2; ++n) {
		v.emplace_back(dosmth);
	}
	for (auto& t : v) {
		t.join();
	}
	return 0;
}