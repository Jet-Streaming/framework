
{
	'includes': [
	],

	'targets': [
		{
			'target_name': 'rtmcsp',
			'type': 'shared_library',

			'dependencies': [
				'jet',
				'jet-lua',
				'rtmp',
			],
			'sources': [
				'module.cc',
				'module.h',

				'rtmcsp.cc',
				'rtmcsp.h',
				#'parser.cc',
				#'parser.h',
				'handshake.cc',
				#'handshake.h',

				'lua/lrtmcsp.cc',
				'lua/lrtmcsp.h',

			],
			'include_dirs': [
				#'../../../../deps/openssl/include',
				'../..',
			],
			'cflags': [
				'-fPIC',
				'-O0', '-g3',
			],
			#'defines': [ 'USING_UV_SHARED=1' ],
			'link_settings': {
                                #'libraries': [ '-llibeay32.lib',  ],
                                #'ldflags': [ '-pthread' ],
                        },
                        #'direct_dependent_settings': {
                        #        'include_dirs': [
                        #                '../../..',
                        #        ],
                        #},
	        #'conditions': [
	        #  [ 'be=="Lua"', {
    	        #'msvs_configuration_attributes': {
    	        #	'OutputDirectory' : '$(SolutionDir)/../win64/$(Configuration)/lualib/adobe',
    	        #},
        	#  }],
	        #],

		},
	],
}
