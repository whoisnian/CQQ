# CQQ
CQQ即CoolQ + Qt + QQ，通过[CoolQ HTTP API 插件](https://github.com/richardchien/coolq-http-api)的WebSocket接口进行通信，使用Qt的C++绑定编写的QQ客户端。

## 测试环境
* 服务器：
  * CoolQ Docker(richardchien/cqhttp[https://cqhttp.cc/docs/4.8/#/Docker])
  * CoolQ Pro 5.13.1(190303)
  * richardchien/coolq-http-api 4.8.0
* 本地开发环境：
  * Qt Creator 4.8.2 Based on Qt 5.12.1 (GCC 8.2.1 20181127, 64 bit)

## 已实现
* [*] 获取好友列表，群组列表
* [*] 接收多种格式消息
* [*] 发送文本消息
* [*] 发送图片
* [*] 聊天图片预览
* [*] 聊天消息框自动滚动
* [*] 记录窗口大小调整

## 希望实现
* [ ] 菜单选项：清空缓存
* [ ] 菜单选项：刷新好友列表
* [ ] 菜单选项：重新连接
* [ ] 菜单选项：重置窗口布局
* [ ] 消息部分加载
* [ ] 消息显示时间戳
* [ ] 自动滚动优化，当窗口非激活状态时不自动滚动
* [ ] 文本消息中的链接自动识别
* [ ] 撤回消息
* [ ] at 群组成员
* [ ] 新消息提醒
* [ ] 托盘图标

## 说明
* Websocket需要使用心跳来维持连接，wsEVENT接口由服务端发送[心跳事件](https://cqhttp.cc/docs/4.8/#/Post?id=%E5%BF%83%E8%B7%B3)，wsAPI接口由客户端每15s（模仿服务端心跳间隔）发送一次`get_status`请求。
* 与单个聊天对象的聊天信息过多时，自动滚动会无法滚动到最底部，程序中的具体表现是`QTextBrowser`调用`setHtml()`后立即获取`verticalScrollBar()->maximum()`，得到的值不正确。推测可能是`QTextBrowser`渲染需要时间，渲染结束后才可以获得正确的值。
* CoolQ HTTP API推送的消息中少数CQ码的图片url无法打开，表现为下载失败，复制出来手动访问时显示404，推测与CoolQ HTTP API缓存有关，需要进一步验证。
* 离线消息由于CoolQ HTTP API的接口限制，可能需要特殊途径实现。
* 发送图片时存在大小限制，需要进一步测试。
* 插入emoji表情时的表格存在性能问题，拉伸选择emoji窗口时比较明显，需要换用其它方式进行选择。
* 截图功能未实现，当前的截图按钮是发送剪切版中的文件名对应的图片，可以识别`/home/nian/test.png`或者`file:///home/nian/test.png`格式的路径。可以在系统默认截图工具中设置保存图片后复制路径到剪切版，搭配系统截图工具实现发送截图功能。
* 图片预览需要放大，缩小，拖动，旋转功能，当前程序中是调用系统默认图片查看器打开图片文件。
