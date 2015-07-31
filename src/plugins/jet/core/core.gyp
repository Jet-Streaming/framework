
{
	'includes': [
	],

	'targets': [
		{
			'target_name': 'core',
			'type': 'shared_library',

			'dependencies': [
				'jet',
			],
			'sources': [
				'plugin.c',

				'identity.c',
				'fakesink.c',
			],
#			'include_dirs': [
#				'.',
#			],
			'cflags': [
				'-fPIC',
				'-O0', '-g3',
			],
			'link_settings': {
                                'libraries': [ '-lrt',  ],
                                'ldflags': [ '-pthread' ],
                        },
                        'direct_dependent_settings': {
                                'include_dirs': [
                                        '.',
                                ],
                        },
		},
	],
}
