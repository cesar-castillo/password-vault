project('password_vault', 'cpp', default_options: ['cpp_std=c++17'])

sources = [
	'src/main.cpp',
	'src/app_controller.hpp',
	'src/app_controller.cpp',
	'src/password_page.hpp',
	'src/password_page.cpp',
	'src/opendb_page.hpp',
	'src/opendb_page.cpp',
	'src/password_list_manager.hpp',
	'src/password_list_manager.cpp',
	'src/god.hpp',
	'src/god.cpp'
]

# External dependency
gtkmm_dep = dependency('gtkmm-3.0')

# Subproject dependency
pvdbmanager_proj = subproject('pvdbmanager')
pvdbmanager_dep = pvdbmanager_proj.get_variable('pvdbmanager_dep')

# filesystem starndard library
add_project_link_arguments(['-lstdc++fs'], language: 'cpp')

# Target executable
executable('password_vault', sources, dependencies: [gtkmm_dep, pvdbmanager_dep])