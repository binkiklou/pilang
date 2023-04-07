env = Environment(
    CXX='clang++',
    CPPPATH='lib/',
    LIBPATH=['.'],
    SCONS_CXX_STANDARD='c++14'
)

env.StaticLibrary('driver', Glob('src/driver/*.cpp'))
env.StaticLibrary('lexer', Glob('src/lexer/*.cpp'))
env.StaticLibrary('parser', Glob('src/parser/*.cpp'))
env.StaticLibrary('shared', Glob('src/shared/*.cpp'))

env.Program('pilc', 'src/main.cpp', LIBS=['driver','lexer','parser','shared'])