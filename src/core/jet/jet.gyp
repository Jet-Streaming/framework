
{
	'includes': [
	],

	'targets': [
		{
			'target_name': 'jet',
			'type': 'shared_library',

			'dependencies': [
				'st',
			],
			'sources': [
				'jet.c',

				'log.c',

				'object.c',
				'element.c',
				'bin.c',
				'pipeline.c',
				'pad.c',
				'capability.c',

				'plugin.c',
				'videocap.c',
				'videocap_xraw.c',

				'basesrc.c',
				'basetransform.c',
				'basesink.c',
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
