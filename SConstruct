opts = Variables()
opts.Add(BoolVariable('debug_build', 'If the build is debug', False))

env = Environment(
    variables=opts,
    CXX='clang++',
    CPPPATH='lib/',
    LIBPATH=['.'],
    SCONS_CXX_STANDARD='c++14',
    CXXFLAGS=''
)

if env['debug_build']:
    env.Append(CXXFLAGS='-g')

env.StaticLibrary('driver', Glob('src/driver/*.cpp'))
env.StaticLibrary('lexer', Glob('src/lexer/*.cpp'))
env.StaticLibrary('parser', Glob('src/parser/*.cpp'))
env.StaticLibrary('shared', Glob('src/shared/*.cpp'))

env.Program('pilc', 'src/main.cpp', LIBS=['driver','lexer','parser','shared'])