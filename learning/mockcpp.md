

* 学习资料

	[用户指南](https://github.com/tanqingkuang/mockcpp/blob/master/docs/SimpleUserInstruction_zh.md)

	[中文版本说明](https://github.com/tanqingkuang/mockcpp/blob/master/docs/ChineseVersionManual.md)
	
	


* 标准格式

	```
	MOCKER(function) / MOCK_METHOD(mocker, method)
	.stubs() / defaults() / expects(never() | once() | exactly(3) | atLeast(3) | atMost(3) )
	[.before("some-mocker-id")]
	[.with( any() | eq(3) | neq(3) | gt(3) | lt(3) | spy(var_out) | checkWith(check_func) | outBound(var_out) | outBoundP(var_out_addr, var_size) | mirror(var_in_addr, var_size) | smirror(string) | contains(string) | startWith(string) | endWith(string) )]
	[.after("some-mocker-id")]
	.will( returnValue(1) | repeat(1, 20) | returnObjectList(r1, r2) | invoke(func_stub) | ignoreReturnValue() | increase(from, to) | increase(from) | throws(exception) | die(3))
	[.then(returnValue(2))]
	[.id("some-mocker-id")]
	```
	
	[]表示必须可选项
	
	|表示有多种选择

  

* .stubs/defaults/expects

  > stubs 表示指定函数的行为，不校验次数。 
  > expects 与stubs的区别就是校验次数 
  > defaults 表示定义一个默认的mock规范，但它优先级很低；如果后面有stubs或者expects重新指定函数行为，就会按照新指定的来运行。（一般用在setup中）

  

* .will/then

  

* .with

* .before/after

* .id