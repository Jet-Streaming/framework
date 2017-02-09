
{
	'includes': [
	],

	'targets': [
		{
			'target_name': 'odp-generic',
			'type': 'shared_library',

			'dependencies': [
			],
			'sources': [
				# utils

				# framework
				#'task.h',
				#'task.c',
			],
			'include_dirs': [
				'include',
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
                                        #'..',
                                ],
                        },
	'msvs_configuration_attributes': {
        'OutputDirectory':'$(SolutionDir)/../win64/$(Configuration)'
    },
			'all_dependent_settings': {
	'msvs_configuration_attributes': {
        'OutputDirectory':'$(SolutionDir)/../win64/$(Configuration)'
    },
			},

			'conditions': [
				[ 'OS=="win"', {
					'defines': [
						'_WIN32_WINNT=0x0600',
						'_GNU_SOURCE',
					],
					'link_settings': {
						'libraries': [
							#'-ladvapi32',
							#'-liphlpapi',
							#'-lpsapi',
							#'-lshell32',
							#'-luserenv',
							'-lws2_32'
						],
					},
				  },
				],
			],
		},
	],
}
