
{
	'includes': [
	],

	'targets': [
		{
			'target_name': 'dpdk',
			'type': 'static_library',

			'dependencies': [
				#'utils',
			],
			'sources': [
				#'dpdkpacket.h',
				#'dpdkpacket.cc',
				#'rte_mbuf.h',
				#'rte_mbuf.c',
			],
			'include_dirs': [
				#'..',
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
			#    'defines': [ 'USING_UV_SHARED=1' ],
	'msvs_configuration_attributes': {
        'OutputDirectory':'$(SolutionDir)/../win64/$(Configuration)'
    },
			},

			#'defines': [ 'BUILDING_UV_SHARED=1' ],
			'conditions': [
				[ 'OS=="win"', {
					'defines': [
						'_WIN32_WINNT=0x0600',
						'_GNU_SOURCE',
					],
					'link_settings': {
						'libraries': [
							'-ladvapi32',
							'-liphlpapi',
							'-lpsapi',
							'-lshell32',
							'-luserenv',
							'-lws2_32'
						],
					},
				  },
				],
			],
		},
	],
}
