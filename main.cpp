// KwDiB-L5oLm (215949762 - 214275732 = 1674030 ranges)
#include <iostream>
#include <mutex>
#include <thread>

#include "base58.h"
#include "sha256.h"
#include "test.h"

#define PREFIX_LENGTH 5
#define ROOT_LENGTH 6

const unsigned char CHECKSUM[] = { 0xF2, 0x66, 0x5D, 0xBE };

char* prefix;
std::mutex mutex_;
Timer timer;
int threads_number;
int* threads_progresses;
int progresses_number;

void thread_function(int thread_id)
{
	unsigned char wif[52 + 1];
	memcpy(wif, prefix, PREFIX_LENGTH);
	memcpy(wif + PREFIX_LENGTH + ROOT_LENGTH, WIF_ENDING, strlen(WIF_ENDING));
	wif[52] = 0;
	int to = (thread_id + 1) * 58 / threads_number;
	unsigned char extended_key[64] = { 0 };
	extended_key[0] = 0x80;
	extended_key[33] = 0x01;
	extended_key[34] = 0x80;
	extended_key[62] = 0x01;
	extended_key[63] = 0x10;
	unsigned char digest1[64] = { 0 };
	digest1[32] = 0x80;
	digest1[62] = 0x01;
	unsigned char digest2[32];
	for (int i0 = thread_id * 58 / threads_number; i0 < to; i0++)
	{
		wif[PREFIX_LENGTH] = BASE58[i0];
		for (int i1 = 0; i1 < 58; i1++)
		{
			wif[PREFIX_LENGTH + 1] = BASE58[i1];
			for (int i2 = 0; i2 < 58; i2++)
			{
				wif[PREFIX_LENGTH + 2] = BASE58[i2];
				for (int i3 = 0; i3 < 58; i3++)
				{
					wif[PREFIX_LENGTH + 3] = BASE58[i3];
					for (int i4 = 0; i4 < 58; i4++)
					{
						wif[PREFIX_LENGTH + 4] = BASE58[i4];
						for (int i5 = 0; i5 < 58; i5++)
						{
							wif[PREFIX_LENGTH + 5] = BASE58[i5];
							base58_decode(wif, extended_key);
							sha256(extended_key, digest1);
							sha256(digest1, digest2);
							if (digest2[0] == CHECKSUM[0] && digest2[1] == CHECKSUM[1] && digest2[2] == CHECKSUM[2] && digest2[3] == CHECKSUM[3])
							{
								mutex_.lock();
								std::cout << "[W] " << (char*)wif << std::endl;
								mutex_.unlock();
							}
						}
					}
				}
			}
			mutex_.lock();
			threads_progresses[thread_id]++;
			bool log = true;
			for (int j = 0; j < threads_number; j++)
				log &= threads_progresses[j] >= threads_progresses[thread_id];
			if (log)
				std::cout << "[I] Progress = " << std::fixed << threads_progresses[thread_id] * 100.0 / progresses_number << " % [" << std::fixed << (threads_number * 58 * 58 * 58 * 58 / timer.stop() / 1000000) << " Mkeys/sec]" << std::endl;
			mutex_.unlock();
		}
	}
	mutex_.lock();
	threads_progresses[thread_id] = progresses_number;
	mutex_.unlock();
}

int main(int argc, char* argv[])
{
	std::cout.precision(2);
	//test();
	//return 0;
	//while (true)
	//{
		//for (int i = 0; i < PREFIX_LENGTH; i++)
		//	prefix[i] = BASE58[rand() % 58];
		//prefix[PREFIX_LENGTH] = 0;
	if (argc < 2)
	{
		std::cout << "[E] No parameter with WIF prefix" << std::endl;
		test();
		return 0;
	}
	if (strlen(argv[1]) != PREFIX_LENGTH)
	{
		std::cout << "[E] Wrong length of WIF prefix: " << strlen(argv[1]) << std::endl;
		return 0;
	}
	prefix = argv[1];
	for (int i = 0; i < PREFIX_LENGTH; i++)
		if (!strchr(BASE58, prefix[i]))
		{
			std::cout << "[E] Wrong symbol of WIF prefix: " << prefix[i] << std::endl;
			return 0;
		}
		std::cout << "[I] CHECKING WIF PREFIX " << prefix << ":" << std::endl;
		threads_number = std::thread::hardware_concurrency();
		if (!threads_number)
			threads_number = 1;
		std::thread* threads = new std::thread[threads_number];
		threads_progresses = new int[threads_number] { 0 };
		progresses_number = (58 / threads_number + (bool)(58 % threads_number)) * 58;
		timer = Timer();
		for (int t = 0; t < threads_number; t++)
			threads[t] = std::thread(thread_function, t);
		for (int t = 0; t < threads_number; t++)
			threads[t].join();
		delete[] threads;
		delete[] threads_progresses;
		std::cout << "[I] WIF PREFIX " << prefix << " CHECKED" << std::endl;
	//}
	return 0;
}
