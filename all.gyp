
{

        'includes': [
			#'src/core/utils/utils.gyp',
            #'src/core/st/st.gyp',
            #'common.gypi',
            #'src/libs/uv/common.gypi',
            #'src/libs/uv/uv.gyp',
            # fastpath
            #'src/core/fastpath/dpdk/dpdk.gyp',
            #'src/core/jsk/jsk.gyp',
            'src/core/jet/jet.gyp',


            #'src/libs/odp/odp-generic.gyp',
            #'src/libs/odp-dpdk/odp-dpdk.gyp',

			# business engines:
        #'conditions': [
        #  ['jet_be=="Lua"', {
			#'src/libs/lua-5.1.5/lua.gyp',
			'src/bes/lua/jlua.gyp',
        #  }],
        #  ['jet_be=="V8"', {
        #  }],
        #  ['jet_be=="false"', {
        #  }]
        #],

			'src/bes/nodejs/jnode.gyp',
            
			# modules:
			#'src/modules/jet/videotestsrc/videotestsrc.gyp',
			'src/modules/coremodule/coremodule.gyp',

			'src/modules/jcache/jcache.gyp',

			#'src/modules/adobe/base/base.gyp',
			'src/modules/adobe/rtmp/rtmp.gyp',
			'src/modules/adobe/rtmcsp/rtmcsp.gyp',

			'src/modules/http/http.gyp',

			# demos:
			#'tests/test.gyp',
        ],
}
