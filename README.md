# 一、survive

用于模拟生物进化的一些策略性问题，在该项目实现中练习自己的编程和设计能力

# 二、需求

## 2.1 cmake

该需求主要练习工程创建和cmake相关技能

### 2.1.1 支持多工程独立编译

根据构想，该项目可能会有多个工程（每个工程做不同的模拟仿真），因此需要能够解决一套代码支持多个项目的编译架构

### 2.1.2 支持二进制生成时不污染源代码

在编译过程中，.o、.exe等文件能够不生成到c文件旁边，以免对相关文件夹产生污染，并且能够不使用git进行管理

### 2.1.3 支持一键编译、运行、调试目标工程

为了快速调试，因此希望每个工程都可以一键编译、运行和调试，在切换工程时应该减少修改和配置

## 2.2 googletest & mock

该需求主要练习googletest和桩功能的

### 2.2.1 支持使用googletest

希望googletest的测试用例和src文件解耦

### 2.2.2 支持使用mock功能

华为使用的mockcpp，这里想先尝试一下googletest自带的mock，如果不好用，则切换到mockcpp

### 2.2.3 支持基于独立模块的googletest

如果一个独立模块能够独立进行测试，则会对项目有更少的依赖

### 2.2.4 支持基于工程的googletest

项目也需要能够进行测试

## 2.3 ini cfg

该功能为后续其他功能从配置文件获取信息并初始化提供能力

## 2.3.1 支持根据读取配置文件

按照ini格式来获取配置文件信息

## 2.4 map

该功能为地图功能

### 2.4.1 根据配置文件生成地图

使用ini文件生成地图，文件信息包括

* 地图尺寸
* 地图资源数量


### 2.4.2 支持每天更新地图资源

每天资源个数从map.ini来，每日定期更新

### 2.4.3 支持多个生物抢占资源

这是为了解决多生物并发问题

## 2.5 上帝规则

上帝规则为通用型规则，与具体地图和生物无关，但是由于地图具有唯一性，因此上帝规则和地图规则当前并不冲突，为了保证设计的一致性，和地图对象相关的放到map中，和规则相关的放到rule中

### 2.5.1 基于配置文件来初始化规则

包括
* 每天的tick数
* 移动速度和能量消耗的比例，每个tick根据移动的步长计算消耗一次
* 视野范围和能量消耗的比例，每个tick迭代消耗一次
* 繁殖消耗的能量，每次繁殖进行消耗
* 体型和能量消耗的比例，每天结束进行消耗
* 冲突的物种，在抢占食物时的提高倍数
* 冲突时，物种的为冲突所消耗的能量
* 
### 2.5.2 提供食物抢占时的资源分配算法

抢占有共享式、争夺式等，争夺式有赢者通吃和按能力分配，争夺式需要消耗能量 —— TODO 这个当前map中，后面放到rule中

### 2.5.3 提供食物觅食算法和移动算法

* 随机算法方案
* 贪心算法方案 —— TODO，可以后续做扩展

### 2.5.4 提供繁殖算法

* 固定遗传方案
* 遗传算法方案 —— TODO，可以后续做扩展

## 2.6 生物

### 2.6.1 基于配置文件来初始化生物系统
包括
* 有多少种生物
* 每种生物的初始个数
* 每种生物的初始体型
* 每种生物的视野范围
* 每种生物的繁殖门槛
* 每种生物的移动速度
* 每种生物的觅食方案
* 每种生物的繁殖方案

### 2.6.2 觅食

调用上帝规则中的觅食算法来寻找食物，并抢占食物

### 2.6.3 繁殖

当能量大于生育门槛时，调用上帝规则中的繁殖算法

### 2.6.4 新生与死亡

任何时刻，能量小于0即死亡；物种在所有个体死亡后该物种消失；繁殖后生物个体+1。这意味
* 种群的总数只会减少
* 每个种群的个数可增可减，需要支持动态方案

# 三、TodoList

- [ ] fclose有未定义符号
- [ ] unittest对被测文件使用cmake直接引用，没有做到cmake组件化
- [X] debug show在llt下还是会有打印
- [x] mockcpp函数替换的方式未掌握
- [x] 测试用例无法直接全量运行，需要手动操作
- [x] 需要有修改ini文件的能力
- [ ] rule.c依赖map_data.h的MAP_RUN_INFO_TAKE_ANIMAL_S，map_data.c依赖rule.c的RuleResouceAllocate，形成了循环依赖