
{
	'includes': [
	],

	'targets': [
		{
			'target_name': 'rtmp',
			'type': 'shared_library',

			'dependencies': [
				'jet',
				'jet-lua',
			],
			'sources': [
				'module.cc',
				'module.h',
				'message.cc',
				'message.h',

				'rtmp.cc',
				'rtmp.h',
				'amf.cc',
				'amf.h',
				'NetConnection.cc',
				'NetConnection.h',

				'lua/lrtmp.cc',
				'lua/lrtmp.h',

			],
			'include_dirs': [
				#'../../../libs/openssl/include',
			],
			'cflags': [
				'-fPIC',
				'-O0', '-g3',
			],
			'defines': [ 'JET_BUILDING_RTMP=1' ],
			'all_dependent_settings': {
			    'defines': [ 'JET_USING_RTMP=1' ],
			},
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
