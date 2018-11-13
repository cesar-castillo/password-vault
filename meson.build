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

# Subproject dependency
pvdbmanager_proj = subproject('pvdbmanager')
pvdbmanager_dep = pvdbmanager_proj.get_variable('pvdbmanager_dep')

# filesystem starndard library
add_project_link_arguments(['-lstdc++fs'], language: 'cpp')

# Target executable
executable('password_vault', sources, dependencies: [gtkmm_dep, pvdbmanager_dep])