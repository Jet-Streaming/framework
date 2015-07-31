

{
        'includes': [
        ],      

        'targets': [
                {       
                        'target_name': 'jet-lua',
                        'type': 'executable',

			'dependencies': [
				'jet',
				'lua-coco',
				'lua-binding',
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
