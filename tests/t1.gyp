

{
        'includes': [
        ],      

        'targets': [
                {       
                        'target_name': 't1',
                        'type': 'executable',

			'dependencies': [
				'jet',
				#'videotestsrc',
				#'coreplugin',
			],
                        'sources': [
                                't1.c',
                        ],      
                        'include_dirs': [
                                '.',
				'..',
                        ],      
                        'cflags': [
                                '-O0', '-g',
                        ],      
                        'link_settings': {
                                'libraries': [ '-lrt',  ],
                                'ldflags': [ '-pthread' ],
                        },      
                },      
        ],      
}
