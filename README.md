# Sapphire-EnhancedDesktop

现代化的 Windows 桌面取代/桌面增强软件

# 项目概述

- **项目**： "Sapphire-EnhancedDesktop"，意在提供一个增强的桌面体验。
- **环境**：使用 Windows API 和 Qt 框架，mingw32 编译，只保证兼容 Windows 11。
- 由原项目`Enhanced-desktop`接续开发，使用`Sapphire`作为新的软件名与原项目进行区别。

- README 文件对应版本：`V1.2.1.1`

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
- scale_fix_ratio=1.3 聚焦时缩放比率
- enable_lnk_redirect 快捷方式的图标重定向
  - 开启会获取原软件的图标，没有小箭头，但可能会有错误
  - 关闭会获取快捷方式本身的图标，有小箭头

# 须知

- 本软件目前定位为**美化**软件，而非**生产力**，如果您需要原生桌面作为生产力需要，该软件目前还不能满足您的要求。
- 本软件目前开发者只有我一人，开发进度较为缓慢，待完善功能较多，欢迎开发/美化人员加入合作。
- 本软件目前没有盈利打算，完全免费。
- 如果您喜欢，请给我一个 Star，或者在视频平台点赞，以帮助该软件让更多人看到，谢谢！
