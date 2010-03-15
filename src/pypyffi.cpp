/*

Copyright (c) 2007-2008, Python File Format Interface
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

   * Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.

   * Redistributions in binary form must reproduce the above
     copyright notice, this list of conditions and the following
     disclaimer in the documentation and/or other materials provided
     with the distribution.

   * Neither the name of the Python File Format Interface
     project nor the names of its contributors may be used to endorse
     or promote products derived from this software without specific
     prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

*/

#include <boost/python.hpp>
#include "pyffi.hpp"

using namespace boost::python;

int main(int argc, char **argv)
{
	try {
		Py_Initialize();
		// get namespace
		object main_module = import("__main__");
		object main_namespace = main_module.attr("__dict__");

		// set some objects in the namespace
		// - import all format namespaces
		// - possibly, set up some convenience functions (load & save?)
		// - perhaps, if called from for instance a gui, automatically load
		//   data, and have it automatically saved upon exit of the
		//   interpreter
		// for now, just do a little test
		// simply import the pyffi module
		import("sys").attr("path").attr("append")(""); // make sure current dir is in path
		main_namespace["pyffi"] = import("pyffi");
		// start interactive console, using the main namespace
		import("code").attr("InteractiveConsole")(main_namespace).attr("interact")();
	} catch (error_already_set) {
		PyErr_Print();
	}
}
