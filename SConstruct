env = Environment()
Export("env")
bases = SConscript("cffi/objectmodels/SConscript")
expat = SConscript("extern/expat/SConscript")
#libffi = Library("cffi", [bases, expat])
#Program("test.cpp", LIBS=["cffi"], LIBPATH=['.'])
Program(["test.cpp"] + bases + expat)

