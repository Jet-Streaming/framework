
{
	'includes': [
	],

	'targets': [
		{
			'target_name': 'jcache',
			'type': 'shared_library',

			'dependencies': [
				'jet',
				'jet-lua',
			],
			'sources': [
				'module.cc',
				'module.h',

				'jcache.cc',
				'jcache.h',

				# Lua
				'lua/lmodule.cc',
				'lua/ljcache.cc',
				'lua/ljcache.h',
			],
#			'include_dirs': [
#				'..',
#			],
			'cflags': [
				'-fPIC',
				'-O0', '-g3',
			],
			#'defines': [ 'USING_UV_SHARED=1' ],
			'link_settings': {
                                #'libraries': [ '-lrt',  ],
                                #'ldflags': [ '-pthread' ],
                        },
                        'direct_dependent_settings': {
                                'include_dirs': [
                                        #'../../..',
                                ],
                        },
	        #'conditions': [
	        #  [ 'be=="Lua"', {
        	#  }],
	        #],
		},
	],
}
