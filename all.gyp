
{
        'variables': {
                'with_slice': '0',
                'with_pcre': '0',
        },      
#       'cflags': [
#               '-fPIC',
#       ],      
        'includes': [
                'src/core/jet/jet.gyp',
                'src/core/st/st.gyp',

		'src/plugins/jet/videotestsrc/videotestsrc.gyp',
		'src/plugins/jet/core/core.gyp',

                'tests/t1.gyp',
        ],      
}
