
{
	'includes': [
	],

	'targets': [
		{
			'target_name': 'videotestsrc',
			'type': 'shared_library',

			'dependencies': [
				'jet',
			],
			'sources': [
				'srcpad.c',
				'videotestsrc.c',
			],
#			'include_dirs': [
#				'.',
#			],
			'cflags': [
				'-fPIC',
				'-O0', '-g3',
			],
			'link_settings': {
                                #'libraries': [ '-lrt',  ],
                                #'ldflags': [ '-pthread' ],
                        },
                        'direct_dependent_settings': {
                                'include_dirs': [
                                        '.',
                                ],
                        },
		},
	],
}
