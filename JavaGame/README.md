# Java小游戏

本项目使用maven构建，游戏入口为src/main/java/j05/Roguelike.java中的main函数

游戏说明：
- @代表玩家，由键盘操控。
- z和m代表怪物，其中z不会移动。m是由多线程控制移动并追逐玩家，在j05.world.CreatureController类中实现对m的控制。
- 怪物会对玩家发起攻击，玩家也可以攻击怪物。
- 游戏阶段屏幕左上角为“玩家剩余血量/玩家总血量”和“剩余怪物数/总怪物数”。游戏中的红心可以让玩家增加血量，但不会超过玩家总血量这一上限。
- @旁边有时会出现绿色的点，该绿色的点是指引玩家找到附近此时不可见的怪物。
- 按a对怪物进行攻击，按h对游戏进行保存。

1. 实现了并发：

录屏链接：https://www.bilibili.com/video/BV1w94y1P79k/

2. 实现游戏保存功能。

- 地图保存/地图加载：

  视频链接：https://www.bilibili.com/video/BV1qQ4y1u7r6/ （视频00:36之前为开始一个新的游戏，然后保存当前地图。00:36之后为加载刚才保存的地图并在该地图上进行一段时间游戏。）

  j05.gameio.MapSaver类和j05.gameio.MapLoader类分别实现了地图保存和地图加载功能。

- 进度保存/进度恢复：

  视频链接：https://www.bilibili.com/video/BV1aK41187Zr/ （视频00:30之前为开始一个新的游戏，进行一段时间游戏后保存当前进度。00:30之后为恢复刚才保存的进度并继续玩游戏。）

  j05.gameio.GameProgressSaver类和j05.gameio.GameProgressLoader类分别实现了进度保存和进度加载功能。

- 游戏过程录制/回放：

  视频链接：https://www.bilibili.com/video/BV1Zw411V7G6/ （视频01:18之前为开始一个新的游戏，同时录制游戏过程。01:18之后为回放刚才录制的游戏过程。）

  j05.gameio.GameRecorder类和j05.gameio.GamePlayback类分别实现了游戏过程录制和游戏过程回放。