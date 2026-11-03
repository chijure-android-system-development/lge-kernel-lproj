/*
 * LG Optimus L3 (E400) misc_apps_apis RPC server.
 *
 * The modem calls program 0x30000006 on the apps processor, among other
 * moments when the GPS engine starts, to read the flex country and operator
 * codes. The stock 2.6.38 kernel answers these calls; without a server the
 * modem never gets its answer and resets the chip about 10 s later.
 *
 * There is no flex.xml on this device, so the flex procedures answer the
 * same as stock does in that case: result 7 with an empty string.
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/string.h>
#include <linux/types.h>
#include <asm/byteorder.h>
#include <mach/msm_rpcrouter.h>

#define E0_MISC_PROG		0x30000006
#define E0_MISC_VERS		0

#define E0_MISC_PROC_ATCMD_FACTORY	3
#define E0_MISC_PROC_ATCMD_FACTORY_LARGE	6
#define E0_MISC_PROC_FLEX_MCC		7
#define E0_MISC_PROC_FLEX_MNC		8
#define E0_MISC_PROC_FLEX_OPERATOR	9
#define E0_MISC_PROC_FLEX_COUNTRY	10

#define E0_MISC_RESULT_ERROR		7
#define E0_MISC_STRING_LEN		500

/* Same layout as the AMSS side: reply header, two values, string. */
struct e0_misc_reply {
	__be32 xid;
	__be32 type;
	__be32 reply_stat;
	__be32 accept_stat;
	__be32 verf_flavor;
	__be32 verf_length;
	__be32 ret_value1;
	__be32 ret_value2;
	u8 ret_string[E0_MISC_STRING_LEN];
};

static int e0_misc_send_result(uint32_t xid, uint32_t result)
{
	struct e0_misc_reply reply;
	int rc;

	memset(&reply, 0, sizeof(reply));
	reply.xid = cpu_to_be32(xid);
	reply.type = cpu_to_be32(1);
	reply.reply_stat = cpu_to_be32(RPCMSG_REPLYSTAT_ACCEPTED);
	reply.accept_stat = cpu_to_be32(result);

	rc = msm_rpc_server_write_raw_reply(&reply, sizeof(reply));
	if (rc < 0)
		pr_err("%s: could not write reply: %d\n", __func__, rc);

	/* Above zero tells the server thread the reply was already sent. */
	return 1;
}

static int e0_misc_rpc_call(struct msm_rpc_server *server,
			    struct rpc_request_hdr *req, unsigned len)
{
	switch (req->procedure) {
	case 0:
	case 1:
	case 2:
	case 4:
	case 5:
		return 0;
	case E0_MISC_PROC_ATCMD_FACTORY:
	case E0_MISC_PROC_ATCMD_FACTORY_LARGE:
	case E0_MISC_PROC_FLEX_MCC:
	case E0_MISC_PROC_FLEX_MNC:
	case E0_MISC_PROC_FLEX_OPERATOR:
	case E0_MISC_PROC_FLEX_COUNTRY:
		return e0_misc_send_result(req->xid, E0_MISC_RESULT_ERROR);
	default:
		return -ENODEV;
	}
}

static struct msm_rpc_server e0_misc_server = {
	.prog = E0_MISC_PROG,
	.vers = E0_MISC_VERS,
	.rpc_call = e0_misc_rpc_call,
};

static int __init e0_misc_rpc_init(void)
{
	return msm_rpc_create_server(&e0_misc_server);
}
device_initcall(e0_misc_rpc_init);
