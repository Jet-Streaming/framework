#include "jet.h"
#include "limits.h"
//#include "plugin.h"
//#include "object.h"
#include "element.h"
#include "bin.h"
//#include "pipeline.h"
//#include "basetransform.h"
//#include "basesink.h"
#include "config.h"
#include "be.h"
#include <openssl/hmac.h>
#include <openssl/sha.h>

//using namespace ;

//static int argc;
//static char **argv;

namespace jet {

static be *current_be;

static int
load_be(const char *filename)
{
    uv_lib_t lib;
    int rc;

    rc = uv_dlopen(filename, &lib);
    if (rc) {
        goto err;
    }

    if (!current_be) {
        // be  did not self-register
        rc = -ENOENT;
        goto err;
    }
    if (current_be->version != 0) {
        rc = -EINVAL;
        goto err;
    }

    //uv_default_loop();
    //rc = be_pending->init(argc, argv);
    //if (rc) {
    //    goto err;
    //}

    //be_pending = nullptr;
    return EOK;
err:
    // log
    //be_pending = nullptr;
    uv_dlclose(&lib);
    return rc;
}

int
register_be(be *b)
{
    if (current_be) {
        return -EEXIST;
    }
    current_be = b;
    return EOK;
}

//
int
init()
{
    int rc;

    //rc = jconf_init();
    // parse config file

    //if (startup has Lua script) {
    if (1) {
        rc = load_be(
			//"jet-lua.dll");
			"jet-nodejs.dll");
    }

    //for (i = 0; i != bes.nelts; ++i) {
    //    rc = be[i]->pre_class_init();
    //    assert(EOK == rc);
    //}

    //for (i = 0; i != bes.nelts; ++i) {
    //    rc = be[i]->post_class_init();
    //    assert(EOK == rc);
    //}

    //for (auto i = be::bes.begin(); i != be::bes.end(); ++i) {
    //    rc = (*i)->init();
    //    if (EOK != rc) {
    //        return rc;
    //    }
    //}

	HMAC_CTX hmac;
	HMAC_CTX_init(&hmac);
	uv_version_string();

    return rc;
}

int
run(int argc, char *argv[])
{
    int rc;
    //int i;
    //jbe_t **be;

//    st_task_exit(NULL);

    //be = jet_bes.elts;
    //for (i = 0; i != jet_bes.nelts; ++i) {
    //    rc = be[i]->run();
    //    assert(EOK == rc);
    //}

    rc = current_be->start(argc, argv);
    rc = uv_run(uv_default_loop(), UV_RUN_NOWAIT); // dummy
    return rc;
}

} // namespace jet