project('password_vault', 'cpp', default_options: ['cpp_std=c++17'])

sources = [
	'main.cpp',
	'app_controller.hpp',
	'app_controller.cpp',
	'password_page.hpp',
	'password_page.cpp',
	'opendb_page.hpp',
	'opendb_page.cpp',
	'password_list_manager.hpp',
	'password_list_manager.cpp',
	'god.hpp',
	'god.cpp'
]

# External dependency
gtkmm_dep = dependency('gtkmm-3.0')
# Other dependency
cpp_comp = meson.get_compiler('cpp')
pvdbmanager_dep = cpp_comp.find_library('pvdbmanager', dirs : ['/home/emmanuel/Development/C++/pvdbmanager/builddir'])
pvdbmanager_inc = include_directories('/home/emmanuel/Development/C++/pvdbmanager/include')

# filesystem starndard library
add_project_link_arguments(['-lstdc++fs'], language: 'cpp')

cryptopp = dependency('cryptopp')
pthread = dependency('threads')

# Target executable
executable('password_vault', sources,
	dependencies: [gtkmm_dep, pvdbmanager_dep, cryptopp, pthread],
	include_directories: [pvdbmanager_inc])