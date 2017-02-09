

{
        'includes': [
        ],

        'targets': [
                {
                        'target_name': 'test-pipeline',
                        'type': 'executable',

			'dependencies': [
				'jet',

				# just for speed up startup speed when debuging
				#'lua51',
				'coremodule',
				#'rtmp',
				#'rtmcsp',
				#'http',
			],
                        'sources': [
                                'test-pipeline.cc',
                        ],
                        'include_dirs': [
                                '.',
				'..',
                        ],
                        'cflags': [
                                '-O0', '-g',
                        ],
                        'link_settings': {
                                #'libraries': [ '-lrt',  ],
                                #'ldflags': [ '-pthread' ],
                        },

                },      
        ],
}
