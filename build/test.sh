# 自动化运行全部用例，请使用F1->run test task

# 检查前置条件
if  [ ! -f "Makefile" ]; then
    echo -e "\033[31m Makefile不存在，请先运行cmake configure\033[0m"
    exit 1
fi

# 编译
cmake ..
if mingw32-make.exe; then
    echo -e "\033[32m编译成功\033[0m"
else
    echo -e "\033[31m编译失败，请检查\033[0m"
    exit 1
fi

# 输出测试过程
if find ./ -name "unittest*exe" -exec ./{} \; > test_rst.txt ; then
    echo -e "\033[32m"
    egrep "\-.*test.*from.*ms total|  PASSED  " test_rst.txt
else
    echo -e "\033[31m用例运行失败，请检查\033[0m"
    exit 1
fi

# 检查是否有错误用例
echo -e "\033[31m"
if grep FAILED test_rst.txt; then
    echo -e "\033[0m"
else
    echo -e "\033[32m 用例全部运行，没有错误"
    echo -e "\033[0m"
fi
