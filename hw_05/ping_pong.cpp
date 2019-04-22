#include <iostream>
#include <thread>
#include <atomic>

using namespace std;

static constexpr size_t Nmax = 500'000;
static atomic<bool> Ping(true);

void ping()
{
	size_t N = 0;
	while(true) {
		if (N < Nmax) {
			if (Ping) {
				cout << "ping\n";
				++N;
				Ping = false;
			}
		}
		else break;
	}
}

void pong()
{
	size_t N = 0;
	while(true) {
		if (N < Nmax) {
			if (!Ping) {
				cout << "pong\n";
				++N;
				Ping = true;
			}
		}
		else break;
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
