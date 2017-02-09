
{
	'includes': [
	],

	'targets': [
		{
			'target_name': 'lua51',
			'type': 'static_library',

			'dependencies': [
			],
			'sources': [
				# core
				'src/lapi.c',
				'src/lapi.h',
				'src/lcode.c',
				'src/lcode.h',
				'src/ldebug.c',
				'src/ldebug.h',
				'src/ldo.c',
				'src/ldo.h',
				'src/ldump.c',
				'src/lfunc.c',
				'src/lfunc.h',
				'src/lgc.c',
				'src/lgc.h',
				'src/llex.c',
				'src/llex.h',
				'src/lmem.c',
				'src/lmem.h',
				'src/lobject.c',
				'src/lobject.h',
				'src/lopcodes.c',
				'src/lopcodes.h',
				'src/lparser.c',
				'src/lparser.h',
				'src/lstate.c',
				'src/lstate.h',
				'src/lstring.c',
				'src/lstring.h',
				'src/ltable.c',
				'src/ltable.h',
				'src/ltm.c',
				'src/ltm.h',
				'src/lundump.c',
				'src/lundump.h',
				'src/lvm.c',
				'src/lvm.h',
				'src/lzio.c',
				'src/lzio.h',
				
				# lib
				'src/lauxlib.c',
				'src/lauxlib.h',
				'src/lbaselib.c',
				'src/ldblib.c',
				'src/liolib.c',
				'src/lmathlib.c',
				'src/loslib.c',
				'src/ltablib.c',
				'src/lstrlib.c',
				'src/loadlib.c',
				'src/linit.c',
			],
			'include_dirs': [
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
            },
			'all_dependent_settings': {
                'include_dirs': [
                    'src',
                ],
				'defines': [
					#'_WIN32_WINNT=0x0600',
					#'_GNU_SOURCE',
					#'LUA_BUILD_AS_DLL',
				],
			},
			'conditions': [
				[ 'OS=="win"', {
					'defines': [
						#'_WIN32_WINNT=0x0600',
						#'_GNU_SOURCE',
						'LUA_BUILD_AS_DLL',
					],
					'link_settings': {
						'libraries': [
							#'-ladvapi32',
						],
					},
				},],
			],
		},
	],
}
