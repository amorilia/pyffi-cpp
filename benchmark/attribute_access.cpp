// compare speed of attribute access, using various implementations

#define NUM_ATTRIBUTES 30
#define NUM_TRIALS (3000000 / NUM_ATTRIBUTES)

#include <map>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include <boost/python.hpp>
#include <boost/version.hpp>
#if (BOOST_VERSION >= 103600)
#include <boost/unordered/unordered_map.hpp>
#endif

//! Create random string.
std::string random_string(int len) {
	std::string ret;
	for (int i=0; i < len; ++i) {
		int num;
		num = rand()%122;
		if (48 > num)
			num += 48;
		if ((57 < num) && (65 > num))
			num += 7;
		if ((90 < num) && (97 > num))
			num += 6;
		ret += (char)num;
	}
	return ret;
}

void impl_map(const std::vector<std::string> & names) {
	std::map<std::string, int> dict1;
	for (int i=0; i < NUM_TRIALS; i++) {
		BOOST_FOREACH(const std::string & name, names) {
			dict1[name] = i;
		};
	};
};

#if (BOOST_VERSION >= 103600)
void impl_hash_map(const std::vector<std::string> & names) {
	boost::unordered_map<std::string, int> dict1;
	for (int i=0; i < NUM_TRIALS; i++) {
		BOOST_FOREACH(const std::string & name, names) {
			dict1[name] = i;
		};
	};
};
#endif

#if (BOOST_VERSION >= 103600)
struct name_hash2 : std::unary_function<std::string, std::size_t> {
	std::size_t operator()(std::string const & name) const {
		std::size_t seed = 0;
		try {
			boost::hash_combine(seed, name.at(0));
			boost::hash_combine(seed, name.at(2));
			boost::hash_combine(seed, name.at(8));
			//boost::hash_combine(seed, name.at(32));
			//boost::hash_combine(seed, name.at(64));
		} catch (const std::out_of_range &) {
			// pass the exception
		};
		return seed;
	}
};

void impl_hash_map2(const std::vector<std::string> & names) {
	boost::unordered_map<std::string, int, name_hash2> dict1;
	for (int i=0; i < NUM_TRIALS; i++) {
		BOOST_FOREACH(const std::string & name, names) {
			dict1[name] = i;
		};
	};
};
#endif

#if (BOOST_VERSION >= 103600)
struct name_hash3 : std::unary_function<std::string, std::size_t> {
	std::size_t operator()(std::string const & name) const {
		std::size_t seed = 0;
		try {
			seed ^= name.at(0);
			seed ^= name.at(2);
			seed ^= name.at(8);
		} catch (const std::out_of_range &) {
			// pass the exception
		};
		return seed;
	}
};

void impl_hash_map3(const std::vector<std::string> & names) {
	boost::unordered_map<std::string, int, name_hash2> dict1;
	for (int i=0; i < NUM_TRIALS; i++) {
		BOOST_FOREACH(const std::string & name, names) {
			dict1[name] = i;
		};
	};
};
#endif

void impl_python_dict(const std::vector<std::string> & names) {
	boost::python::dict dict1;
	for (int i=0; i < NUM_TRIALS; i++) {
		BOOST_FOREACH(const std::string & name, names) {
			dict1[name] = i;
		};
	};
};

class Test {};

void impl_python_class(const std::vector<std::string> & names) {
	boost::python::object dict1 = boost::python::class_<Test>("Test");
	for (int i=0; i < NUM_TRIALS; i++) {
		BOOST_FOREACH(const std::string & name, names) {
			dict1.attr(name.c_str()) = i;
		};
	};
};

int main(void) {
	Py_Initialize();

	clock_t t;

	std::vector<std::string> names;
	for (int j=0; j < NUM_ATTRIBUTES; j++) {
		std::string name = random_string(10);
		//std::cout << name << std::endl;
		names.push_back(name);
	};

	t = clock();
	impl_map(names);
	std::cout << "map: " << float(clock() - t) / CLOCKS_PER_SEC << " seconds"<< std::endl;

#if (BOOST_VERSION >= 103600)
	t = clock();
	impl_hash_map(names);
	std::cout << "hash map: " << float(clock() - t) / CLOCKS_PER_SEC << " seconds" << std::endl;

	t = clock();
	impl_hash_map2(names);
	std::cout << "hash map 2: " << float(clock() - t) / CLOCKS_PER_SEC << " seconds" << std::endl;

	t = clock();
	impl_hash_map3(names);
	std::cout << "hash map 3: " << float(clock() - t) / CLOCKS_PER_SEC << " seconds" << std::endl;
#endif

	t = clock();
	impl_python_dict(names);
	std::cout << "python dict: " << float(clock() - t) / CLOCKS_PER_SEC << " seconds" << std::endl;

	t = clock();
	impl_python_class(names);
	std::cout << "python class: " << float(clock() - t) / CLOCKS_PER_SEC << " seconds" << std::endl;
};
