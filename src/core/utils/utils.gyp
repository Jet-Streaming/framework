
{
	'includes': [
	],

	'targets': [
		{
			'target_name': 'utils',
			'type': 'static_library',

			'dependencies': [
				'libuv',
				#'lua51',
			],
			'sources': [
				#'object.c',
				
				# Lua
				#'lua/lobject.c',
			],
			'include_dirs': [
				'..',
			],
			'cflags': [
				'-fPIC',
				'-O0', '-g3',
			],
			'link_settings': {
                                #'libraries': [ '-ldl',  ],
                                #'ldflags': [ '-pthread' ],
                        },
                        'direct_dependent_settings': {
                                'include_dirs': [
                                        '..',
                                ],
                        },
			'defines': [ 'USING_UV_SHARED=1' ],
		},
	],
}
