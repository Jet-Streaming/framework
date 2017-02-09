
{
	'includes': [
	],

	'targets': [
		{
			'target_name': 'coremodule',
			'type': 'shared_library',

			'dependencies': [
				'jet',
				'jet-lua',
				'jet-nodejs',
			],
			'sources': [
				'module.cc',
				'module.h',

				#'identity.c',
				#'identity.h',
				#'fakesinkpad.c',
				#'fakesinkpad.h',
				#'fakesink.c',
				#'fakesink.h',
				#'tcpsrc.c',
				#'tcpsrc.h',
				#'queuepad.c',
				#'queuepad.h',
				#'queue.c',
				#'queue.h',
				'tcp.cc',
				'tcp.h',
				'ostcp.cc',
				'ostcp.h',
				#'ospacket.cc',
				#'ospacket.h',

				# Lua
				'lua/lmodule.cc',
				'lua/ltcp.cc',
				'lua/ltcp.h',
				'lua/lostcp.cc',
				'lua/lostcp.h',

				# v8
				'v8/v8tcp.cc',
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
	          #[ 'jet_be=="Lua"', {
        	  #}],
	        #],
#          	'msvs_configuration_attributes': {
#      			'OutputDirectory':'$(SolutionDir)/../win64/$(Configuration)/node_modules/coremodule/bin'
#    		},
		},
	],
}
