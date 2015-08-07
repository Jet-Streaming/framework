
{
	'includes': [
	],

	'targets': [
		{
			'target_name': 'jsk',
			'type': 'static_library',

			'dependencies': [
				'utils',
				'st',
			],
			'sources': [
				'socket.c',
			],
			'include_dirs': [
				'..',
			],
			'cflags': [
				'-fPIC',
				'-O0', '-g', '-DDEBUG',
			],
			'link_settings': {
                                'libraries': [ '-lrt',  ],
                                #'ldflags': [ '-pthread' ],
                        },
                        'direct_dependent_settings': {
                                'include_dirs': [
                                        '..',
                                ],
                        },
		},
	],
}
