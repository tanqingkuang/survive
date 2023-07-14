/**
 * @file rule.c
 * @author zhangmeng (847305142@qq.com)
 * @brief 上帝规则实现
 * @version 0.1
 * @date 2023-07-15
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#if 0
uint32 MapCreate(const char *filename)
{
    /* 如果已经初始化过了，则报错 */
    CHECK_CONDITION_AUTORETURN(MapInfoGet(MAG_INI_INFO_HIGHT), CHECK_CONDITION_EQ, 0);

    /* 读取ini配置文件中的信息更新 */
    uint32 ret = MapIniCfg(filename);
    CHECK_RET_AUTORETURN(ret);

    /* 初始化运行态数据 */
    ret = MapRunInfoInit();
    if (ret != SUCCESS) {
        MapDestory();
        return ret;
    }

    return SUCCESS;
}
#endif