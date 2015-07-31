
{
	'includes': [
	],

	'targets': [
		{
			'target_name': 'st',
			'type': 'static_library',

			'sources': [
				'sched.c',
				'stk.c',
				'sync.c',
				'key.c',
				'io.c',
				'md.S',

				'epoll.c',
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
