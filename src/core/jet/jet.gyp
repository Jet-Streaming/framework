{
  'variables': {
    'node_shared%': 'false',
    'force_dynamic_crt%': 0,
    'node_shared_zlib%': 'false',
    'node_shared_http_parser%': 'false',
    'node_shared_cares%': 'false',
    'node_shared_libuv%': 'false',
    'node_use_openssl%': 'true',
    'node_shared_openssl%': 'false',
    'use_openssl_def%': 1,
    'conditions': [
      [ 'node_shared=="true"', {
        'node_target_type%': 'shared_library',
      }, {
        'node_target_type%': 'executable',
      }],
      [ 'OS=="win"', {
        'use_openssl_def': 1,
      }, {
        'use_openssl_def': 0,
      }],
    ],
  },

	'targets': [
		{
			'target_name': 'jet',
			#'type': 'shared_library',
			'type': 'executable',

			'dependencies': [
				#'utils',
				#'st',
				'deps/uv/uv.gyp:libuv',
				#'dpdk',

				# just for debug
				#'lua51',

				'deps/openssl/openssl.gyp:openssl',
			],
			'sources': [
				#'jet.def',
				'<(SHARED_INTERMEDIATE_DIR)/openssl.def',

				'main.cc',

				# utils
				#'log.h',
				#'log.cc',
				#'array.c',
				#'array.h',
				#'hash.h',
				#'list.h',
				#'string.h',
				#'string.c',
				#'time.h',
				#'time.c',
				'literal.h',

				# framework
				'jet.h',
				'jet.cc',

				'config.h',
				'config.cc',
				#'event.h',
				#'event.cc',

				'object.cc',
				'object.h',
				'element.h',
				'element.cc',
				'bin.h',
				'bin.cc',
				#'pipeline.h',
				#'pipeline.c',
				'pad.h',
				'pad.cc',
				#'capability.h',
				#'capability.c',

				'runtime_class.h',
				'runtime_class.cc',

				'packet.h',
				'sequence.h',
				'sequence.cc',
				'stream.h',
				'stream.cc',
				'seqpacket.h',
				'seqpacket.cc',
				'dgram.h',
				'dgram.cc',

				'ospacket.h',
				'ospacket.cc',

				#'buffer.h',
				#'buffer.c',
				#'task.h',
				#'task.c',

				'be.h',
				'be.cc',
				'module.h',
				'module.cc',
				#'registry.h',
				#'registry.cc',
				#'videocap.c',
				#'videocap_xraw.c',

				#'basesrc.c',
				#'basetransform.c',
				#'basesink.c',

			],
			'include_dirs': [
				'..',
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
                    '..',
					'../../../deps/uv/include',
					'../../../deps/openssl/openssl/include',
					'../../../deps/zlib',
                ],
        		'conditions': [
          			['OS != "win"', {
            			'defines': [
              				'_LARGEFILE_SOURCE',
              				'_FILE_OFFSET_BITS=64',
            			],
          			}],
          			['OS in "mac ios"', {
            			'defines': [ '_DARWIN_USE_64_BIT_INODE=1' ],
          			}],
          			['OS == "linux"', {
            			'defines': [ '_POSIX_C_SOURCE=200112' ],
          			}],
        		],
					'link_settings': {
						'libraries': [
							'<(PRODUCT_DIR)/jet.lib',
						],
					},

            },
			#'msvs_configuration_attributes': {
        	#	'OutputDirectory':'$(SolutionDir)/../win64/$(Configuration)'
    		#},
			'all_dependent_settings': {
			    'defines': [
					'USING_UV_SHARED=1',
					'BUILDING_JET_EXTENSION=1',
				],
			},

			#'defines': [
			#	'BUILDING_UV_SHARED=1',
			#],
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
                    # openssl.def is based on zlib.def, zlib symbols
                    # are always exported.
                    #['use_openssl_def==1', {
                    #  'sources': ['<(SHARED_INTERMEDIATE_DIR)/openssl.def'],
                    #}],
                    #['OS=="win" and use_openssl_def==0', {
                    #  'sources': ['../../../deps/zlib/win32/zlib.def'],
                    #}],

				[ 'node_shared_zlib=="false"', {
					'dependencies': [ 'deps/zlib/zlib.gyp:zlib' ],
				}, {
					'defines': [ 'ZLIB_CONST' ],
				}],
			],
		},

		    {
		      'target_name': 'mkssldef',
		      'type': 'none',
		      # TODO(bnoordhuis) Make all platforms export the same list of symbols.
		      # Teach mkssldef.py to generate linker maps that UNIX linkers understand.
			  # jk: I don't know why use_openssl_def always is 0
		      'conditions': [
		        [ 'use_openssl_def==0', {
		          'variables': {
		            'mkssldef_flags': [
		              # Categories to export.
		              '-CAES,BF,BIO,DES,DH,DSA,EC,ECDH,ECDSA,ENGINE,EVP,HMAC,MD4,MD5,'
		              'NEXTPROTONEG,PSK,RC2,RC4,RSA,SHA,SHA0,SHA1,SHA256,SHA512,SOCK,'
		              'STDIO,TLSEXT',
		              # Defines.
		              '-DWIN32',
		              # Symbols to filter from the export list.
		              '-X^DSO',
		              '-X^_',
		              '-X^private_',
		              # Base generated DEF on zlib.def
		              '-Bdeps/zlib/win32/zlib.def'
		            ],
		          },
		          'conditions': [
		            ['openssl_fips!=""', {
		              'variables': { 'mkssldef_flags': ['-DOPENSSL_FIPS'] },
		            }],
		          ],
		          'actions': [
		            {
		              'action_name': 'mkssldef',
		              'inputs': [
		                '../../../deps/openssl/openssl/util/libeay.num',
		                '../../../deps/openssl/openssl/util/ssleay.num',
		              ],
		              'outputs': ['<(SHARED_INTERMEDIATE_DIR)/openssl.def'],
		              'action': [
		                'python',
		                'tools/mkssldef.py',
		                '<@(mkssldef_flags)',
		                '-o',
		                '<@(_outputs)',
		                '<@(_inputs)',
		              ],
		            },
		          ],
		        }],
		      ],
		    },
	],
}
