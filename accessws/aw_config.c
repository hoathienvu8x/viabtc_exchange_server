/*
 * Description: 
 *     History: yang@haipo.me, 2017/04/21, create
 */

# include "aw_config.h"

struct settings settings;

static int read_config_from_json(json_t *root)
{
    int ret;
    ret = load_cfg_process(root, "process", &settings.process);
    if (ret < 0) {
        printf("load process config fail: %d\n", ret);
        return -__LINE__;
    }
    ret = load_cfg_log(root, "log", &settings.log);
    if (ret < 0) {
        printf("load log config fail: %d\n", ret);
        return -__LINE__;
    }
    ret = load_cfg_alert(root, "alert", &settings.alert);
    if (ret < 0) {
        printf("load alert config fail: %d\n", ret);
        return -__LINE__;
    }
    ret = load_cfg_ws_svr(root, "svr", &settings.svr);
    if (ret < 0) {
        printf("load svr config fail: %d\n", ret);
        return -__LINE__;
    }
    ret = load_cfg_svr(root, "monitor", &settings.monitor);
    if (ret < 0) {
        printf("load monitor config fail: %d\n", ret);
        return -__LINE__;
    }
    ret = load_cfg_rpc_clt(root, "matchengine", &settings.matchengine);
    if (ret < 0) {
        printf("load matchengine clt config fail: %d\n", ret);
        return -__LINE__;
    }
    ret = load_cfg_rpc_clt(root, "marketprice", &settings.marketprice);
    if (ret < 0) {
        printf("load marketprice clt config fail: %d\n", ret);
        return -__LINE__;
    }
    ret = load_cfg_rpc_clt(root, "readhistory", &settings.readhistory);
    if (ret < 0) {
        printf("load readhistory clt config fail: %d\n", ret);
        return -__LINE__;
    }
    ret = load_cfg_kafka_consumer(root, "orders", &settings.orders);
    if (ret < 0) {
        printf("load kafka orders config fail: %d\n", ret);
        return -__LINE__;
    }
    ret = load_cfg_kafka_consumer(root, "balances", &settings.balances);
    if (ret < 0) {
        printf("load kafka balances config fail: %d\n", ret);
        return -__LINE__;
    }

    ERR_RET(read_cfg_int(root, "worker_num", &settings.worker_num, false, 1));
    ERR_RET(read_cfg_str(root, "auth_url", &settings.auth_url, NULL));
    ERR_RET(read_cfg_real(root, "backend_timeout", &settings.backend_timeout, false, 1.0));
    ERR_RET(read_cfg_real(root, "cache_timeout", &settings.cache_timeout, false, 0.5));

    ERR_RET(read_cfg_real(root, "deals_interval", &settings.deals_interval, false, 0.5));
    ERR_RET(read_cfg_real(root, "price_interval", &settings.price_interval, false, 0.5));
    ERR_RET(read_cfg_real(root, "today_interval", &settings.today_interval, false, 0.5));
    ERR_RET(read_cfg_real(root, "kline_interval", &settings.kline_interval, false, 0.5));
    ERR_RET(read_cfg_real(root, "depth_interval", &settings.depth_interval, false, 0.5));

    ERR_RET(read_cfg_int(root, "depth_subscribe_max", &settings.depth_subscribe_max, false, 100));

    return 0;
}

int init_config(const char *path)
{
    json_error_t error;
    json_t *root = json_load_file(path, 0, &error);
    if (root == NULL) {
        printf("json_load_file from: %s fail: %s in line: %d\n", path, error.text, error.line);
        return -__LINE__;
    }
    if (!json_is_object(root)) {
        json_decref(root);
        return -__LINE__;
    }
    int ret = read_config_from_json(root);
    if (ret < 0) {
        json_decref(root);
        return ret;
    }
    json_decref(root);

    return 0;
}

