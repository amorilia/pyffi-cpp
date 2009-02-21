// compare speed of attribute access, using various implementations

#define NUM_ATTRIBUTES 30
#define NUM_TRIALS (3000000 / NUM_ATTRIBUTES)

#include <map>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include <boost/python.hpp>
#include <boost/unordered/unordered_map.hpp>

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

void impl_hash_map(const std::vector<std::string> & names) {
	boost::unordered_map<std::string, int> dict1;
	for (int i=0; i < NUM_TRIALS; i++) {
		BOOST_FOREACH(const std::string & name, names) {
			dict1[name] = i;
		};
	};
};

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

	t = clock();
	impl_hash_map(names);
	std::cout << "hash map: " << float(clock() - t) / CLOCKS_PER_SEC << " seconds" << std::endl;

	t = clock();
	impl_python_dict(names);
	std::cout << "python dict: " << float(clock() - t) / CLOCKS_PER_SEC << " seconds" << std::endl;

	t = clock();
	impl_python_class(names);
	std::cout << "python class: " << float(clock() - t) / CLOCKS_PER_SEC << " seconds" << std::endl;
};
