
{
	'includes': [
	],

	'targets': [
		{
			'target_name': 'utils',
			'type': 'static_library',

			'dependencies': [
			],
			'sources': [
				'log.c',
			],
			'include_dirs': [
				'..',
			],
			'cflags': [
				'-fPIC',
				'-O0', '-g3',
			],
			'link_settings': {
                                'libraries': [ '-ldl',  ],
                                'ldflags': [ '-pthread' ],
                        },
                        'direct_dependent_settings': {
                                'include_dirs': [
                                        '..',
                                ],
                        },
		},
	],
}
