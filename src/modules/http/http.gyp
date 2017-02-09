
{
	'includes': [
	],

	'targets': [
		{
			'target_name': 'http',
			'type': 'shared_library',

			'dependencies': [
				'jet',
				'jet-lua',
			],
			'sources': [
				'module.cc',
				'module.h',

				'http.cc',
				'http.h',

				# Node.JS parser (from Nginx)
				'http_parser.c',
				'http_parser.h',

				# H2O http server
				#'picohttpparser.c',

				'http_generator.cc',
				'http_generator.h',

				# Lua
				#'lua/lmodule.cc',
				'lua/lhttp.cc',
				'lua/lhttp.h',
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
                                        '../../..',
                                ],
                        },
	        #'conditions': [
	        #  [ 'be=="Lua"', {
        	#  }],
	        #],
		},
	],
}
