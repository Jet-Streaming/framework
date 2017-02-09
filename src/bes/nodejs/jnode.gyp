{
  'variables': {
    'v8_use_snapshot%': 'false',
    'node_use_dtrace%': 'false',
    'node_use_lttng%': 'false',
    'node_use_etw%': 'false',
    'node_use_perfctr%': 'false',
    'node_no_browser_globals%': 'false',
    'node_use_v8_platform%': 'true',
    'node_use_bundled_v8%': 'true',
    'node_shared%': 'false',
    'force_dynamic_crt%': 0,
    'node_module_version%': '',
    'node_shared_zlib%': 'false',
    'node_shared_http_parser%': 'false',
    'node_shared_cares%': 'false',
    'node_shared_libuv%': 'false',
    'node_use_openssl%': 'true',
    'node_shared_openssl%': 'false',
    'node_v8_options%': '',
    'node_enable_v8_vtunejit%': 'false',
    'node_core_target_name%': 'jet-nodejs',
    'library_files': [
      #'../../../deps/v8/tools/splaytree.js',
      #'../../../deps/v8/tools/codemap.js',
      #'../../../deps/v8/tools/consarray.js',
      #'../../../deps/v8/tools/csvparser.js',
      #'../../../deps/v8/tools/profile.js',
      #'../../../deps/v8/tools/profile_view.js',
      #'../../../deps/v8/tools/logreader.js',
      #'../../../deps/v8/tools/tickprocessor.js',
      #'../../../deps/v8/tools/SourceMap.js',
      #'../../../deps/v8/tools/tickprocessor-driver.js',
    ],
    'conditions': [
      [ 'node_shared=="true"', {
        'node_target_type%': 'shared_library',
      }, {
        'node_target_type%': 'executable',
      }],
      [ 'OS=="win" and '
        'node_use_openssl=="true" and '
        'node_shared_openssl=="false"', {
        'use_openssl_def': 1,
      }, {
        'use_openssl_def': 0,
      }],
    ],

  },

	'targets': [
		{
			'target_name': '<(node_core_target_name)',
			'type': '<(node_target_type)',

			'dependencies': [
				'jet',
		        #'node_js2c#host',
	            #'deps/v8/src/v8.gyp:v8',
    	        #'deps/v8/src/v8.gyp:v8_libplatform',
				#'deps/zlib/zlib.gyp:zlib',
			],

            'direct_dependent_settings': {
                'include_dirs': [
                    #'..',
					'../../../deps/v8/include',
                ],
			    'defines': [
	              	#'V8_SHARED',
	              	#'USING_V8_SHARED',
				],
            },

			'include_dirs': [
		        '<(SHARED_INTERMEDIATE_DIR)', # for node_natives.h
			],

			'sources': [
				'jnode.cc',

        'debug-agent.cc',
        'async-wrap.cc',
        'env.cc',
        #'src/fs_event_wrap.cc',
        #'src/cares_wrap.cc',
        #'src/connection_wrap.cc',
        #'src/connect_wrap.cc',
        'handle_wrap.cc',
        #'src/js_stream.cc',
        'node.cc',
        'node_buffer.cc',
        'node_config.cc',
        'node_constants.cc',
        'node_contextify.cc',
        #'node_file.cc',
        #'src/node_http_parser.cc',
        'node_javascript.cc',
        #'src/node_main.cc',
        #'src/node_os.cc',
        'node_revert.cc',
        'node_util.cc',
        #'src/node_v8.cc',
        #'src/node_stat_watcher.cc',
        'node_watchdog.cc',
        #'src/node_zlib.cc',
        'node_i18n.cc',
        #'src/pipe_wrap.cc',
        #'src/signal_wrap.cc',
        #'src/spawn_sync.cc',
        'string_bytes.cc',
        #'src/stream_base.cc',
        #'src/stream_wrap.cc',
        #'src/tcp_wrap.cc',
        'timer_wrap.cc',
        #'src/tty_wrap.cc',
        #'src/process_wrap.cc',
        #'src/udp_wrap.cc',
        'uv.cc',
        # headers to make for a more pleasant IDE experience
        'async-wrap.h',
        'async-wrap-inl.h',
        #'src/base-object.h',
        #'src/base-object-inl.h',
        #'src/connection_wrap.h',
        #'src/connect_wrap.h',
        'debug-agent.h',
        'env.h',
        'env-inl.h',
        'handle_wrap.h',
        #'src/js_stream.h',
        'node.h',
        'node_buffer.h',
        'node_constants.h',
        #'src/node_file.h',
        #'src/node_http_parser.h',
        'node_internals.h',
        'node_javascript.h',
        'node_mutex.h',
        #'src/node_root_certs.h',
        'node_version.h',
        'node_watchdog.h',
        #'src/node_wrap.h',
        'node_revert.h',
        'node_i18n.h',
        #'src/pipe_wrap.h',
        #'src/tty_wrap.h',
        #'src/tcp_wrap.h',
        #'src/udp_wrap.h',
        #'src/req-wrap.h',
        #'src/req-wrap-inl.h',
        'string_bytes.h',
        #'src/stream_base.h',
        #'src/stream_base-inl.h',
        #'src/stream_wrap.h',
        'tree.h',
        'util.h',
        'util-inl.h',
        'util.cc',
        'string_search.cc',
        #'deps/http_parser/http_parser.h',
        '../../../deps/v8/include/v8.h',
        '../../../deps/v8/include/v8-debug.h',
        '<(SHARED_INTERMEDIATE_DIR)/node_natives.h',
        # javascript files to make for an even more pleasant IDE experience
        '<@(library_files)',
        # node.gyp is added to the project by default.
        'common.gypi',
			],

      'defines': [
        'NODE_ARCH="<(target_arch)"',
        'NODE_PLATFORM="<(OS)"',
        'NODE_WANT_INTERNALS=1',
        # Warn when using deprecated V8 APIs.
        'V8_DEPRECATION_WARNINGS=1',
      ],

      'conditions': [
        [ 'node_enable_d8=="true"', {
          'dependencies': [ 'deps/v8/src/d8.gyp:d8' ],
        }],
        [ 'node_use_bundled_v8=="true"', {
          'dependencies': [
            'deps/v8/tools/gyp/v8.gyp:v8',
            'deps/v8/tools/gyp/v8.gyp:v8_libplatform'
          ],
        }],
        [ 'node_use_v8_platform=="true"', {
          'defines': [
            'NODE_USE_V8_PLATFORM=1',
          ],
        }, {
          'defines': [
            'NODE_USE_V8_PLATFORM=0',
          ],
        }],
        [ 'node_tag!=""', {
          'defines': [ 'NODE_TAG="<(node_tag)"' ],
        }],
        [ 'node_v8_options!=""', {
          'defines': [ 'NODE_V8_OPTIONS="<(node_v8_options)"'],
        }],
        # No node_main.cc for anything except executable
        [ 'node_target_type!="executable"', {
          'sources!': [
            'src/node_main.cc',
          ],
        }],
        [ 'node_release_urlbase!=""', {
          'defines': [
            'NODE_RELEASE_URLBASE="<(node_release_urlbase)"',
          ]
        }],
        [ 'v8_enable_i18n_support==1', {
          'defines': [ 'NODE_HAVE_I18N_SUPPORT=1' ],
          'dependencies': [
            '<(icu_gyp_path):icui18n',
            '<(icu_gyp_path):icuuc',
          ],
          'conditions': [
            [ 'icu_small=="true"', {
              'defines': [ 'NODE_HAVE_SMALL_ICU=1' ],
          }]],
        }],
        [ 'node_use_bundled_v8=="true" and \
           node_enable_v8_vtunejit=="true" and (target_arch=="x64" or \
           target_arch=="ia32" or target_arch=="x32")', {
          'defines': [ 'NODE_ENABLE_VTUNE_PROFILING' ],
          'dependencies': [
            'deps/v8/src/third_party/vtune/v8vtune.gyp:v8_vtune'
          ],
        }],
        [ 'v8_inspector=="true"', {
          'defines': [
            'HAVE_INSPECTOR=1',
            'V8_INSPECTOR_USE_STL=1',
            'V8_INSPECTOR_USE_OLD_STL=1',
          ],
          'sources': [
            'inspector_agent.cc',
            'inspector_socket.cc',
            'inspector_socket.h',
            'inspector_agent.h',
          ],
          #'dependencies': [
          #  'deps/v8_inspector/third_party/v8_inspector/platform/'
          #      'v8_inspector/v8_inspector.gyp:v8_inspector_stl',
          #  'v8_inspector_compress_protocol_json#host',
          #],
          #'include_dirs': [
          #  'deps/v8_inspector/third_party/v8_inspector',
          #  '<(SHARED_INTERMEDIATE_DIR)/blink', # for inspector
          #],
        }, {
          'defines': [ 'HAVE_INSPECTOR=0' ]
        }],
        [ 'node_use_openssl=="true"', {
          'defines': [ 'HAVE_OPENSSL=1' ],
          #'sources': [
          #  'src/node_crypto.cc',
          #  'src/node_crypto_bio.cc',
          #  'src/node_crypto_clienthello.cc',
          #  'src/node_crypto.h',
          #  'src/node_crypto_bio.h',
          #  'src/node_crypto_clienthello.h',
          #  'src/tls_wrap.cc',
          #  'src/tls_wrap.h'
          #],
          'conditions': [
            ['openssl_fips != ""', {
              'defines': [ 'NODE_FIPS_MODE' ],
            }],
            [ 'node_shared_openssl=="false"', {
              'dependencies': [
                './deps/openssl/openssl.gyp:openssl',

                # For tests
                './deps/openssl/openssl.gyp:openssl-cli',
              ],
              # Do not let unused OpenSSL symbols to slip away
              'conditions': [
                # -force_load or --whole-archive are not applicable for
                # the static library
                [ 'node_target_type!="static_library"', {
                  'xcode_settings': {
                    'OTHER_LDFLAGS': [
                      '-Wl,-force_load,<(PRODUCT_DIR)/<(OPENSSL_PRODUCT)',
                    ],
                  },
                  'conditions': [
                    ['OS in "linux freebsd" and node_shared=="false"', {
                      'ldflags': [
                        '-Wl,--whole-archive,'
                            '<(PRODUCT_DIR)/obj.target/deps/openssl/'
                            '<(OPENSSL_PRODUCT)',
                        '-Wl,--no-whole-archive',
                      ],
                    }],
                    # openssl.def is based on zlib.def, zlib symbols
                    # are always exported.
                    ['use_openssl_def==1', {
                      'sources': ['<(SHARED_INTERMEDIATE_DIR)/openssl.def'],
                    }],
                    #['OS=="win" and use_openssl_def==0', {
                    #  'sources': [deps/zlib/win32/zlib.def'],
                    #}],
                  ],
                }],
              ],
            }]]
        }, {
          'defines': [ 'HAVE_OPENSSL=0' ]
        }],
        [ 'node_use_dtrace=="true"', {
          'defines': [ 'HAVE_DTRACE=1' ],
          'dependencies': [
            'node_dtrace_header',
            'specialize_node_d',
          ],
          'include_dirs': [ '<(SHARED_INTERMEDIATE_DIR)' ],

          #
          # DTrace is supported on linux, solaris, mac, and bsd.  There are
          # three object files associated with DTrace support, but they're
          # not all used all the time:
          #
          #   node_dtrace.o           all configurations
          #   node_dtrace_ustack.o    not supported on mac and linux
          #   node_dtrace_provider.o  All except OS X.  "dtrace -G" is not
          #                           used on OS X.
          #
          # Note that node_dtrace_provider.cc and node_dtrace_ustack.cc do not
          # actually exist.  They're listed here to trick GYP into linking the
          # corresponding object files into the final "node" executable.  These
          # object files are generated by "dtrace -G" using custom actions
          # below, and the GYP-generated Makefiles will properly build them when
          # needed.
          #
          'sources': [ 'src/node_dtrace.cc' ],
          'conditions': [
            [ 'OS=="linux"', {
              'sources': [
                '<(SHARED_INTERMEDIATE_DIR)/node_dtrace_provider.o'
              ],
            }],
            [ 'OS!="mac" and OS!="linux"', {
              'sources': [
                'src/node_dtrace_ustack.cc',
                'src/node_dtrace_provider.cc',
              ]
            }
          ] ]
        } ],
        [ 'node_use_lttng=="true"', {
          'defines': [ 'HAVE_LTTNG=1' ],
          'include_dirs': [ '<(SHARED_INTERMEDIATE_DIR)' ],
          'libraries': [ '-llttng-ust' ],
          'sources': [
            'src/node_lttng.cc'
          ],
        } ],
        [ 'node_use_etw=="true"', {
          'defines': [ 'HAVE_ETW=1' ],
          #'dependencies': [ 'node_etw' ],
          'sources': [
            'node_win32_etw_provider.h',
            'node_win32_etw_provider-inl.h',
            'node_win32_etw_provider.cc',
            'node_dtrace.cc',
            'tools/msvs/genfiles/node_etw_provider.h',
            'tools/msvs/genfiles/node_etw_provider.rc',
          ]
        } ],
        [ 'node_use_perfctr=="true"', {
          'defines': [ 'HAVE_PERFCTR=1' ],
          #'dependencies': [ 'node_perfctr' ],
          'sources': [
            'node_win32_perfctr_provider.h',
            'node_win32_perfctr_provider.cc',
            'node_counters.cc',
            'node_counters.h',
            'tools/msvs/genfiles/node_perfctr_provider.rc',
          ]
        } ],
        [ 'node_no_browser_globals=="true"', {
          'defines': [ 'NODE_NO_BROWSER_GLOBALS' ],
        } ],
        [ 'node_use_bundled_v8=="true" and v8_postmortem_support=="true"', {
          'dependencies': [ 'deps/v8/tools/gyp/v8.gyp:postmortem-metadata' ],
          'conditions': [
            # -force_load is not applicable for the static library
            [ 'node_target_type!="static_library"', {
              'xcode_settings': {
                'OTHER_LDFLAGS': [
                  '-Wl,-force_load,<(V8_BASE)',
                ],
              },
            }],
          ],
        }],

        [ 'OS=="win"', {
          'sources': [
            'backtrace_win32.cc',
            'res/node.rc',
          ],
          'defines!': [
            'NODE_PLATFORM="win"',
          ],
          'defines': [
            'FD_SETSIZE=1024',
            # we need to use node's preferred "win32" rather than gyp's preferred "win"
            'NODE_PLATFORM="win32"',
            '_UNICODE=1',
          ],
          'libraries': [ '-lpsapi.lib' ]
        }, { # POSIX
          'defines': [ '__POSIX__' ],
          'sources': [ 'src/backtrace_posix.cc' ],
        }],
      ],
    },
    {
      'target_name': 'node_js2c',
      'type': 'none',
      'toolsets': ['host'],
      'actions': [
        {
          'action_name': 'node_js2c',
          'inputs': [
            '<@(library_files)',
            '../../../config.gypi',
          ],
          'outputs': [
            '<(SHARED_INTERMEDIATE_DIR)/node_natives.h',
          ],
          'conditions': [
            [ 'node_use_dtrace=="false" and node_use_etw=="false"', {
              'inputs': [ 'notrace_macros.py' ]
            }],
            ['node_use_lttng=="false"', {
              'inputs': [ 'nolttng_macros.py' ]
            }],
            [ 'node_use_perfctr=="false"', {
              'inputs': [ 'perfctr_macros.py' ]
            }]
          ],
          'action': [
            'python',
            'tools/js2c.py',
            '<@(_outputs)',
            '<@(_inputs)',
          ],
        },
      ],
    }, # end node_js2c
  ], # end targets

	#'msvs_configuration_attributes': {
    #    'OutputDirectory':'$(SolutionDir)/../win64/$(Configuration)'
    #},
          	#'defines': [
            #	'V8_SHARED',
            #	'BUILDING_V8_SHARED',
          	#],

            'direct_dependent_settings': {
                'include_dirs': [
                    #'..',
					#'../../../deps/v8/include',
                ],
			    'defines': [
	              	#'V8_SHARED',
	              	#'USING_V8_SHARED',
				],
            },
			'all_dependent_settings': {
				'defines': [
					#'_WIN32_WINNT=0x0600',
					#'_GNU_SOURCE',
					#'LUA_BUILD_AS_DLL',
				],
			},
		#},
	#],
}
