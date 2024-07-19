# Sapphire-EnhancedDesktop

现代化的 Windows 桌面取代/桌面增强软件

# 项目概述

- **项目**： "Sapphire-EnhancedDesktop"，意在提供一个增强的桌面体验。
- **环境**：使用 Windows API 和 Qt 框架，mingw64 编译，只保证兼容 Windows 11。
- 由原项目`Enhanced-desktop`接续开发，使用`Sapphire`作为新的软件名与原项目进行区别。

- README 文件对应版本：`V1.2.3.1`

# 项目特点

- 系统级别的植入和事件处理。
- 符合直觉和使用习惯的交互。
- 无学习成本、即开即用。
- 模块化设计，统一管理。
- 灵活的动态布局管理解决方案。
- 可自定义的外观。
- 美观、高级、精致、可选的外观特效。
- 方便开发的设计。

# 功能细节

## 系统和算法

- 接入并覆盖原生 Windows 图标层
- 获取系统主题色作为默认颜色
- 系统级别事件钩子（默认未启用）
- 扫描用户桌面和公共桌面
- 追踪快捷方式
- 转化文件信息为项目组件
- 调用系统 URL
- 网络 API 使用
- Steam 专项优化

## 全局

- 完美兼容 WallpaperEngine（需在设置中设置`当其他程序成为焦点时`为`始终运行`）
- 自选和显示图片背景（待完善）
- 组件拖动
- 组件定位
- 组件创建
- 双击隐藏（可设置始终显示）
- 聚焦动画
- 位移动画
- 从文件夹拖动
- Shift右键可以调用原生菜单
- Shift滚轮快速改变大小
- 用户可控：
  - 开关背景透视
  - 切换大图标填充模式
  - 开关光效特效追踪
  - 开关绘制矩形
  - 开关绘制光效
  - 控制所有组件的精简模式
  - 控制所有组件的缩放系数
  - 控制所有组件的圆角大小

## Style.ini 详细配置

_斜体_ 表示已在可视化组件中给出

### Color

- sleep_alpha 未聚焦的颜色 Alpha
- active_alpha 聚焦时的颜色 Alpha
- sleep_alpha_deep 深色图标未聚焦的颜色 Alpha
- active_alpha_deep 深色图标聚焦时的颜色 Alpha
- sleep_color_ratio 未聚焦的色值比率（RBG）
- active_color_ratio 聚焦时的色值比率（RBG）

### Effect

- light_alpha_start 光效的起点 Alpha
- light_alpha_end 光效的终点 Alpha
- icon_shadow_alpha 图标阴影特效 Alpha
- icon_shadow_blur_radiu 图标阴影特效 Radius（扩散半径）
- unit_shadow_alpha 所有组件阴影特效 Alpha
- unit_shadow_blur_radius 所有组件阴影特效 Radius（扩散半径）

### Render

- _unit_radius_ 组件圆角
- _ShowRect_ 绘制组件的矩形
- ShowSide 绘制组件边框（用于 Debug）
- _ShowLight_ 绘制组件光效
- _enable_background_transparent_ 背景透视
- enable_background_blur 背景模糊（未完工）
- _enable_light_track_ 特效追踪
- _enable_intime_repaint_ 即时重绘（用于 Debug）

### Preference

- _enable_image_fill_ 大图标填充
- muilt_icon_default_type 设置 Steam 图标获取方式
  - 0 小图标，1 竖图标，2 横图标（切换未完工）
- scale_fix_ratio 聚焦时缩放比率
- enable_lnk_redirect 快捷方式的图标重定向
  - 开启会获取原软件的图标，没有小箭头，但可能会有错误
  - 关闭会获取快捷方式本身的图标，有小箭头

# Todo

- [ ] 自定义图标
- [X] 更好的保存逻辑
- [ ] 文件夹缩放
- [ ] 多屏适配
- [X] 更好的右键菜单
- [X] 滚轮改变大小
- [ ] 滚动文件夹


# QA

- Q：软件概况

- A：由 QT 开发，设计/编写/测试/运维都是我一个人，开源免费，目前无盈利计划。

    软件的设计理念是 桌面端的操作习惯+移动端/现代化的功能与特性+Windows 本身的可拓展性，让用户可以无痛，几乎无学习成本的从原生桌面切换到该软件。



- Q：开发进度

- A：目前软件正像一个评论说的一样，像是一个毛坯房，正属于前期开发阶段，Bug 多，兼容性差，功能也较少，体验不够。如果按照标准软件产业流程，现在连内测的阶段都算不上，只能是 Alpha 版，所以目前并不能真正的在日常中使用起来！（这点在视频也已经说了，现在处于前期开发阶段）



- Q：兼容性问题

- A：由于该软件只在少量的设备上进行过测试，也没有专业的测试环境和流程，所以只能保证在 Winodws11-22631.3880（23H2最新），与 WallpaperEngine2.5.2.8 Steam 版，单屏运行，在理想情况下是没有任务栏窗口图标、兼容右下角和 Win+D 快速返回桌面的。

    - Windows10/7的适配：（如果不能用）还需要时间，欢迎积极反馈或者与我私信交流。
    
    - 多屏：我本身没用过多屏，不知道各位多屏用户的使用习惯和偏好是怎样的，还请多多反馈。



- Q：性能问题

- A：在我的 CPU 为7945HX 45w功耗限制下，CPU闲时不占用，活动时 CPU占用率平均1%，超过2%、3%的时候极少。不使用 GPU，内存150M 左右。



- Q：流畅度问题

- A：首先由于性能与兼容性考虑选择了 QT+CPU 绘图（Electron 懂的都懂），但是 QT 本身的更新策略更适合静态页面，所以有时会出现小卡顿、部分区域未更新的情况，这些都是比较底层的优化问题了，在不影响使用的情况下，暂时没有精力优化。


- Q：关于同类软件

- A：

    - TranslucentTB：神，建议搭配使用。
    
    - Rainmeter：个人认为作为增强软件很有用，但是有学习门槛，调试时间长，并且每个皮肤是相对静态的，若要大面积取代桌面本身的功能，会很麻烦。
    
    - SAO Uilts（2）：个人认为不是同一个赛道的，设计理念也不同。
    
    - 桌面整理软件（腾讯等）：有些确实还不错，兼容性也很好，但与该软件有“深度美化与自定义”的差距。



- Q：折腾不如原生

- A：只能说萝卜青菜各有所爱，就像当初玩机流行时代的那句 “愿你刷机半生，归来仍是 MIUI”，就像我在换了14之后不能 ROOT 也慢慢接受原生的（相对）稳定了，完全可以理解这样的观点。但是，也许折腾来折腾去，在使用或生产力方面不如原生，甚至倒退，个人认为也是值得的，因为用户有了更多的从“我想让这个地方怎么怎么样”变成了“我可以把这个地方变成我想要的样子”的改变，从移动端 ROOT 到该软件，这一点的理念是相通的。所以对于美化和折腾的必要性，见仁见智，每个人的需求不同。



如果您遇到了 BUG 或者有改进/兼容的想法，欢迎通过以下渠道反馈：



- GitHub Issue

- bilibili私信

- bilibili专栏评论区



如果您有意加入开发、帮助适配、改进外观，欢迎与我联系。

感谢所有支持和反馈！

# 须知

- 本软件目前定位为**美化**软件，而非**生产力**，如果您需要原生桌面作为生产力需要，该软件目前还不能满足您的要求。
- 本软件目前开发者只有我一人，开发进度较为缓慢，待完善功能较多，欢迎开发/美化人员加入合作。
- 本软件目前没有盈利打算，完全免费。
- 如果您喜欢，请给我一个 Star，或者在视频平台点赞，以帮助该软件让更多人看到，谢谢！
