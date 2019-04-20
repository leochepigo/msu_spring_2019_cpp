#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;

static mutex m;
static condition_variable c;
static constexpr size_t Nmax = 500000;

void tabble_tennis(string data)
{
	for (size_t N = 0; N < Nmax; ++N)
	{
		unique_lock<mutex> lock(m);
		cout << data;
		c.notify_one();
		c.wait(lock);
	}
	c.notify_one();
}

int main()
{
	thread t1(tabble_tennis,"ping\n");
	thread t2(tabble_tennis,"pong\n");

	t1.join();
	t2.join();

	return 0;
}
