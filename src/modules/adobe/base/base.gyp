
{
	'includes': [
	],

	'targets': [
		{
			'target_name': 'adobe_base',
			'type': 'shared_library',

			'dependencies': [
				'jet',
			],
			'sources': [
				'base.def',
				'base.c',
				'base.h',
				'message.c',
				'message.h',
			],
			'include_dirs': [
			],
			'cflags': [
				'-fPIC',
				'-O0', '-g3',
			],
			#'defines': [ 'USING_UV_SHARED=1' ],
			'link_settings': {
                        },
                        'direct_dependent_settings': {
                                'include_dirs': [
                                        '../..',
                                ],
                        },
		},
	],
}
