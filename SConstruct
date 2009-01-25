env = Environment()
env.Tool('crossmingw', toolpath = ['scons-tools'])
Export("env")
bases = SConscript("cffi/objectmodels/SConscript")
#expat = SConscript("extern/expat/SConscript")
libffi = env.Library("cffi", [bases])
env.Program("test.cpp", LIBS=["cffi"], LIBPATH=['.'])
#env.Program(["test.cpp"] + bases)

