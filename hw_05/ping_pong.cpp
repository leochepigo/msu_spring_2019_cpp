#include <iostream>
#include <thread>
#include <atomic>

using namespace std;

static constexpr size_t Nmax = 500'000;
static atomic<bool> Ping(true);

void ping()
{
	for (size_t N = 0; N < Nmax;) {
		if (Ping) {
			cout << "ping\n";
			++N;
			Ping = false;
		}
	}
}

void pong()
{
	for (size_t N = 0; N < Nmax;) {
		if (!Ping) {
			cout << "pong\n";
			++N;
			Ping = true;
		}
	}
}

int main()
{
	thread t1(ping);
	thread t2(pong);

	t1.join();
	t2.join();

	return 0;
}
