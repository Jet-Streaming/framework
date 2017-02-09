
{
	'includes': [
	],

	'targets': [
		{
			'target_name': 'jet-lua',
			'type': 'shared_library',

			'dependencies': [
				'jet',
				'deps/lua-5.1.5/lua.gyp:lua51',
			],
			'sources': [
				# jet wrapper
				#'lua.def',
				'jlua.cc',
				'jlua.h',
				'jlthread.cc',
				'jlthread.h',
				#'jlobject.c',
				#'jlobject.h',
				#'jlplugin.c',
				#'jlplugin.h',
				'jlelement.cc',
				'jlelement.h',

			],
			'include_dirs': [
				#'src',
			],
			'cflags': [
				'-fPIC',
				'-O0', '-g3',
			],
			'link_settings': {
                #'libraries': [ '-ldl',  ],
                #'ldflags': [ '-pthread' ],
            },

	#'msvs_configuration_attributes': {
    #    'OutputDirectory':'$(SolutionDir)/../win64/$(Configuration)'
    #},
            'direct_dependent_settings': {
                'include_dirs': [
                    '..',
                ],
            },
			'all_dependent_settings': {
					'defines': [
						#'_WIN32_WINNT=0x0600',
						#'_GNU_SOURCE',
						#'LUA_BUILD_AS_DLL',
					],
			},
			'conditions': [
				[ 'OS=="win"', {
					'defines': [
						#'_WIN32_WINNT=0x0600',
						#'_GNU_SOURCE',
						'LUA_BUILD_AS_DLL',
					],
					'link_settings': {
						'libraries': [
							#'-ladvapi32',
						],
					},
				},],
			],
		},
	],
}
