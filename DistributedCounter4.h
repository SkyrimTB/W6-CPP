#ifndef DISTRIBUTED_COUNTER_H
#  define DISTRIBUTED_COUNTER_H
// Add padding to avoid false sharing
#include<atomic>
#include<map>
#include<numeric>
#include<utility>
#include<vector>
#include<thread>
namespace mpcs {
	class DistributedCounter {
		using value_type = size_t;

		struct bucket {
			std::atomic<value_type> count;
			char padding[256];
		};
		static size_t const buckets{ 128 };
		std::vector<bucket> counts{ buckets };
	public:

		void operator++() {
			size_t index = std::hash<std::thread::id>()(std::this_thread::get_id()) % buckets;
			counts[index].count++;
		}
		void operator++(int) {
			++*this;
		}

		value_type get() {
			return std::accumulate(counts.begin(), counts.end(), (value_type)0,
				[](auto acc, auto &x) { return acc + x.count.load(); });
		}
	};
}
#endif