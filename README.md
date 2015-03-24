优化版本

1. 加入二进制解析;
2. 尽量减少解析过程中不必要的损耗，如vector的遍历等;
3. 当前版本比原始版本提高30% ~ 50%的解析速度;

[
二进制使用方法

下载xml2fb工具将相应的xml转换为二进制形式(https://github.com/puckery/xml2fb) 

1. XMLToFB -s x:\skeleton.xml;

2. XMLToFB -t x:\texture.xml;

3. 包含 "DBCCFactoryExt.h" 并用DBCCFactoryExt::getInstance()...进行测试;
]
