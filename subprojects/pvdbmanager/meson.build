
project('pvdbmanager', 'cpp', default_options: ['cpp_std=c++17'])

incdir = include_directories('include')
sources = [
	'src/db_manager.cpp',
	'src/crypto/util.hpp',
	'src/crypto/cipher.hpp',
	'src/crypto/aes_cipher.hpp',
	'src/crypto/aes_cipher.cpp'
]

# Dependencies to build the library
cryptopp_dep = dependency('cryptopp')
pthread_dep = dependency('threads')

# Library
pvdbmanager = static_library('pvdbmanager', sources, include_directories: incdir, dependencies: [cryptopp_dep, pthread_dep])

# Declare Dependency (in Meson)
pvdbmanager_dep = declare_dependency(include_directories: incdir, link_with: pvdbmanager)