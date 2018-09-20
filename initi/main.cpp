#include <cstdlib>
#include <string>
#include <ctime>
#include <iostream>
#include <sys/time.h>
#include <list>
#include <fstream>
#include <cstddef>
#include <algorithm>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>




// Существует условный контейнер для записей (строк), который хранит их в сортированном порядке.
// Записи постоянно добавляются и удаляются. При этом из контейнера с той же периодичностью считываются данные по порядковому номеру.

// Например добавляем строчку "gs", при считывании элемента по 0 индексу ее же и получим.
// Затем добавляем "bnj", соответственно в нулевой позиции будет строка "bnj", в первой - "gs".
// При добавлении "kju" первые две записи не поменяют свое место, а последняя строка будет доступна по индексу 2.

// Итого необходимо реализовать контейнер, который будет иметь следующее api:
// - вставить новый элемент (строка),
// - удалить элемент (строку),
// - получить элемент по порядковому номеру с учетом сортировки.

// Контейнер должен успевать обрабатывать 100K операций в секунду.

// Для проверки алгоритма необходимо использовать приложенные тестовые данные.
// Файл insert.txt - последовательность рандомных строк.
// Файл read.txt - последовательность рандомных индексов и соотвествующих строк разделенные пробелом.

// Алгоритм проверки должен быть итеративным - вставляете один элемент из insert.txt,
// затем считываете из контейнера запись по индексу из read.txt. Результат должен совпасть с соответствующей строкой из read.txt.

// Для удобства приложен файл с заготовкой - template.txt.



using namespace __gnu_pbds;

// A red-black tree table storing strings and their order
// statistics. Note that since the tree uses
// tree_order_statistics_node_update as its update policy, then it
// includes its methods by_order and order_of_key.
typedef
tree<std::string, null_type, std::less<std::string>, rb_tree_tag,
		tree_order_statistics_node_update> // This policy updates nodes' metadata for order statistics
set_t;


using write_sequence = std::list<std::string>;
using read_sequence = std::list<std::pair<uint64_t, std::string>>;


uint64_t get_time() {
	struct timeval tv;
	gettimeofday(&tv, NULL);

	return tv.tv_sec * 1000000LL + tv.tv_usec;
}

write_sequence get_write_sequence() {
	std::ifstream infile("insert.txt");
	write_sequence result;

	std::string item;

	while (infile >> item) {
		result.emplace_back(item);
	}

	return result;
}

read_sequence get_read_sequence() {
	std::ifstream infile("read.txt");
	read_sequence result;

	uint64_t index;
	std::string str;

	while (infile >> index >> str) {
		result.emplace_back(std::make_pair(index, str));
	}

	return result;
}


class Storage
{
public:
	void insert(const std::string& str)
	{
		stg_.insert(str);
	}

	const std::string& get(uint64_t index)
	{
	    if (index > stg_.size())
	        throw(container_error());

		return *stg_.find_by_order(index);
	}

	void remove(const std::string& str)
	{
		stg_.erase(str);
	}

private:
	set_t stg_;
};


void test(const write_sequence &write, const read_sequence &read) {
	Storage st;

	uint64_t timestamp_us;
	uint64_t total_time = 0;

	write_sequence::const_iterator iitr = write.begin();
	read_sequence::const_iterator ritr = read.begin();

	while (iitr != write.end() && ritr != read.end()) {
		timestamp_us = get_time();
		st.insert(*iitr);
		total_time += get_time() - timestamp_us;

		timestamp_us = get_time();
		const std::string &str = st.get(ritr->first);
		total_time += get_time() - timestamp_us;

		if (ritr->second != str) {
			std::cout << "test failed" << std::endl;
			return;
		}

		iitr++;
		ritr++;
	}

	std::cout << "total time: " << total_time/1000000LL << " sec" << std::endl;
}



int main()
{
	write_sequence insert = get_write_sequence();
	read_sequence read = get_read_sequence();

	test(insert, read);
}
